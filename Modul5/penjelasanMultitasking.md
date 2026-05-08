# Modul 5 – Multitasking dengan FreeRTOS

## Penjelasan Program

Program ini menggunakan FreeRTOS (Free Real-Time Operating System) untuk menjalankan tiga task secara bersamaan (concurrent execution). Ketiga task berjalan bergantian dengan sangat cepat, menciptakan ilusi bahwa semuanya berjalan bersamaan. Setiap task mengendalikan LED berbeda atau menampilkan counter.

---

## Library dan Setup Awal

### 1. Include Library Arduino_FreeRTOS

```cpp
#include <Arduino_FreeRTOS.h>
```
**Penjelasan:** Library Arduino_FreeRTOS memungkinkan penggunaan FreeRTOS OS pada papan Arduino. Library ini menyediakan fungsi-fungsi untuk membuat task, scheduler, delay, dan manajemen task lainnya.

---

## Deklarasi Task

### 2. Forward Declaration (Deklarasi Awal Function)

```cpp
void TaskBlink1( void *pvParameters );  // Task untuk menggerakkan LED pada pin 9
void TaskBlink2( void *pvParameters );  // Task untuk menggerakkan LED pada pin 8
void Taskprint( void *pvParameters );   // Task untuk menampilkan counter
```

**Penjelasan:**
- Forward declaration digunakan untuk memberitahu compiler bahwa function ini akan didefinisikan di bawah
- Parameter `void *pvParameters` adalah parameter yang dapat dikirimkan ke task (saat ini tidak digunakan, bisa diisi `NULL`)
- Ketiga function ini adalah function utama yang akan dijalankan sebagai task terpisah

---

## Fungsi Setup

### 3. Inisialisasi Komunikasi Serial

```cpp
void setup() {
  Serial.begin(9600);
```
**Penjelasan:** Membuka komunikasi serial dengan baud rate 9600 untuk menampilkan output dari ketiga task ke Serial Monitor.

### 4. Pembuatan Task 1 - TaskBlink1

```cpp
  xTaskCreate(
    TaskBlink1,       // Nama function yang akan dijalankan
    "task1",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );
```

**Penjelasan Parameter:**

| Parameter | Nilai | Penjelasan |
|-----------|-------|-----------|
| Function | `TaskBlink1` | Function yang akan dijalankan oleh task |
| Name | `"task1"` | Nama deskriptif untuk debugging (opsional) |
| Stack Size | `128` | Ukuran memory stack untuk task (bytes) |
| Parameters | `NULL` | Data yang dikirimkan ke task (tidak ada) |
| Priority | `1` | Tingkat prioritas (0=rendah, 3=tinggi) |
| Handle | `NULL` | Pointer ke task handle (tidak diperlukan) |

**Fungsi xTaskCreate():**
- Membuat task baru dan menambahkannya ke scheduler
- Task akan dijalankan oleh scheduler secara bergantian
- Stack size harus cukup untuk menyimpan variabel lokal task

### 5. Pembuatan Task 2 - TaskBlink2

```cpp
  xTaskCreate(
    TaskBlink2,       // Task untuk LED pin 8
    "task2",
    128,
    NULL,
    1,                // Priority sama dengan Task 1
    NULL
  );
```

**Penjelasan:** Membuat task kedua dengan cara yang sama seperti Task 1, tapi menjalankan function `TaskBlink2`.

### 6. Pembuatan Task 3 - Taskprint

```cpp
  xTaskCreate(
    Taskprint,        // Task untuk menampilkan counter
    "task3",
    128,
    NULL,
    1,                // Priority sama dengan Task 1 dan 2
    NULL
  );
```

**Penjelasan:** Membuat task ketiga untuk menampilkan counter ke Serial Monitor.

### 7. Memulai FreeRTOS Scheduler

```cpp
  vTaskStartScheduler();
}
```

**Penjelasan:**
- Fungsi `vTaskStartScheduler()` memulai FreeRTOS scheduler
- Setelah fungsi ini dipanggil, program tidak akan kembali ke setup()
- Scheduler akan terus mengelola eksekusi ketiga task secara bergantian
- **PENTING:** Jangan panggil function lain setelah `vTaskStartScheduler()`

---

## Fungsi Loop Kosong

### 8. Loop Function Tidak Digunakan

```cpp
void loop() {
}
```

**Penjelasan:**
- Function `loop()` adalah bagian dari Arduino framework, tapi tidak digunakan dalam program FreeRTOS
- Semua proses dikendalikan oleh FreeRTOS scheduler melalui task
- Loop() tetap ada tapi kosong untuk mematuhi struktur Arduino framework

