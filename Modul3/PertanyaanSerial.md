# Praktikum Serial UART LED Control

1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika '2' aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem

---

## Jawaban Pertanyaan

### 1) Proses dari input keyboard hingga LED menyala/mati

Alurnya sebagai berikut:

1. User mengetik **'1' atau '0'** pada **Serial Monitor** di komputer
2. Data dikirim dari komputer ke Arduino melalui **komunikasi serial UART**
3. Arduino mengecek apakah ada data masuk menggunakan

   ```cpp
   Serial.available()
   ```
4. Jika ada data, Arduino membaca karakter dengan

   ```cpp
   char data = Serial.read();
   ```
5. Arduino melakukan pengecekan:

   * Jika `'1'` → LED menyala
   * Jika `'0'` → LED mati
6. Arduino mengontrol LED menggunakan:

   ```cpp
   digitalWrite(PIN_LED, HIGH); // nyala
   digitalWrite(PIN_LED, LOW);  // mati
   ```
7. Arduino mengirim feedback ke Serial Monitor:

   * "LED ON"
   * "LED OFF"

Jadi alurnya:

Keyboard → Serial Monitor → UART → Arduino → Serial.read() → if-else → digitalWrite → LED

---

### 2) Mengapa menggunakan Serial.available() ?

Digunakan untuk **mengecek apakah data sudah tersedia** sebelum dibaca.

```cpp
if (Serial.available() > 0)
```

Tanpa ini, Arduino akan membaca data walaupun **belum ada data masuk**.

### Jika dihapus

Masalah yang terjadi:

* Serial.read() membaca data kosong
* Nilai bisa random
* LED bisa menyala/mati sendiri
* Serial Monitor bisa spam "Perintah tidak dikenal"

Jadi `Serial.available()` berfungsi sebagai **pengaman pembacaan data**.

---

### 3) Modifikasi Program Input '2' → LED Berkedip

### Program Arduino (Blink Mode)

```cpp
const int PIN_LED = 12;   // Pin LED
char mode = '0';          // Mode LED (0=off,1=on,2=blink)

unsigned long previousMillis = 0; // penyimpan waktu sebelumnya
const long interval = 500;        // interval kedip 500ms
bool ledState = LOW;              // status LED

void setup() {
  Serial.begin(9600); // memulai komunikasi serial
  pinMode(PIN_LED, OUTPUT); // set pin LED sebagai output

  Serial.println("Ketik:");
  Serial.println("1 = LED ON");
  Serial.println("0 = LED OFF");
  Serial.println("2 = LED BLINK");
}

void loop() {

  // cek apakah ada data masuk
  if (Serial.available() > 0) {

    char data = Serial.read(); // baca data

    if (data == '1') {
      mode = '1'; // mode nyala
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ON");
    }

    else if (data == '0') {
      mode = '0'; // mode mati
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED OFF");
    }

    else if (data == '2') {
      mode = '2'; // mode blink
      Serial.println("LED BLINK");
    }

    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // MODE BLINK TANPA DELAY
  if (mode == '2') {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {

      previousMillis = currentMillis;

      ledState = !ledState;

      digitalWrite(PIN_LED, ledState);
    }
  }

}
```

### Penjelasan Tiap Baris

**Deklarasi Pin LED**
```cpp
const int PIN_LED = 12;
```
Digunakan untuk menentukan pin LED pada Arduino

**Variabel Mode**
```cpp
char mode = '0';
```
Digunakan untuk menyimpan mode LED:
- 0 = mati
- 1 = nyala
- 2 = berkedip

**Variabel millis**
```cpp
unsigned long previousMillis = 0;
const long interval = 500;
bool ledState = LOW;
```
- `previousMillis`: Menyimpan waktu sebelumnya
- `interval`: Interval kedip LED (500 ms)
- `ledState`: Menyimpan status LED saat ini

**Setup**
```cpp
Serial.begin(9600);
```
Memulai komunikasi serial dengan baudrate 9600

```cpp
pinMode(PIN_LED, OUTPUT);
```
Mengatur pin LED sebagai output

**Loop**
```cpp
if (Serial.available() > 0)
```
Mengecek apakah ada data masuk

```cpp
char data = Serial.read();
```
Membaca data dari serial monitor

- Jika input '1': `mode = '1'` → LED menyala
- Jika input '0': `mode = '0'` → LED mati
- Jika input '2': `mode = '2'` → LED berkedip

**Mode Blink**
```cpp
if (mode == '2')
```
Menjalankan mode berkedip

```cpp
unsigned long currentMillis = millis();
```
Mengambil waktu sekarang

```cpp
if (currentMillis - previousMillis >= interval)
```
Jika waktu sudah mencapai interval

```cpp
ledState = !ledState;
```
Membalik kondisi LED

```cpp
digitalWrite(PIN_LED, ledState);
```
Menyalakan / mematikan LED

---

### 4) Menggunakan delay() atau millis() ?

Lebih baik menggunakan **millis()**

### Jika menggunakan delay()

Contoh:
```cpp
delay(500);
```

Kelemahan:
* Arduino berhenti total
* Tidak bisa membaca input baru
* Serial tidak responsif
* LED tidak bisa langsung berhenti

Jika tekan '0', LED tetap berkedip sampai delay selesai

### Jika menggunakan millis()

Kelebihan:
* Non blocking
* Arduino tetap membaca input
* Respons cepat
* Bisa ganti mode kapan saja
* Sistem lebih real-time

**Kesimpulan:**

Gunakan → **millis()**
Karena sistem tetap berjalan dan responsif terhadap input serial.