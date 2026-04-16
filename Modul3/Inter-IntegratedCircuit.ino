#include <Wire.h>              // Library komunikasi I2C
#include <LiquidCrystal_I2C.h> // Library LCD I2C
#include <Arduino.h>           // Library standar Arduino

// Ganti alamat jika perlu (0x27 / 0x20)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD I2C alamat 0x27 ukuran 16 kolom 2 baris

const int pinPot = A0; // Pin potensiometer terhubung ke analog A0

void setup() {
  Serial.begin(9600); // Memulai komunikasi serial dengan baudrate 9600
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Menyalakan lampu backlight LCD
}

void loop() {

  int nilai = analogRead(pinPot); // Membaca nilai analog dari potensiometer (0 - 1023)

  // Mapping ke bar (0-16)
  int panjangBar = map(nilai, 0, 1023, 0, 16); 
  // Mengubah nilai ADC menjadi panjang bar 0 sampai 16 karakter LCD

  // Tampilkan ke Serial
  Serial.print("Nilai ADC: "); // Menampilkan teks "Nilai ADC: " ke Serial Monitor
  Serial.println(nilai);       // Menampilkan nilai ADC dan pindah baris

  // Baris 1: nilai ADC
  lcd.setCursor(0, 0); // Mengatur kursor LCD kolom 0 baris 0
  lcd.print("ADC: ");  // Menampilkan teks "ADC: "
  lcd.print(nilai);    // Menampilkan nilai ADC
  lcd.print(" ");      // Menghapus sisa karakter sebelumnya (clear sisa)

  // Baris 2: bar
  lcd.setCursor(0, 1); // Mengatur kursor LCD kolom 0 baris 1

  for (int i = 0; i < 16; i++) { // Perulangan untuk membuat bar sepanjang 16 karakter
    if (i < panjangBar) {        // Jika indeks masih kurang dari panjang bar
      lcd.print((char)255);      // Tampilkan blok penuh (bar)
    }
    else {                       // Jika lebih dari panjang bar
      lcd.print(" ");            // Tampilkan spasi (kosong)
    }
  }

  delay(200); // Delay 200ms untuk refresh tampilan LCD
}