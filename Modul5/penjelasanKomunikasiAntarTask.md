# Modul 5 – Komunikasi Antar Task dengan Queue

## Penjelasan Program

Program ini menggunakan FreeRTOS Queue untuk implementasi komunikasi antar task. Task `read_data` membaca data sensor (simulasi) dan mengirimkan data melalui queue, sementara task `display` menerima data dari queue dan menampilkannya ke Serial Monitor. Queue memastikan data transfer yang aman dan terhindar dari race condition.

---

## Library dan Setup Awal

### 1. Include Library Arduino_FreeRTOS

```cpp
#include <Arduino_FreeRTOS.h>
```
**Penjelasan:** Library utama FreeRTOS untuk Arduino. Menyediakan fungsi task, scheduler, dan primitif sinkronisasi seperti queue, semaphore, dan mutex.

### 2. Include Library Queue

```cpp
#include <queue.h>
```
**Penjelasan:** Library queue FreeRTOS yang menyediakan antrian thread-safe untuk komunikasi antar task. Queue mencegah race condition dengan menggunakan internal locking mechanism.

---

## Struktur Data

### 3. Struktur untuk Menyimpan Data Sensor

```cpp
struct readings {
  int temp;  // Nilai temperatur dari sensor
  int h;     // Nilai humidity (kelembaban) dari sensor
};
```

**Penjelasan:**
- Struct `readings` digunakan untuk menggabungkan dua data (temperatur dan humidity) dalam satu unit
- Data akan dikirim melalui queue sebagai struct, bukan value individual
- Ukuran struct = 2 × 4 bytes (int) = 8 bytes
- Dengan struct, lebih mudah mengirim data yang related bersamaan

**Keuntungan Menggunakan Struct:**
```cpp
//  Tanpa struct - rumit dan rawan error
xQueueSend(temp_queue, &temp, portMAX_DELAY);
xQueueSend(humidity_queue, &h, portMAX_DELAY);
// Kedua data bisa tidak sinkron

// ✓ Dengan struct - clean dan aman
struct readings data = {54, 30};
xQueueSend(my_queue, &data, portMAX_DELAY);
// Kedua data selalu bersama
```

---

## Deklarasi Queue

### 4. Handle Queue Global

```cpp
QueueHandle_t my_queue;
```

**Penjelasan:**
- `QueueHandle_t` adalah tipe data untuk handle (penunjuk) ke queue
- `my_queue` adalah variabel global yang menyimpan referensi queue
- Handle ini digunakan oleh kedua task untuk akses queue
- **PENTING:** Deklarasi di global scope agar bisa diakses oleh semua task

---

## Fungsi Setup

### 5. Inisialisasi Komunikasi Serial

```cpp
void setup() {
  Serial.begin(9600);
```
**Penjelasan:** Membuka komunikasi serial dengan baud rate 9600 untuk menampilkan data yang diterima dari queue.

### 6. Pembuatan Queue

```cpp
  my_queue = xQueueCreate(1, sizeof(struct readings));
```

**Penjelasan Parameter:**

| Parameter | Nilai | Penjelasan |
|-----------|-------|-----------|
| Capacity | `1` | Queue dapat menampung maksimal 1 item |
| Item Size | `sizeof(struct readings)` | Ukuran setiap item = 8 bytes |
| Return Value | `my_queue` | Handle ke queue yang baru dibuat |

**Penjelasan xQueueCreate():**
- Membuat queue dengan kapasitas 1 item
- Setiap item memiliki ukuran 8 bytes (sizeof struct readings)
- Queue mengalokasikan memory sebesar: 1 × 8 = 8 bytes (plus overhead)
- Jika queue full, `xQueueSend()` akan blocking

**Contoh dengan Kapasitas Berbeda:**
```cpp
// ✓ Queue dengan kapasitas 5 item
my_queue = xQueueCreate(5, sizeof(struct readings));
// Bisa menampung 5 data sebelum full

// ✓ Queue dengan kapasitas 1 item (seperti di program)
my_queue = xQueueCreate(1, sizeof(struct readings));
// Hanya bisa menampung 1 data, jika ada data baru, yang lama ditimpa
```

### 7. Pembuatan Task 1 - read_data

```cpp
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
```

**Penjelasan:**
- Membuat task untuk membaca sensor
- Stack size 128 bytes untuk menyimpan variabel lokal
- Priority 0 (rendah), tapi karena hanya 2 task, prioritas tidak terlalu penting
- Function `read_data()` akan dijalankan sebagai task terpisah

### 8. Pembuatan Task 2 - display

```cpp
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
```

**Penjelasan:** Membuat task untuk menampilkan data yang diterima dari queue. Struktur sama dengan pembuatan Task 1.

---

## Fungsi Loop Kosong

### 9. Loop Function Tidak Digunakan

```cpp
void loop() {}
```

