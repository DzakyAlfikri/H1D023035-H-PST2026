# Praktikum Percabangan 1.5.4

1. Pada kondisi apa program masuk ke blok if?
2. Pada kondisi apa program masuk ke blok else?
3. Apa fungsi dari perintah delay(timeDelay)?
4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati),
ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang →
mati dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

---

## Jawaban Pertanyaan

### 1. Pada kondisi apa program masuk ke blok if?
Program masuk ke blok `if` ketika **`timeDelay <= 100`**, yaitu ketika nilai delay sudah mencapai 100 milidetik atau lebih kecil. Kondisi ini menandakan bahwa LED sudah berkedip dengan kecepatan maksimal (paling cepat), sehingga program perlu melakukan tindakan berikutnya.

### 2. Pada kondisi apa program masuk ke blok else?
Program masuk ke blok `else` ketika **`timeDelay > 100`**, yaitu ketika nilai delay masih lebih besar dari 100 milidetik. Pada kondisi ini, program terus memercepat perkedipan LED dengan mengurangi nilai `timeDelay` sebesar 100 milidetik dalam setiap siklus perulangan.

### 3. Apa fungsi dari perintah delay(timeDelay)?
Perintah `delay(timeDelay)` berfungsi untuk **menunda atau membuat jeda eksekusi program selama periode waktu tertentu** (dalam satuan milidetik). Dalam konteks program ini:
- `delay(timeDelay)` pertama: membuat LED menyala selama periode `timeDelay` milidetik
- `delay(timeDelay)` kedua: membuat LED mati selama periode `timeDelay` milidetik
- Semakin kecil nilai `timeDelay`, semakin cepat LED berkedip

---

## Program yang Dimodifikasi

### Alur Program Baru:
**Cepat (100ms) → Sedang (550ms) → Mati (Berhenti)**

```cpp
const int ledPin = 11;              // Deklarasi pin LED pada pin digital 11
int timeDelay = 100;                // Inisialisasi delay dengan nilai cepat (100ms), mulai dari cepat bukan lambat
int delayState = 0;                 // Variabel untuk melacak tahap kecepatan: 0=cepat, 1=sedang, 2=mati/berhenti

void setup() {
    pinMode(ledPin, OUTPUT);        // Atur pin LED sebagai OUTPUT (keluaran)
}

void loop() {
    // Nyalakan LED
    digitalWrite(ledPin, HIGH);     // Kirim sinyal HIGH ke pin LED untuk menyalakan LED
    delay(timeDelay);               // Tunggu selama periode timeDelay (dalam milidetik)

    // Matikan LED
    digitalWrite(ledPin, LOW);      // Kirim sinyal LOW ke pin LED untuk mematikan LED
    delay(timeDelay);               // Tunggu selama periode timeDelay (dalam milidetik)

    // Ubah kondisi kecepatan berdasarkan tahap perjalanan
    if (delayState == 0) {
        // Tahap 1: Cepat (100ms) - lanjut ke tahap sedang
        delayState = 1;             // Pindah ke tahap sedang
        timeDelay = 550;            // Ubah delay ke 550ms (kecepatan sedang)
    }
    else if (delayState == 1) {
        // Tahap 2: Sedang (550ms) - lanjut ke tahap mati/berhenti
        delayState = 2;             // Pindah ke tahap mati/berhenti
        delay(3000);                // Berikan jeda 3 detik sebelum berhenti
    }
    else if (delayState == 2) {
        // Tahap 3: Mati/Berhenti - LED berhenti berkedip
        // Loop tidak akan melakukan apapun, LED tetap dalam kondisi terakhir (mati)
        // Jika ingin LED tetap hidup atau mode lain, bisa dimodifikasi di sini
    }
}
```

---

## Penjelasan Detail Program yang Dimodifikasi

### Deklarasi Variabel
| Baris | Kode | Penjelasan |
|-------|------|-----------|
| 1 | `const int ledPin = 11;` | Mendeklarasikan konstanta `ledPin` dengan nilai 11 sebagai nomor pin digital tempat LED terhubung |
| 2 | `int timeDelay = 100;` | Mendeklarasikan variabel `timeDelay` dengan nilai awal 100 milidetik (tahap cepat dimulai dari sini) |
| 3 | `int delayState = 0;` | Mendeklarasikan variabel `delayState` untuk melacak tahap kecepatan saat ini |

### Fungsi setup()
| Baris | Kode | Penjelasan |
|-------|------|-----------|
| 5 | `void setup() {` | Mendeklarasikan fungsi `setup()` yang dijalankan sekali saat Arduino dinyalakan |
| 6 | `pinMode(ledPin, OUTPUT);` | Mengatur pin 11 sebagai OUTPUT agar dapat mengirimkan sinyal ke LED |
| 7 | `}` | Menutup fungsi `setup()` |

### Fungsi loop()
| Baris | Kode | Penjelasan |
|-------|------|-----------|
| 9 | `void loop() {` | Mendeklarasikan fungsi `loop()` yang berjalan berulang kali secara terus-menerus |
| 11 | `digitalWrite(ledPin, HIGH);` | Mengirimkan sinyal HIGH ke pin LED, menyebabkan LED menyala |
| 12 | `delay(timeDelay);` | Menunggu selama `timeDelay` milidetik sambil LED tetap menyala |
| 14 | `digitalWrite(ledPin, LOW);` | Mengirimkan sinyal LOW ke pin LED, menyebabkan LED mati |
| 15 | `delay(timeDelay);` | Menunggu selama `timeDelay` milidetik sambil LED tetap mati |
| 18 | `if (delayState == 0) {` | Memeriksa apakah program berada di tahap pertama (cepat) |
| 19 | `delayState = 1;` | Mengubah status ke tahap kedua (sedang) |
| 20 | `timeDelay = 550;` | Mengubah delay menjadi 550ms untuk tahap sedang (lebih lambat dari cepat) |
| 21 | `}` | Menutup blok if pertama |
| 22 | `else if (delayState == 1) {` | Memeriksa apakah program berada di tahap kedua (sedang) |
| 23 | `delayState = 2;` | Mengubah status ke tahap ketiga (mati/berhenti) |
| 24 | `delay(3000);` | Memberikan jeda 3 detik sebelum memasuki tahap berhenti |
| 25 | `}` | Menutup blok else if pertama |
| 26 | `else if (delayState == 2) {` | Memeriksa apakah program berada di tahap ketiga (mati/berhenti) |
| 28 | `// Komentar penjelasan` | Pada tahap ini, loop tidak melakukan apa-apa dan LED tetap dalam kondisi terakhir |
| 30 | `}` | Menutup blok else if kedua |
| 31 | `}` | Menutup fungsi `loop()` |