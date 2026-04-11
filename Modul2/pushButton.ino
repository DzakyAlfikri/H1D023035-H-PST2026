#include <Arduino.h>  // Import library Arduino untuk mengakses fungsi yang diperlukan

// =================== PIN ==================
const int segmentPins[8] = {7, 6, 5 ,11, 10, 8, 9, 4};  // Array untuk pin seven segment (a,b,c,d,e,f,g,dp)
// a b c d e f g dp

const int btnUp = 3;  // Pin tombol untuk increment (naik)

// ================= DATA =================
// Array 2D untuk menyimpan pola display digit 0-F
// CC: 1 = ON, 0 = OFF (untuk Common Anode, akan di-negasi)
byte digitPattern[16][8] = {

{1,1,1,1,1,1,0,0}, //0
{0,1,1,0,0,0,0,0}, //1
{1,1,0,1,1,0,1,0}, //2
{1,1,1,1,0,0,1,0}, //3
{0,1,1,0,0,1,1,0}, //4
{1,0,1,1,0,1,1,0}, //5
{1,0,1,1,1,1,1,0}, //6
{1,1,1,0,0,0,0,0}, //7
{1,1,1,1,1,1,1,0}, //8
{1,1,1,1,0,1,1,0}, //9
{1,1,1,0,1,1,1,0}, //A
{0,0,1,1,1,1,1,0}, //b
{1,0,0,1,1,1,0,0}, //C
{0,1,1,1,1,0,1,0}, //d
{1,0,0,1,1,1,1,0}, //E
{1,0,0,0,1,1,1,0}  //F
};

int currentDigit = 0;  // Variabel untuk menyimpan digit yang sedang ditampilkan (0-15)

// state sebelumnya (untuk edge detection) - untuk mendeteksi saat tombol ditekan
bool lastUpState = HIGH;  // Menyimpan state tombol UP dari pembacaan sebelumnya

// ============= FUNCTION ============
// Fungsi menampilkan digit ke seven segment
void displayDigit(int num)  // Parameter num = digit yang ingin ditampilkan (0-15)
{
  // Loop untuk mengakses setiap segmen (a sampai dp)
  for(int i=0;i<8;i++)  // i dari 0 sampai 7 (8 segmen total)
  {
    // Kirim sinyal ke pin sesuai pola digit
    // !digitPattern[num][i] = negasi karena Common Anode (HIGH=mati, LOW=nyala)
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

// ================= SETUP ============
// Fungsi setup - Dijalankan sekali saat Arduino dinyalakan
void setup() {
  // Loop untuk mengatur semua pin seven segment sebagai OUTPUT
  for(int i=0;i<8;i++)  // Loop dari i=0 sampai i<8 (8 pin segmen)
  {
    pinMode(segmentPins[i], OUTPUT);  // Set pin sebagai OUTPUT untuk mengirim data
  }

  // Atur pin tombol UP sebagai INPUT dengan pull-up internal
  pinMode(btnUp, INPUT_PULLUP);  // INPUT_PULLUP = tidak perlu resistor eksternal

  // Tampilkan digit awal saat Arduino pertama kali dijalankan
  displayDigit(currentDigit);  // Tampilkan digit 0 (nilai awal currentDigit)
}

// ========== LOOP ============
// Fungsi loop - Berjalan terus menerus setelah setup
void loop() {
  // Baca state tombol UP dan simpan ke variable
  bool upState = digitalRead(btnUp);  // Baca pin 3 (HIGH jika tidak ditekan, LOW jika ditekan)

  // ======== UP (maju) =========
  // Deteksi perubahan state (edge detection): dari HIGH menjadi LOW = tombol baru ditekan
  if(lastUpState == HIGH && upState == LOW)  // Kondisi: state sebelumnya HIGH, sekarang LOW
  {
    currentDigit++;  // Increment nilai digit (naik 1)
    
    // Jika digit sudah mencapai 15 (F), kembali ke 0
    if(currentDigit > 15) currentDigit = 0;  // Counter cycling: F kembali ke 0
    
    displayDigit(currentDigit);  // Tampilkan digit baru ke seven segment
  }

  // Simpan state tombol saat ini untuk perbandingan loop berikutnya
  lastUpState = upState;  // State sekarang menjadi state sebelumnya untuk loop berikutnya

}