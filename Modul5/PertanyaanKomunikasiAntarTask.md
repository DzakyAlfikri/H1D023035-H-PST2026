# Pertanyaan Komunikasi Antar Task

## 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Kedua task berjalan secara **bergantian (concurrent execution)**, bukan benar-benar bersamaan. Berikut mekanisme kerjanya:

### Mekanisme Eksekusi:

**a) Task Bergantian dengan FreeRTOS Scheduler:**
- Task `read_data` membaca sensor dan mengirim data ke queue
- Task `display` menunggu menerima data dari queue
- Scheduler secara berkala berganti antara kedua task


**b) Penjelasan Alur:**
1. `read_data` membaca sensor (simulasi: 10ms)
2. `read_data` mengirim data ke queue via `xQueueSend()`
3. `display` menerima data dari queue via `xQueueReceive()`
4. `display` menampilkan data ke Serial (10ms)
5. Kedua task delay 100ms
6. Cycle berulang

---

## 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!

### Jawaban: **TIDAK, program ini aman dari race condition**

### Mengapa Tidak Ada Race Condition?

**a) Queue adalah Thread-Safe:**
```cpp
// xQueueSend() dan xQueueReceive() adalah atomic operations
// Artinya operasi selesai dalam satu "tick" tanpa interupsi

xQueueSend(my_queue, &x, portMAX_DELAY);    // Atomic - aman
xQueueReceive(my_queue, &x, portMAX_DELAY); // Atomic - aman
```

**b) FreeRTOS Queue Mechanism:**
- Queue menggunakan internal mutex/semaphore
- Hanya satu task yang bisa akses queue dalam satu waktu
- Task lain akan BLOCKED sampai akses selesai

**c) Diagram Perlindungan Queue:**
```
Task 1 (read_data) ingin send:
[LOCK QUEUE] → [COPY DATA KE QUEUE] → [UNLOCK QUEUE]
Task 2 (display) harus WAIT di sini

Task 2 (display) ingin receive:
[LOCK QUEUE] → [COPY DATA DARI QUEUE] → [UNLOCK QUEUE]
Task 1 (read_data) harus WAIT di sini
```

### Mengapa Queue Aman?

**Queue menggunakan Interlocking Mechanism:**
```
struct readings x;

// Task 1 - Atomic operation
xQueueSend(my_queue, &x, portMAX_DELAY);
// Seluruh struct &x disalin sekaligus tanpa interupsi

// Task 2 - Atomic operation
xQueueReceive(my_queue, &x, portMAX_DELAY);
// Seluruh data diambil sekaligus tanpa interupsi
```

---

## 3. Modifikasi Program dengan Sensor DHT Sesungguhnya

### Program Lengkap dengan Sensor DHT:

