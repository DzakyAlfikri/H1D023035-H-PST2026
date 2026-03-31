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
