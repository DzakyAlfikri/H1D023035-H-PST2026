const int PIN_LED = 12; 
// Menentukan pin LED pada Arduino (pin digital 12)

void setup() {
  Serial.begin(9600); 
  // Memulai komunikasi serial dengan baudrate 9600 bps

  Serial.println("Ketik '1' untuk menyalakan LED, '0' untuk mematikan LED"); 
  // Menampilkan instruksi ke Serial Monitor

  pinMode(PIN_LED, OUTPUT); 
  // Mengatur pin LED sebagai output
}

void loop() {

  if (Serial.available() > 0) { 
  // Mengecek apakah ada data yang dikirim dari Serial Monitor

    char data = Serial.read(); 
    // Membaca 1 karakter data dari Serial Monitor

    if (data == '1') { 
    // Jika user mengetik angka 1

      digitalWrite(PIN_LED, HIGH); 
      // Menyalakan LED

      Serial.println("LED ON"); 
      // Menampilkan pesan LED ON ke Serial Monitor
    }

    else if (data == '0') { 
    // Jika user mengetik angka 0

      digitalWrite(PIN_LED, LOW); 
      // Mematikan LED

      Serial.println("LED OFF"); 
      // Menampilkan pesan LED OFF ke Serial Monitor
    }

    else if (data != '\n' && data != '\r') { 
    // Jika input bukan 1, 0, atau ENTER

      Serial.println("Perintah tidak dikenal"); 
      // Menampilkan pesan error jika perintah salah
    }
  }
}