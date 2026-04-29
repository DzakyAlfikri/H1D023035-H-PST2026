#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan servo
const int potensioPin = A0;   // Menggunakan pin Analog A0
const int servoPin = 9;       // Menggunakan pin Digital 9 (PWM)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan data ADC dan sudut servo
int pos = 0; // Variabel untuk menyimpan hasil pemetaan sudut
int val = 0; // Variabel untuk menyimpan nilai mentah ADC

void setup() {

  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin); 

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600); // Baud rate standar 9600

}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(potensioPin); 

  // ===================== KONVERSI DATA =====================
  // Ubah nilai ADC (0-1023) menjadi sudut servo (0–180 derajat)
  pos = map(val,
             0,      // Nilai minimum ADC
             1023,   // Nilai maksimum ADC
             0,      // Sudut minimum servo
             180);   // Sudut maksimum servo

  // ===================== OUTPUT SERVO =====================
  // Gerakkan servo sesuai hasil mapping
  myservo.write(pos); 

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val); 

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos); 

  // ===================== STABILISASI =====================
  // Delay 15ms agar servo memiliki waktu untuk mencapai posisi
  delay(15); 
}