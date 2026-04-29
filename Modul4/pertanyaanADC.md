# Pertanyaan ADC

## 1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?

Perintah `analogRead(potensioPin)` digunakan untuk membaca nilai tegangan analog dari potensiometer yang terhubung pada pin A0. Nilai tegangan tersebut dikonversi oleh ADC Arduino menjadi data digital dengan rentang 0 sampai 1023. Nilai inilah yang kemudian digunakan sebagai input untuk menentukan posisi sudut servo.

---

## 2. Mengapa diperlukan fungsi map() dalam program tersebut?

Fungsi `map()` digunakan untuk mengubah rentang nilai ADC dari potensiometer (0–1023) menjadi rentang sudut servo (0–180 derajat). Karena servo hanya menerima nilai sudut, maka data analog dari potensiometer harus dikonversi terlebih dahulu agar sesuai dengan kebutuhan servo.

Tanpa `map()`, nilai 0–1023 tidak bisa langsung digunakan oleh servo karena servo hanya memahami rentang sudut 0–180.

---

## 3. Modifikasi Program (Servo hanya bergerak 30°–150°)

Ubah bagian `map()` menjadi:

```cpp
pos = map(val,
          0,      // Nilai minimum ADC
          1023,   // Nilai maksimum ADC
          30,     // Sudut minimum servo
          150);   // Sudut maksimum servo
```

### Program Lengkap:

```cpp
#include <Servo.h>

Servo myservo;

const int potensioPin = A0;
const int servoPin = 9;

int pos = 0;
int val = 0;

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // Baca nilai potensiometer
  val = analogRead(potensioPin);
  
  // Mapping 0–1023 menjadi 30–150 derajat
  pos = map(val, 0, 1023, 30, 150);
  
  // Gerakkan servo
  myservo.write(pos);
  
  // Monitoring serial
  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);
  
  delay(15);
}
```

---
