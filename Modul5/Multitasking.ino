// ===================== LIBRARY INCLUDE =====================
// Mengimport library Arduino_FreeRTOS untuk mendukung multitasking
#include <Arduino_FreeRTOS.h>

// ===================== DEKLARASI TASK =====================
// Forward declaration untuk ketiga task yang akan dijalankan
void TaskBlink1( void *pvParameters );  // Task untuk menggerakkan LED pada pin 9
void TaskBlink2( void *pvParameters );  // Task untuk menggerakkan LED pada pin 8
void Taskprint( void *pvParameters );   // Task untuk menampilkan counter

void setup() {
  // ===================== INISIALISASI SERIAL =====================
  // Aktifkan komunikasi serial untuk monitoring output task
  Serial.begin(9600); // Baud rate standar 9600

  // ===================== PEMBUATAN TASK 1 =====================
  // Membuat task pertama untuk mengedipkan LED di pin 9
  xTaskCreate(
    TaskBlink1,       // Nama function yang akan dijalankan
    "task1",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== PEMBUATAN TASK 2 =====================
  // Membuat task kedua untuk mengedipkan LED di pin 8
  xTaskCreate(
    TaskBlink2,       // Nama function yang akan dijalankan
    "task2",          // Nama deskriptif task
    128,              // Ukuran stack (bytes)
    NULL,             // Parameter yang dikirim ke task
    1,                // Priority (1 = sama tinggi)
    NULL              // Handle task (tidak diperlukan)
  );

  // ===================== PEMBUATAN TASK 3 =====================
  // Membuat task ketiga untuk menampilkan counter ke serial
  xTaskCreate(
    Taskprint,        // Nama function yang akan dijalankan
    "task3",          // Nama deskriptif task
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

// ===================== TASK 1 - BLINK LED PIN 9 =====================
// Task untuk mengedipkan LED di pin 9 dengan interval 200ms (on/off)
void TaskBlink1(void *pvParameters) {
  // Set pin 9 sebagai output untuk LED
  pinMode(9, OUTPUT);

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // Tampilkan status task ke Serial Monitor
    Serial.println("Task1");
    
    // Nyalakan LED
    digitalWrite(9, HIGH);
    // Tahan selama 200ms (on time)
    vTaskDelay(200 / portTICK_PERIOD_MS);
    
    // Matikan LED
    digitalWrite(9, LOW);
    // Tahan selama 200ms (off time)
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK 2 - BLINK LED PIN 8 =====================
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

// ===================== TASK 3 - PRINT COUNTER =====================
// Task untuk menampilkan counter yang terus bertambah dengan interval 500ms
void Taskprint(void *pvParameters) {
  // Variabel untuk menyimpan nilai counter
  int counter = 0;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  while(1) {
    // Increment counter setiap kali loop berjalan
    counter++;
    
    // Tampilkan nilai counter ke Serial Monitor
    Serial.println(counter);
    
    // Tahan selama 500ms sebelum increment counter berikutnya
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}