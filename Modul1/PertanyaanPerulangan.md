## Pertanyaan Praktikum Perulangan 1.6.4

1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan!  
2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan! 
3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri! 
4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian
dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

---

## 1. Rangkaian Schematic 5 LED Running

```
         +5V
          |
      [R1 470Ω]   [R2 470Ω]   [R3 470Ω]   [R4 470Ω]   [R5 470Ω]
         |            |            |            |            |
        LED1          LED2          LED3         LED4         LED5
         |            |            |            |            |
         |            |            |            |            |
        Pin2         Pin3          Pin4         Pin5         Pin6
         |            |            |            |            |
    -----+------------+------------+------------+------------+-----
         |            |            |            |            |
        GND          GND          GND          GND          GND

Keterangan:
- LED1 hingga LED5 terhubung ke Pin 2 hingga Pin 6 (GPIO Arduino)
- Setiap LED memiliki resistor pembatas arus 470 Ohm
- Semua rangkaian terhubung ke GND (ground/mas)
- Supply tegangan: +5V (dari Arduino)
```

---

## 2. Penjelasan Program: Efek LED Berjalan Kiri ke Kanan

### Analisis Alur Program:

Pada program perulangan.ino, efek LED berjalan dari kiri ke kanan terjadi pada loop pertama:

```cpp
for (int ledPin = 2; ledPin < 8; ledPin++) {
    digitalWrite(ledPin, HIGH);     // Nyalakan LED
    delay(timer);                   // Tunggu 100ms
    digitalWrite(ledPin, LOW);      // Matikan LED
}
```

**Penjelasan:**
- Loop dimulai dari Pin 2 (LED paling kiri)
- Variabel `ledPin` bertambah 1 setiap iterasi: 2 → 3 → 4 → 5 → 6 → 7
- Setiap Pin: Nyalakan selama 100ms, lalu matikan
- Hasilnya: Efek LED berjalan dari kiri ke kanan dengan kecepatan timer

**Visualisasi Pergerakan:**
```
Iterasi 1: [LED1 ON] [----] [----] [----] [----]
Iterasi 2: [----] [LED2 ON] [----] [----] [----]
Iterasi 3: [----] [----] [LED3 ON] [----] [----]
Iterasi 4: [----] [----] [----] [LED4 ON] [----]
Iterasi 5: [----] [----] [----] [----] [LED5 ON]
```

---

## 3. Penjelasan Program: Efek LED Kembali Kanan ke Kiri

Efek LED kembali dari kanan ke kiri terjadi pada nested loop (loop kedua):

```cpp
for (int ledPin = 7; ledPin >= 2; ledPin--) {
    digitalWrite(ledPin, HIGH);     // Nyalakan LED
    delay(timer);                   // Tunggu 100ms
    digitalWrite(ledPin, LOW);      // Matikan LED
}
```

**Penjelasan:**
- Loop dimulai dari Pin 7 (LED paling kanan)
- Variabel `ledPin` berkurang 1 setiap iterasi: 7 → 6 → 5 → 4 → 3 → 2
- Kondisi `ledPin >= 2` memastikan loop berhenti saat mencapai Pin 2
- Setiap Pin: Nyalakan selama 100ms, lalu matikan
- Hasilnya: Efek LED berjalan balik dari kanan ke kiri

**Visualisasi Pergerakan:**
```
Iterasi 1: [----] [----] [----] [----] [LED5 ON]
Iterasi 2: [----] [----] [----] [LED4 ON] [----]
Iterasi 3: [----] [----] [LED3 ON] [----] [----]
Iterasi 4: [----] [LED2 ON] [----] [----] [----]
Iterasi 5: [LED1 ON] [----] [----] [----] [----]
```

---

## 4. Program LED Bergantian: 3 LED Kanan vs 3 LED Kiri

Program baru dengan efek LED bergantian 3 LED kanan dan 3 LED kiri:

### File: `perulangan_bergantian.ino`