---

## Task 1 - TaskBlink1

### 9. Konfigurasi Pin LED

```cpp
void TaskBlink1(void *pvParameters) {
  pinMode(9, OUTPUT);
```

**Penjelasan:**
- Set pin 9 sebagai output untuk mengendalikan LED
- Fungsi `pinMode()` harus dipanggil **di dalam task**, bukan di setup()
- Setiap task yang menggunakan pin harus mengkonfigurasi pinnya sendiri

### 10. Loop Tak Terbatas Task

```cpp
  while(1) {
```

**Penjelasan:**
- Setiap task harus memiliki loop tak terbatas untuk terus berjalan
- Loop ini akan dijalankan berulang kali selamanya
- Ketika task mengalami delay atau blocking, scheduler akan beralih ke task lain

### 11. Menampilkan Status Task

```cpp
    Serial.println("Task1");
```

**Penjelasan:**
- Menampilkan teks "Task1" ke Serial Monitor setiap kali loop berjalan
- Ini membantu user mengetahui task mana yang sedang berjalan

### 12. Menghidupkan LED

```cpp
    digitalWrite(9, HIGH);
```

**Penjelasan:**
- Mengirimkan tegangan HIGH (5V) ke pin 9
- LED menyala karena menerima tegangan positif

### 13. Delay ON - Waktu LED Menyala

```cpp
    vTaskDelay(200 / portTICK_PERIOD_MS);
```

**Penjelasan:**
- Fungsi `vTaskDelay()` menunda task selama waktu tertentu
- Parameter: `200 / portTICK_PERIOD_MS` = 200 milidetik
- **PENTING:** Gunakan `vTaskDelay()` bukan `delay()` di dalam task FreeRTOS
- Saat delay, scheduler akan beralih ke task lain, bukan idle

| Function | Digunakan di | Efek |
|----------|-----------|------|
| `delay()` | setup(), loop() normal | CPU idle, task tidak berjalan |
| `vTaskDelay()` | Dalam task FreeRTOS | CPU beralih ke task lain (efisien) |

### 14. Mematikan LED

```cpp
    digitalWrite(9, LOW);
```

**Penjelasan:**
- Mengirimkan tegangan LOW (0V/GND) ke pin 9
- LED mati karena tidak ada tegangan

### 15. Delay OFF - Waktu LED Mati

```cpp
    vTaskDelay(200 / portTICK_PERIOD_MS);
```

**Penjelasan:**
- Delay selama 200 milidetik saat LED mati
- Total periode: 200ms ON + 200ms OFF = 400ms
- LED berkedip dengan frekuensi: 1000ms / 400ms = 2.5 Hz

### Task 1 Complete
```cpp
  }
}
```

---

## Task 2 - TaskBlink2

### 16. Setup Pin dan Loop

```cpp
void TaskBlink2(void *pvParameters) {
  pinMode(8, OUTPUT);

  while(1) {
    Serial.println("Task2");
    
    digitalWrite(8, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    
    digitalWrite(8, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
```

**Penjelasan:**
- Task 2 sama seperti Task 1, tapi dengan perbedaan:
  - Menggunakan pin 8 (bukan pin 9)
  - Delay 300ms untuk ON dan OFF (bukan 200ms)
  - Total periode: 300ms ON + 300ms OFF = 600ms
  - Frekuensi: 1000ms / 600ms ≈ 1.67 Hz

---

## Task 3 - Taskprint

### 17. Inisialisasi Variabel Counter

```cpp
void Taskprint(void *pvParameters) {
  int counter = 0;

  while(1) {
```

**Penjelasan:**
- Membuat variabel counter untuk menghitung berapa kali task dijalankan
- Counter di-inisialisasi dengan nilai 0
- Variabel `counter` adalah variabel lokal yang disimpan di stack task ini

### 18. Increment Counter

```cpp
    counter++;
```

**Penjelasan:**
- Menambah nilai counter sebesar 1
- Operator `++` adalah shorthand untuk `counter = counter + 1`

### 19. Menampilkan Nilai Counter

```cpp
    Serial.println(counter);
```

**Penjelasan:**
- Menampilkan nilai counter ke Serial Monitor
- Contoh output: 1, 2, 3, 4, 5, ... (terus meningkat)

### 20. Delay Task Print

```cpp
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

**Penjelasan:**
- Delay 500 milidetik sebelum loop berikutnya
- Counter naik setiap 500ms
- Output terlihat lebih lambat dibanding Task 1 dan 2

---