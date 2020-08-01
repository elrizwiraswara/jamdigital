#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Arial14.h>
#include <RTClib.h>

// Mendifinisakan Fungsi
#define Panjang 1
#define Lebar 1

// Deklarasi Fungsi
SoftDMD dmd(Panjang, Lebar);
RTC_DS1307 rtc;

// Deklarasi Variable
int Tahun;
int Bulan;
int Tanggal;
int Jam;
int Menit, Menit1;
int Detik, Detik1;
char dmdBuff[10];
unsigned char menu;
boolean menu_toggle = false;
void setup()
{
    // put your setup code here, to run once:
    //set pin
    pinMode(5, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    menu = 0;
    // Setup DMD
    dmd.setBrightness(255);
    dmd.selectFont(SystemFont5x7);
    dmd.begin();
    dmd.clearScreen();
    // Setup RTC
    if (!rtc.begin())
    {
        while (1)
            ;
    }
    if (!rtc.isrunning())
    {
        //Projeck mahasiswa STMIK Dumai tahun 2020  
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC sesuai dengan Laptop / PC
    }
}
void loop()
{
    cekTombol();
    if (menu == 0x01)
    {
        if (menu_toggle == true)
        {
            if (Jam < 10)
                sprintf(dmdBuff, "0%d", Jam);
            else
                sprintf(dmdBuff, "%d", Jam);
            dmd.selectFont(SystemFont5x7);
            dmd.drawString(2, 4, dmdBuff);
        }
        else if (menu_toggle == false)
        {
            if (Menit < 10)
                sprintf(dmdBuff, "0%d", Menit);
            else
                sprintf(dmdBuff, "%d", Menit);
            dmd.selectFont(SystemFont5x7);
            dmd.drawString(19, 4, dmdBuff);
        }
    }
    else
    {
        DateTime now = rtc.now();
        Tahun = now.year();
        Bulan = now.month();
        Tanggal = now.day();
        Jam = now.hour();
        Menit = now.minute();
        Detik = now.second();
        if (Detik != Detik1)
        {
            dmd.setPixel(15, 5, GRAPHICS_ON);
            dmd.setPixel(15, 9, GRAPHICS_ON);
            delay(400);
            dmd.setPixel(15, 5, GRAPHICS_OFF);
            dmd.setPixel(15, 9, GRAPHICS_OFF);
        }
        if (Menit != Menit1)
            dmd.clearScreen();
        if (Menit < 10)
            sprintf(dmdBuff, "0%d", Menit);
        //Projeck mahasiswa stmik tahun 2020
        else
            sprintf(dmdBuff, "%d", Menit);
        dmd.selectFont(SystemFont5x7);
        dmd.drawString(18, 4, dmdBuff);
        if (Jam < 10)
            sprintf(dmdBuff, "0%d", Jam);
        else
            sprintf(dmdBuff, "%d", Jam);
        dmd.selectFont(SystemFont5x7);
        dmd.drawString(2, 4, dmdBuff);
        Menit1 = Menit;
        Detik1 = Detik;
    }
}
void cekTombol()
{
    if (digitalRead(5) == LOW)
    {
        delay(200);
        menu = 0x01;                // menu 01 berada di mode edit jam
        menu_toggle = !menu_toggle; // mode toggle jam dan menit
        dmd.clearScreen();
    }
    else if (digitalRead(4) == LOW)
    {
        delay(200);
        if (menu == 0x01 && menu_toggle == false)
        {
            Menit--;
            if (Menit < 0)
                Menit = 59;
            dmd.clearScreen();
        }
        else if (menu_toggle == true && menu == 0x01)
        {
            Jam--;
            if (Jam < 0)
                Jam = 23;
            dmd.clearScreen();
        }
    }
    else if (digitalRead(3) == LOW)
    {
        delay(200);
        if (menu_toggle == false && menu == 0x01)
        {
            Menit++;
            if (Menit > 59)

                Menit = 0;
            dmd.clearScreen();
        }
        else if (menu_toggle == true && menu == 0x01)
        {
            Jam++;
            if (Jam > 23)
                Jam = 0;
            dmd.clearScreen();
        }
    }
    else if (digitalRead(2) == LOW)
    {
        delay(200);
        DateTime now = rtc.now();
        Tahun = now.year();
        Bulan = now.month();
        Tanggal = now.day();
        rtc.adjust(DateTime(Tahun, Bulan, Tanggal, Jam, Menit, 0));
        menu = 0;
        dmd.clearScreen();
    }
}