**Penjelasan:** Sama seperti program multitasking, loop() tidak digunakan karena semua dikendalikan oleh FreeRTOS scheduler.

---

## Task 1 - Pembaca Sensor (read_data)

### 10. Deklarasi Variabel Struct Lokal

```cpp
void read_data(void *pvParameters) {
  struct readings x;

  for(;;) {
```

**Penjelasan:**
- Membuat variabel lokal `x` bertipe `struct readings`
- Loop `for(;;)` adalah loop tak terbatas (equivalent dengan `while(1)`)
- Setiap iterasi loop, `x` akan diisi data sensor baru

### 11. Simulasi Pembacaan Sensor - Temperatur

```cpp
    x.temp = 54;
```

**Penjelasan:**
- Mengisi field `temp` dari struct `x` dengan nilai 54
- Ini adalah simulasi pembacaan sensor (hardcoded)
- Pada aplikasi real, akan diganti dengan pembacaan ADC atau sensor library
- Contoh dengan sensor real: `x.temp = dht22.readTemperature();`

### 12. Simulasi Pembacaan Sensor - Humidity

```cpp
    x.h = 30;
```

**Penjelasan:**
- Mengisi field `h` (humidity) dari struct `x` dengan nilai 30
- Kombinasi temp=54 dan h=30 akan dikirim sebagai satu unit ke queue

### 13. Mengirim Data ke Queue

```cpp
    xQueueSend(my_queue, &x, portMAX_DELAY);
```

**Penjelasan Parameter:**

| Parameter | Nilai | Penjelasan |
|-----------|-------|-----------|
| Queue Handle | `my_queue` | Queue tujuan |
| Data Pointer | `&x` | Pointer ke data yang akan dikirim (address dari x) |
| Timeout | `portMAX_DELAY` | Tunggu sampai queue siap (jangan timeout) |
| Return | `pdPASS` | Jika berhasil mengirim |

**Penjelasan xQueueSend():**
```cpp
// Proses internal xQueueSend():
1. LOCK queue (task lain tidak bisa akses)
2. Cek apakah queue penuh
   - Jika penuh: BLOCK task, tunggu ada tempat
   - Jika ada tempat: COPY data ke queue
3. UNLOCK queue
4. Return pdPASS atau error code
```

### 14. Delay Pembacaan Sensor

```cpp
    vTaskDelay(100);
```

**Penjelasan:**
- Delay 100 tick (sekitar 100ms pada Arduino standard)
- Sensor dibaca setiap 100ms = 10 kali per detik
- Saat delay, scheduler beralih ke task lain (task display bisa berjalan)

**Note:** Pada sensor real seperti DHT, delay minimum harus 2 detik karena sensor membutuhkan waktu untuk stabil.

---

## Task 2 - Penampil Data (display)

### 15. Deklarasi Variabel Struct Lokal

```cpp
void display(void *pvParameters) {
  struct readings x;

  for(;;) {
```

**Penjelasan:**
- Membuat variabel lokal `x` untuk menerima data dari queue
- Loop tak terbatas untuk terus menerima dan menampilkan data
- Variabel `x` di display() BERBEDA dari variabel `x` di read_data() (lokal masing-masing)

### 16. Menerima Data dari Queue

```cpp
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
```

**Penjelasan Parameter:**

| Parameter | Nilai | Penjelasan |
|-----------|-------|-----------|
| Queue Handle | `my_queue` | Queue sumber |
| Data Pointer | `&x` | Pointer ke variabel yang akan menerima data |
| Timeout | `portMAX_DELAY` | Tunggu sampai ada data (jangan timeout) |
| Return | `pdPASS` | Jika berhasil menerima data |

**Penjelasan xQueueReceive():**
```cpp
// Proses internal xQueueReceive():
1. LOCK queue (task lain tidak bisa akses)
2. Cek apakah queue kosong
   - Jika kosong: BLOCK task, tunggu ada data
   - Jika ada data: COPY data dari queue ke &x
3. Hapus data dari queue (queue jadi kosong)
4. UNLOCK queue
5. Return pdPASS
```

**Condition Check:**
- Jika `xQueueReceive()` mengembalikan `pdPASS`, berarti data berhasil diterima
- Jika return error code, data tidak valid (jarang terjadi dengan `portMAX_DELAY`)

### 17. Menampilkan Data Temperatur

```cpp
      Serial.print("temp = ");
      Serial.println(x.temp);
```

**Penjelasan:**
- `Serial.print()` menampilkan teks "temp = " tanpa newline
- `Serial.println()` menampilkan nilai `x.temp` dengan newline
- Output contoh: "temp = 54"

### 18. Menampilkan Data Humidity

```cpp
      Serial.print("humidity = ");
      Serial.println(x.h);
```

**Penjelasan:**
- Sama seperti temperatur, menampilkan "humidity = " diikuti nilai humidity
- Output contoh: "humidity = 30"

---