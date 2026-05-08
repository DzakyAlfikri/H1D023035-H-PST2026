# Pertanyaan Multitasking dan Komunikasi Antar Task

## 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Ketiga task berjalan secara **bergantian (concurrent execution)**, bukan benar-benar bersamaan. Ini terjadi karena:
1. **FreeRTOS Scheduler** adalah bagian dari library yang mengatur jalannya setiap task
2. **Pembagian Waktu (Time Slicing)**: Processor hanya memiliki satu core, sehingga hanya satu task yang bisa berjalan dalam satu waktu
3. **Task Bergantian Cepat**: Scheduler akan:
   - Menjalankan Task 1 selama periode waktu tertentu
   - Menyimpan state Task 1 (context save)
   - Beralih ke Task 2 dan menjalankannya
   - Menyimpan state Task 2
   - Beralih ke Task 3 dan menjalankannya
   - Kembali lagi ke Task 1, dan seterusnya...

### Waktu Pergantian:
- Pergantian task terjadi sangat cepat (dalam hitungan milidetik)
- Hal ini menciptakan **ilusi** bahwa semua task berjalan bersamaan
- Dari perspektif pengguna, tampak seperti tiga LED berkedip dan counter naik secara simultan

### Prioritas Task:
Dalam program `Multitasking.ino`, semua task memiliki **prioritas yang sama (1)**, sehingga scheduler memberikan waktu eksekusi yang adil untuk setiap task. Jika salah satu task memiliki prioritas lebih tinggi, maka task tersebut akan mendapat alokasi waktu lebih banyak.

---

## 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!

### Langkah-langkah Menambah Task Keempat:

#### Langkah 1: Deklarasi Task Baru
Tambahkan deklarasi function di bagian atas, setelah deklarasi task yang ada:

```cpp
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );
void TaskBlink3( void *pvParameters );  // ← Task keempat (baru)
```

#### Langkah 2: Buat Task di setup()
Tambahkan `xTaskCreate()` untuk task keempat di dalam function `setup()`:

```cpp
// ===================== PEMBUATAN TASK 4 =====================
// Membuat task keempat untuk mengedipkan LED di pin 10
xTaskCreate(
  TaskBlink3,       // Nama function yang akan dijalankan
  "task4",          // Nama deskriptif task
  128,              // Ukuran stack (bytes)
  NULL,             // Parameter yang dikirim ke task
  1,                // Priority (1 = sama tinggi dengan task lain)
  NULL              // Handle task (tidak diperlukan)
);
```

#### Langkah 3: Definisikan Function Task Keempat
Tambahkan function baru di akhir program:

```cpp
// ===================== TASK 4 - BLINK LED PIN 10 =====================
// Task untuk mengedipkan LED di pin 10 dengan interval 250ms (on/off)
void TaskBlink3(void *pvParameters) {
  // Set pin 10 sebagai output untuk LED
  pinMode(10, OUTPUT);

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // Tampilkan status task ke Serial Monitor
    Serial.println("Task4");
    
    // Nyalakan LED
    digitalWrite(10, HIGH);
    // Tahan selama 250ms (on time)
    vTaskDelay(250 / portTICK_PERIOD_MS);
    
    // Matikan LED
    digitalWrite(10, LOW);
    // Tahan selama 250ms (off time)
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}
```

---

## 3. Modifikasi Program dengan Potensiometer untuk Mengontrol Kecepatan LED

### Program Lengkap (Multitasking dengan Potensiometer):