```cpp
// ===================== LIBRARY INCLUDE =====================
// Mengimport library Arduino_FreeRTOS untuk mendukung multitasking
#include <Arduino_FreeRTOS.h>
// Mengimport library queue untuk komunikasi antar task
#include <queue.h>
// Mengimport library SimpleDHT untuk sensor DHT
#include <SimpleDHT.h>

// ===================== PIN SETUP =====================
// Pin untuk sensor DHT (data pin)
const int dhtPin = 2;  // Pin Digital 2 untuk data DHT

// ===================== INISIALISASI SENSOR DHT =====================
// Membuat objek sensor DHT22 (bisa juga SimpleDHT11)
SimpleDHT22 dht22(dhtPin);

// ===================== STRUKTUR DATA =====================
// Struct untuk menyimpan data pembacaan sensor (temperatur dan humidity)
struct readings {
  float temp;  // Nilai temperatur dari sensor (float untuk presisi)
  float h;     // Nilai humidity (kelembaban) dari sensor
};

// ===================== DEKLARASI QUEUE =====================
// Handle untuk queue yang digunakan komunikasi antar task
QueueHandle_t my_queue;

void setup() {
  // ===================== INISIALISASI SERIAL =====================
  // Aktifkan komunikasi serial untuk monitoring output
  Serial.begin(9600); // Baud rate standar 9600

  // ===================== INFORMASI AWAL =====================
  Serial.println("===== DHT22 Sensor Multi-Task System =====");
  Serial.println("Initializing sensors and tasks...");
  delay(2000); // Tunggu sensor siap

  // ===================== PEMBUATAN QUEUE =====================
  // Membuat queue dengan kapasitas 1 item dan ukuran setiap item sama dengan struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));
  
  if(my_queue == NULL) {
    Serial.println("ERROR: Could not create queue!");
    while(1);
  }

  // ===================== PEMBUATAN TASK 1 =====================
  // Membuat task untuk membaca data dari sensor DHT
  xTaskCreate(read_data, "read sensors", 256, NULL, 2, NULL);

  // ===================== PEMBUATAN TASK 2 =====================
  // Membuat task untuk menampilkan data yang telah dibaca
  xTaskCreate(display, "display", 256, NULL, 1, NULL);

  Serial.println("Tasks created successfully!");
  Serial.println("=====================================\n");
}

// ===================== LOOP KOSONG =====================
// Loop tidak digunakan karena semua proses ditangani oleh FreeRTOS scheduler
void loop() {}

// ===================== TASK 1 - MEMBACA DATA SENSOR DHT =====================
// Task untuk membaca data temperatur dan humidity dari sensor DHT, lalu mengirimnya ke queue
void read_data(void *pvParameters) {
  // Variabel untuk error handling
  int err = SimpleDHTErrSuccess;
  
  // Variabel struct untuk menyimpan data pembacaan sensor
  struct readings x;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  for(;;) {
    // ===================== PEMBACAAN SENSOR DHT22 =====================
    // Membaca sensor DHT22 (mengambil 1-2 detik)
    // read2() membaca temperatur dan humidity sekaligus
    // Format: dht22.read2(&temp, &humidity, &err)
    err = dht22.read2(&x.temp, &x.h, NULL);
    
    // ===================== ERROR HANDLING =====================
    // Cek apakah pembacaan sensor berhasil
    if (err != SimpleDHTErrSuccess) {
      // Jika ada error, gunakan nilai default
      Serial.print("Read DHT22 failed: ");
      Serial.println(err);
      x.temp = 0.0;
      x.h = 0.0;
    }

    // ===================== PENGIRIMAN DATA KE QUEUE =====================
    // Mengirimkan data struct readings ke queue
    // portMAX_DELAY berarti menunggu sampai queue siap menerima data
    if(xQueueSend(my_queue, &x, portMAX_DELAY) == pdPASS) {
      // ✓ Data berhasil dikirim ke queue
    } else {
      // ✗ Gagal mengirim data
      Serial.println("Queue send failed!");
    }
    
    // ===================== DELAY =====================
    // Tahan selama 2000ms (2 detik) sebelum membaca sensor berikutnya
    // Sensor DHT membutuhkan delay minimal 2 detik antar pembacaan
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK 2 - MENAMPILKAN DATA SENSOR =====================
// Task untuk menerima data dari queue dan menampilkannya ke Serial Monitor
void display(void *pvParameters) {
  // Variabel struct untuk menyimpan data yang diterima dari queue
  struct readings x;
  
  // Counter untuk penampilan
  int displayCount = 0;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  for(;;) {
    // ===================== PENERIMAAN DATA DARI QUEUE =====================
    // Menerima data struct readings dari queue
    // pdPASS berarti data berhasil diterima dari queue
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      
      // Increment display counter
      displayCount++;
      
      // ===================== TAMPILAN HEADER =====================
      Serial.print("===== Reading #");
      Serial.print(displayCount);
      Serial.println(" =====");
      
      // ===================== TAMPILAN DATA TEMPERATUR =====================
      // Menampilkan label temperatur ke Serial Monitor
      Serial.print("Temperature: ");
      // Menampilkan nilai temperatur dengan 2 desimal
      Serial.print(x.temp, 2);  // 2 = jumlah digit desimal
      Serial.println(" °C");

      // ===================== TAMPILAN DATA HUMIDITY =====================
      // Menampilkan label humidity ke Serial Monitor
      Serial.print("Humidity: ");
      // Menampilkan nilai humidity dengan 1 desimal
      Serial.print(x.h, 1);     // 1 = jumlah digit desimal
      Serial.println(" %");

      // ===================== STATUS SENSOR =====================
      // Validasi data dari sensor
      if(x.temp > 0 && x.h > 0) {
        Serial.println("Status: ✓ Valid");
        
        // Tambahan: interpretasi kondisi lingkungan
        if(x.temp > 30) {
          Serial.println("Condition: HOT");
        } else if(x.temp < 15) {
          Serial.println("Condition: COLD");
        } else {
          Serial.println("Condition: COMFORTABLE");
        }
        
        if(x.h > 70) {
          Serial.println("Humidity: HIGH (Lembab)");
        } else if(x.h < 30) {
          Serial.println("Humidity: LOW (Kering)");
        } else {
          Serial.println("Humidity: NORMAL");
        }
      } else {
        Serial.println("Status: ✗ Invalid - Sensor Error");
      }

      // ===================== SEPARATOR =====================
      Serial.println("-----------------------------------\n");
    }
    
    // Delay kecil untuk task display (tidak perlu lama)
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
```

### Penjelasan Program Modifikasi:

#### 1. **Menggunakan Sensor DHT22 Nyata**
```cpp
SimpleDHT22 dht22(dhtPin);  // Membuat objek sensor DHT22
err = dht22.read2(&x.temp, &x.h, NULL);  // Membaca sensor
```

#### 2. **Perubahan Data Type**
```cpp
// Sebelum: int temp, int h (integer)
// Sesudah: float temp, float h (floating-point untuk presisi)
struct readings {
  float temp;  // Bisa menyimpan 25.5°C
  float h;     // Bisa menyimpan 65.3%
};
```

#### 3. **Error Handling**
```cpp
if (err != SimpleDHTErrSuccess) {
  Serial.print("Read DHT22 failed: ");
  Serial.println(err);
}
```

#### 4. **Delay Pembacaan**
```cpp
// Sensor DHT22 membutuhkan minimal 2 detik antar pembacaan
vTaskDelay(2000 / portTICK_PERIOD_MS);
```

#### 5. **Format Penampilan**
```cpp
Serial.print(x.temp, 2);  // Tampilkan dengan 2 desimal
Serial.print(x.h, 1);     // Tampilkan dengan 1 desimal
```

#### 6. **Interpretasi Data**
```cpp
// Analisis kondisi berdasarkan pembacaan
if(x.temp > 30) {
  Serial.println("Condition: HOT");
} else if(x.temp < 15) {
  Serial.println("Condition: COLD");
}
```