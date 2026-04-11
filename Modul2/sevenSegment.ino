#include <Arduino.h>  // Import library Arduino untuk mengakses fungsi digitalWrite, pinMode, delay, dll

//7-Segment Display (Efficient Version)
//Display 0 - 9 and A - F

// Pin mapping segment - Array yang menyimpan nomor pin Arduino untuk setiap segmen
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};  // Pin untuk a, b, c, d, e, f, g, dp
//a b c d e f g dp

// Segment pattern for 0-F - Array 2D untuk menyimpan pola setiap digit
// urutan segmen: a b c d e f g dp (1 = ON, 0 = OFF)
byte digitPattern[16][8] = {  // 16 baris (0-F), 8 kolom (untuk setiap segmen)

{1,1,1,1,1,1,0,0}, //0  -> segmen a,b,c,d,e,f hidup; g,dp mati
{0,1,1,0,0,0,0,0}, //1  -> hanya segmen b,c hidup
{1,1,0,1,1,0,1,0}, //2  -> segmen a,b,g,e,d hidup
{1,1,1,1,0,0,1,0}, //3  -> segmen a,b,c,d,g hidup
{0,1,1,0,0,1,1,0}, //4  -> segmen b,c,f,g hidup
{1,0,1,1,0,1,1,0}, //5  -> segmen a,c,d,f,g hidup
{1,0,1,1,1,1,1,0}, //6  -> segmen a,c,d,e,f,g hidup
{1,1,1,0,0,0,0,0}, //7  -> segmen a,b,c hidup
{1,1,1,1,1,1,1,0}, //8  -> semua segmen kecuali dp hidup
{1,1,1,1,0,1,1,0}, //9  -> segmen a,b,c,d,f,g hidup
{1,1,1,0,1,1,1,0}, //A  -> segmen a,b,c,e,f,g hidup
{0,0,1,1,1,1,1,0}, //b  -> segmen c,d,e,f,g hidup
{1,0,0,1,1,1,0,0}, //C  -> segmen a,d,e,f hidup
{0,1,1,1,1,0,1,0}, //d  -> segmen b,c,d,e,g hidup
{1,0,0,1,1,1,1,0}, //E  -> segmen a,d,e,f,g hidup
{1,0,0,0,1,1,1,0}  //F  -> segmen a,e,f,g hidup
};  // Tutup array digitPattern

// Fungsi menampilkan digit - Fungsi untuk menampilkan digit ke seven segment
// Parameter num = digit yang ingin ditampilkan (0-15)
void displayDigit(int num)
{
  // Loop untuk mengakses setiap segmen dari a sampai dp
  for(int i=0;i<8;i++)  // i dimulai dari 0, loop sampai i < 8 (8 segmen total)
  {
    // Kirim signal ke pin sesuai pola digit yang dipilih
    // !digitPattern[num][i] = negasi untuk logic Common Anode
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

// Fungsi setup - Dijalankan sekali saat Arduino dinyalakan
void setup()
{
  // Loop untuk mengatur pin segmen sebagai OUTPUT
  for(int i=0;i<8;i++)  // Loop untuk 8 pin segmen
  {
    // Atur pin sebagai OUTPUT agar bisa mengirim sinyal
    pinMode(segmentPins[i], OUTPUT);
  }
}

// Fungsi loop - Berjalan terus menerus setelah setup
void loop()
{
  // Loop untuk menampilkan digit 0 sampai F (0-15)
  for(int i=0;i<16;i++)  // i dimulai dari 0, loop sampai i < 16 (16 digit: 0-F)
  {
    displayDigit(i);  // Tampilkan digit ke-i (0,1,2,...,F)
    delay(1000);      // Tunggu 1000 milidetik (1 detik) sebelum digit berikutnya
  }
  // Setelah menampilkan F, loop kembali ke awal (i=0) dan tampilkan 0 lagi
}