```cpp
// ===================== LIBRARY INCLUDE =====================
// Mengimport library Arduino_FreeRTOS untuk mendukung multitasking
#include <Arduino_FreeRTOS.h>

// ===================== PIN SETUP =====================
// Pin untuk membaca nilai potensiometer
const int potensioPin = A0;

// ===================== DEKLARASI TASK =====================
// Forward declaration untuk ketiga task yang akan dijalankan
void TaskBlink1( void *pvParameters );   // Task untuk menggerakkan LED pada pin 9
void TaskBlink2( void *pvParameters );   // Task untuk menggerakkan LED pada pin 8
void TaskBlink3( void *pvParameters );   // Task untuk menggerakkan LED pada pin 10 (kecepatan variable)
void Taskprint( void *pvParameters );    // Task untuk menampilkan counter dan nilai potensiometer

// ===================== VARIABEL GLOBAL =====================
// Variabel untuk menyimpan nilai potensiometer (dibaca oleh task 1, digunakan oleh task 3)
volatile int potensioValue = 0;

void setup() {
  // ===================== INISIALISASI SERIAL =====================
  // Aktifkan komunikasi serial untuk monitoring output task
  Serial.begin(9600); // Baud rate standar 9600

  // ===================== SETUP PIN POTENSIOMETER =====================
  // Pin A0 secara otomatis menjadi input analog, tidak perlu pinMode()

  // ===================== PEMBUATAN TASK 1 =====================
  // Task untuk membaca nilai potensiometer dari pin A0
  xTaskCreate(
    TaskBlink1,       // Nama function yang akan dijalankan
    "task1",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== PEMBUATAN TASK 2 =====================
  // Membuat task kedua untuk mengedipkan LED di pin 8 (fixed speed)
  xTaskCreate(
    TaskBlink2,       // Nama function yang akan dijalankan
    "task2",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== PEMBUATAN TASK 3 =====================
  // Task untuk mengedipkan LED di pin 10 dengan kecepatan yang diatur oleh potensiometer
  xTaskCreate(
    TaskBlink3,       // Nama function yang akan dijalankan
    "task3",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== PEMBUATAN TASK 4 =====================
  // Membuat task keempat untuk menampilkan data monitoring
  xTaskCreate(
    Taskprint,        // Nama function yang akan dijalankan
    "task4",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== MEMULAI SCHEDULER =====================
  // Mulai FreeRTOS scheduler untuk menjalankan semua task
  vTaskStartScheduler();
}

// ===================== LOOP KOSONG =====================
// Loop tidak digunakan karena semua proses ditangani oleh FreeRTOS scheduler
void loop() {
}

// ===================== TASK 1 - BACA POTENSIOMETER =====================
// Task untuk membaca nilai potensiometer dan menyimpannya ke variabel global
void TaskBlink1(void *pvParameters) {
  // ===================== SETUP PIN 9 (LED BLINK FIXED) =====================
  pinMode(9, OUTPUT);

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // ===================== PEMBACAAN POTENSIOMETER =====================
    // Baca nilai analog dari pin A0 (rentang 0-1023)
    potensioValue = analogRead(potensioPin);
    
    // ===================== KONTROL LED PIN 9 (FIXED SPEED) =====================
    // Nyalakan LED
    digitalWrite(9, HIGH);
    // Tahan selama 200ms (on time - fixed)
    vTaskDelay(200 / portTICK_PERIOD_MS);
    
    // Matikan LED
    digitalWrite(9, LOW);
    // Tahan selama 200ms (off time - fixed)
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK 2 - BLINK LED PIN 8 (FIXED SPEED) =====================
// Task untuk mengedipkan LED di pin 8 dengan interval 300ms (on/off)
void TaskBlink2(void *pvParameters) {
  // Set pin 8 sebagai output untuk LED
  pinMode(8, OUTPUT);

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // Tampilkan status task ke Serial Monitor
    Serial.println("Task2");
    
    // Nyalakan LED
    digitalWrite(8, HIGH);
    // Tahan selama 300ms (on time)
    vTaskDelay(300 / portTICK_PERIOD_MS);
    
    // Matikan LED
    digitalWrite(8, LOW);
    // Tahan selama 300ms (off time)
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK 3 - BLINK LED PIN 10 (VARIABLE SPEED) =====================
// Task untuk mengedipkan LED di pin 10 dengan kecepatan yang diatur oleh potensiometer
void TaskBlink3(void *pvParameters) {
  // Set pin 10 sebagai output untuk LED
  pinMode(10, OUTPUT);

  // Variabel untuk menyimpan delay yang dikonversi dari nilai potensiometer
  int delayTime = 0;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // ===================== KONVERSI NILAI POTENSIOMETER =====================
    // Map nilai potensiometer (0-1023) menjadi delay time (50-500ms)
    // 0 → 50ms (cepat)
    // 1023 → 500ms (lambat)
    delayTime = map(potensioValue, 0, 1023, 50, 500);
    
    // ===================== KONTROL LED DENGAN KECEPATAN VARIABLE =====================
    // Nyalakan LED
    digitalWrite(10, HIGH);
    // Tahan selama delay yang ditentukan oleh potensiometer
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    
    // Matikan LED
    digitalWrite(10, LOW);
    // Tahan selama delay yang ditentukan oleh potensiometer
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

// ===================== TASK 4 - MONITORING DATA =====================
// Task untuk menampilkan nilai potensiometer dan counter ke Serial Monitor
void Taskprint(void *pvParameters) {
  // Variabel untuk menyimpan nilai counter
  int counter = 0;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // Increment counter setiap kali loop berjalan
    counter++;
    
    // ===================== TAMPILAN DATA COUNTER =====================
    Serial.print("Counter: ");
    Serial.println(counter);
    
    // ===================== TAMPILAN DATA POTENSIOMETER =====================
    Serial.print("Potensiometer Value: ");
    Serial.print(potensioValue);
    Serial.print(" | Delay (ms): ");
    // Hitung dan tampilkan delay time yang sedang digunakan
    int delayTime = map(potensioValue, 0, 1023, 50, 500);
    Serial.println(delayTime);
    
    // Tambahkan separator untuk readability
    Serial.println("---");
    
    // Tahan selama 1000ms (1 detik) sebelum print data berikutnya
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
```
### Penjelasan Modifikasi:

#### 1. **Penambahan Sensor (Potensiometer)**
   - Pin A0 digunakan untuk membaca nilai analog potensiometer (0-1023)
   - Nilai ini disimpan di variabel global `potensioValue`
   - Variabel `volatile` digunakan karena diakses oleh multiple task

#### 2. **Task 1 (Baca Potensiometer + Fixed LED)**
   - Membaca nilai potensiometer dan menyimpannya
   - Mengontrol LED pin 9 dengan kecepatan tetap (200ms)

#### 3. **Task 3 (LED Kecepatan Variable)**
   - Membaca nilai `potensioValue` yang telah diupdate oleh Task 1
   - Mengkonversi nilai potensiometer (0-1023) menjadi delay time (50-500ms)
   - LED di pin 10 berkedip dengan kecepatan yang berubah sesuai potensiometer

#### 4. **Task 4 (Monitoring)**
   - Menampilkan nilai potensiometer secara real-time
   - Menampilkan delay time yang sedang digunakan
   - Membantu debugging dan verifikasi program

