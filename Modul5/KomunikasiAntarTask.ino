// ===================== LIBRARY INCLUDE =====================
// Mengimport library Arduino_FreeRTOS untuk mendukung multitasking
#include <Arduino_FreeRTOS.h>
// Mengimport library queue untuk komunikasi antar task
#include <queue.h>

// ===================== STRUKTUR DATA =====================
// Struct untuk menyimpan data pembacaan sensor (temperatur dan humidity)
struct readings {
  int temp;  // Nilai temperatur dari sensor
  int h;     // Nilai humidity (kelembaban) dari sensor
};

// ===================== DEKLARASI QUEUE =====================
// Handle untuk queue yang digunakan komunikasi antar task
QueueHandle_t my_queue;

void setup() {
  // ===================== INISIALISASI SERIAL =====================
  // Aktifkan komunikasi serial untuk monitoring output
  Serial.begin(9600); // Baud rate standar 9600

  // ===================== PEMBUATAN QUEUE =====================
  // Membuat queue dengan kapasitas 1 item dan ukuran setiap item sama dengan struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // ===================== PEMBUATAN TASK 1 =====================
  // Membuat task untuk membaca data dari sensor
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);

  // ===================== PEMBUATAN TASK 2 =====================
  // Membuat task untuk menampilkan data yang telah dibaca
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

// ===================== LOOP KOSONG =====================
// Loop tidak digunakan karena semua proses ditangani oleh FreeRTOS scheduler
void loop() {}

// ===================== TASK 1 - MEMBACA DATA SENSOR =====================
// Task untuk membaca data temperatur dan humidity, lalu mengirimnya ke queue
void read_data(void *pvParameters) {
  // Variabel struct untuk menyimpan data pembacaan sensor
  struct readings x;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  for(;;) {
    // ===================== PEMBACAAN SENSOR =====================
    // Simulasi pembacaan nilai temperatur dari sensor
    x.temp = 54;
    // Simulasi pembacaan nilai humidity dari sensor
    x.h = 30;

    // ===================== PENGIRIMAN DATA KE QUEUE =====================
    // Mengirimkan data struct readings ke queue
    // portMAX_DELAY berarti menunggu sampai queue siap menerima data
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    // ===================== DELAY =====================
    // Tahan selama 100ms sebelum membaca data sensor berikutnya
    vTaskDelay(100);
  }
}

// ===================== TASK 2 - MENAMPILKAN DATA SENSOR =====================
// Task untuk menerima data dari queue dan menampilkannya ke Serial Monitor
void display(void *pvParameters) {
  // Variabel struct untuk menyimpan data yang diterima dari queue
  struct readings x;

  // Loop tak terbatas untuk menjalankan task secara terus-menerus
  for(;;) {
    // ===================== PENERIMAAN DATA DARI QUEUE =====================
    // Menerima data struct readings dari queue
    // pdPASS berarti data berhasil diterima dari queue
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      
      // ===================== TAMPILAN DATA TEMPERATUR =====================
      // Menampilkan label temperatur ke Serial Monitor
      Serial.print("temp = ");
      // Menampilkan nilai temperatur yang diterima dari queue
      Serial.println(x.temp);

      // ===================== TAMPILAN DATA HUMIDITY =====================
      // Menampilkan label humidity ke Serial Monitor
      Serial.print("humidity = ");
      // Menampilkan nilai humidity yang diterima dari queue
      Serial.println(x.h);
    }
  }
}