```cpp
// Inisialisasi delay untuk kontrol kecepatan LED
int timer = 100;

// Setup: Inisialisasi semua pin LED sebagai OUTPUT
void setup() {
    // Loop untuk mengatur Pin 2 hingga Pin 6 sebagai OUTPUT (3 LED total)
    for (int ledPin = 2; ledPin < 8; ledPin++) {
        pinMode(ledPin, OUTPUT);
    }
}

// Loop utama: Program berjalan terus-menerus
void loop() {
    // ===== FASE 1: Nyalakan 3 LED Kanan (Pin 4, 5, 6) =====
    // LED di sisi kanan menyala sementara LED kiri mati
    
    // Nyalakan LED kanan Pin 4
    digitalWrite(4, HIGH);
    // Nyalakan LED kanan Pin 5
    digitalWrite(5, HIGH);
    // Nyalakan LED kanan Pin 6
    digitalWrite(6, HIGH);
    
    // Tunggu selama timer sebelum perubahan status
    delay(timer);
    
    // Matikan semua LED sebelum transisi ke fase berikutnya
    for (int ledPin = 2; ledPin < 8; ledPin++) {
        digitalWrite(ledPin, LOW);
    }
    
    // Tunggu sebentar untuk transisi (delay antara fase)
    delay(timer / 2);
    
    // ===== FASE 2: Nyalakan 3 LED Kiri (Pin 2, 3, 4) =====
    // LED di sisi kiri menyala sementara LED kanan mati
    
    // Nyalakan LED kiri Pin 2
    digitalWrite(2, HIGH);
    // Nyalakan LED kiri Pin 3
    digitalWrite(3, HIGH);
    // Nyalakan LED kiri Pin 4
    digitalWrite(4, HIGH);
    
    // Tunggu selama timer sebelum perubahan status
    delay(timer);
    
    // Matikan semua LED sebelum loop berikutnya
    for (int ledPin = 2; ledPin < 8; ledPin++) {
        digitalWrite(ledPin, LOW);
    }
    
    // Tunggu sebentar untuk transisi (delay antara fase)
    delay(timer / 2);
}
```

---

## Penjelasan Detail Setiap Baris Kode Program

| Baris | Kode | Penjelasan |
|-------|------|-----------|
| 1 | `int timer = 100;` | Deklarasi variabel `timer` dengan nilai 100ms untuk mengontrol kecepatan menyala/matinynya LED |
| 2-4 | `void setup() { ... }` | Fungsi inisialisasi yang dijalankan sekali saat Arduino dinyalakan |
| 5 | `for (int ledPin = 2; ledPin < 8; ledPin++)` | Loop untuk mengatur Pin 2 hingga Pin 7 (6 pin total) sebagai OUTPUT |
| 6 | `pinMode(ledPin, OUTPUT);` | Mengatur setiap pin sebagai OUTPUT agar dapat mengatur tegangan HIGH/LOW |
| 9-11 | `void loop() { ... }` | Fungsi utama yang terus berulang selama Arduino menyala |
| 13 | `digitalWrite(4, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 4 untuk menyalakan LED kanan pertama |
| 15 | `digitalWrite(5, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 5 untuk menyalakan LED kanan kedua |
| 17 | `digitalWrite(6, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 6 untuk menyalakan LED kanan ketiga |
| 20 | `delay(timer);` | Memberi jeda sebesar timer (100ms) agar LED menyala selama waktu tersebut |
| 23-25 | `for (int ledPin = 2; ledPin < 8; ledPin++) { digitalWrite(ledPin, LOW); }` | Loop untuk mematikan semua LED (Pin 2-7) dengan mengirim sinyal LOW (0V) |
| 28 | `delay(timer / 2);` | Memberi delay setengah dari timer (50ms) sebagai transisi antar fase untuk efek yang lebih smooth |
| 31 | `digitalWrite(2, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 2 untuk menyalakan LED kiri pertama |
| 33 | `digitalWrite(3, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 3 untuk menyalakan LED kiri kedua |
| 35 | `digitalWrite(4, HIGH);` | Mengirim sinyal HIGH (5V) ke Pin 4 untuk menyalakan LED kiri ketiga (Pin tengah yang digunakan untuk kedua kelompok) |
| 38 | `delay(timer);` | Memberi jeda sebesar timer (100ms) agar LED kiri menyala selama waktu tersebut |
| 41-43 | `for (int ledPin = 2; ledPin < 8; ledPin++) { digitalWrite(ledPin, LOW); }` | Loop untuk mematikan semua LED sebelum loop berikutnya |
| 46 | `delay(timer / 2);` | Memberi delay sebagai transisi sebelum LED kanan menyala kembali |
