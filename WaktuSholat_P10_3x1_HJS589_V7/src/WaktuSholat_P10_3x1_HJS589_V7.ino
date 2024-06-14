/*************************************************************************************
 Waktu Sholat P10   - Program jam petunjuk waktu Sholat otomatis yang dilekapi dengan
 Copyright (C) Des 2017 MFH Robotic. (info https://www.facebook.com/MFH.Robotic/)
 
 Fitur :
 1. Perhitungan waktu sholat otomatis berdasarkan data geografis lokasi 
    (Latitude, Longitude, Altitude, Time Zone dan Prameter pengaman/
    Ihtiyati)
 2. Fasilitas remote menggunakan aplikasi yang berjalan pada HP Android.
 3. Fasilitas running text informasi yang berjumlah 3 x 150 karakter.
    Running text ini cukup besar untuk di isi dengan hadis atau informasi lainnya
    melalui fasilitas remote Aplikasi Android.
 4. Perhitungan Hari dan tanggal Hijriah secara otomatis dan perubahan tanggalnya
    sesuai sesuai standar Hijriah yaitu setelah azzan magrib
 5. Fasilitas nama Masjid dan jenis masjid nya:
        a. Masjid
        b. Musholla
        c. Surau
        d. Langgar
 6. Pengingat waktu sholat mulai dari Azzan, menunggu iqomah dan mulai Sholat
    termasuk pengingat sholat Jumat jika parameter jenis nya di set sebagai Masjid.
 7. Program ini sangat baik dipakai sebagai sarana belajar programing Arduino karena 
    menggunakan banyak librari dan teknik pemrogramn yang asik seperti:
       - Library yang digunakan adalah:
              #include <SPI.h>            --> komunikasi ke modul P10
              #include <DMD3.h>           --> library untuk modul P10 
              #include <Wire.h>           --> komunikasi ke modul RTC DS3231
              #include <DS3231.h>         --> library modul RTC DS 3231
              #include <Timer.h>          --> library Timer untuk mengatur timing tampilan
              #include <EEPROM.h>         --> library untuk penggunaan EEPROM sebagai sarana penyimpan parameter
              #include <avr/pgmspace.h>   --> library untuk penggunaan PROGMEM agar utilisasi SRAM bisa optimal
 8. Program dipecah menjadi 4 File sehingga maintenance bisa lebih mudah dan aman dan tidak mengganggu bagian 
    lain.
    
  Created by Wardi Utari 
  30 Des 2017

  ---

  Progarm ini TIDAK ROYALTY FREE, program ini FREE jika dipergunakan untuk
    1. Jika dipergunakan di sekolah/lembaga latihan/perorangan untuk pendidikan atau proses belajar
    2. Dipergunakan dalam proyek pembuatan Jam pengingat waktu sholat yang disumbangkan ke masjid, musholla dll.

  Jika program ini dipergunakan secara komersial baik keseluruhan atau sebagian, anda diwajibkan 
  membayar ROYALTY FEE sebesar 2.5%. Royalty tersebut agar dibayarkan ke Masjid terdekat dengan lokasi 
  anda, dalam bentuk Sedekah yang diniatkan untuk semua orang yang telah turut urun rembug dalam membuat dan 
  menyempurnakan program ini.

  Semoga Bermanfaat
  Salam Mujahid 212.

**************************************************************************************/
#include <SPI.h>
#include <HJS589.h>
#include <font/SystemFont5x7.h>
#include <font1/System6x7.h>
#include <font1/Angka6x14.h>
#include <font/Bigest.h>
#include <Arab/arab16x10.h>
#include <font1/Arial_black_16_2.h>


#include <DS3231.h>
#include <EEPROM.h>
#include <pgmspace.h>
#include <Wire.h>


#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

WiFiServer server(313);
WiFiClient client;

//#define BUZZ    3 //  gpio 3/RX
const int BUZZ = 3; //  gpio 3/RX
uint8_t state=0;

    
// Object Declarations
HJS589 Disp(3,1);

RTClib          RTC;
DS3231          Clock;

//Structure of Variable 
typedef struct  // loaded to EEPROM
  {
    uint8_t state;   // 1 byte  add 0
    float   L_LA;    // 4 byte  add 1
    float   L_LO;    // 4 byte  add 5
    float   L_AL;    // 4 byte  add 9
    float   L_TZ;    // 4 byte  add 13
    uint8_t MT;      // 1 byte  add 17  // value 1-masjid  2-mushollah 3-surau 4-langgar 
    uint8_t BL;      // 1 byte  add 18
    uint8_t IH;      // 1 byte  add 19
    uint8_t SO;      // 1 byte  add 20
    uint8_t JM;      // 1 byte  add 21
    uint8_t I1;      // 1 byte  add 22
    uint8_t I4;      // 1 byte  add 23
    uint8_t I5;      // 1 byte  add 24
    uint8_t I6;      // 1 byte  add 25
    uint8_t I7;      // 1 byte  add 26
    uint8_t BZ;      // 1 byte  add 27
    uint8_t SI;      // 1 byte  add 28
    uint8_t ST;      // 1 byte  add 29
    uint8_t SU;      // 1 byte  add 30
    int8_t  CH;      // 1 byte  add 31
  } struct_param;

typedef struct  
  { 
    uint8_t   hD;
    uint8_t   hM;
    uint16_t  hY;
  } hijir_date;

   
// Variable by Structure     
struct_param    Prm;
hijir_date      nowH;   

// Time Variable
DateTime        now;
float           floatnow   = 0;
uint8_t         daynow     = 0;
uint8_t         ty_puasa   = 0;
uint8_t         hd_puasa   = 0; 
int8_t          SholatNow  = -1;
boolean         jumat      = false;
boolean         azzan      = false;
uint8_t         reset_x    = 0;   

//Other Variable
float sholatT[8]  = {0,0,0,0,0,0,0,0};
uint8_t Iqomah[8] = {0,0,0,0,0,0,0,0};

//Blue tooth Pram Receive
char        CH_Prm[255];
int         DWidth  = Disp.width();
int         DHeight = Disp.height();
boolean     DoSwap;
int         RunSel    = 1; //
int         RunFinish = 0 ;





//=======================================
//===SETUP=============================== 
//=======================================
void setup()
  {
   
  //init comunications 
  //EEPROM.begin(1024);
  EEPROM.begin(1024);
  Wire.begin();
  Serial.begin(115200);
  pinMode(BUZZ,OUTPUT);//buzer 
  
  const char *ssid = "1000Masjid";  
  const char *pw = "seribumasjid"; 
  //IPAddress ip(192, 168, 50, 110);
  IPAddress ip(11, 11, 11, 11); 
  IPAddress netmask(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask); 
  WiFi.softAP(ssid, pw);
  server.begin(); // TCP server
  Serial.print("\n\n\n");
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.softAPIP());
  Serial.println(" port 313' to connect");
  Serial.print("\n\n");
  delay(131);
  //delay(500);  
    /*
    //init comunications 
    EEPROM.begin(1024);
    Wire.begin();
    Serial.begin(9600);
    pinMode(3,OUTPUT);//buzer 
    
const char *ssid = "tatang";  
const char *pw = "pak tatang"; 
IPAddress ip(11, 11, 11, 11); 
IPAddress netmask(255, 255, 255, 0);
//(TCP) -> 11.11.11.11:313
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask); 
  WiFi.softAP(ssid, pw);
 server.begin(); // TCP server          
delay(131);
*/
    // Get Saved Parameter from EEPROM   
    updateTime();
    GetPrm();
    SendPrm();

    //init P10 Led Disp & Salam
    Disp_init();

   
  }


//=======================================
//===MAIN LOOP Function =================   
//=======================================
void loop()
  { 
    // Reset & Init Display State
    updateTime();   //every time
    check_azzan();  //check Sholah Time for Azzan
    DoSwap  = false ;
    fType(1);  
    Disp.clear();
    
    
    // Timer Function every 10 Minutes
    // Up All function with Timer in this fuction
    Timer_Minute(1);
    

    // =========================================
    // List of Display Component Block =========
    // =========================================

    
    
    fType(3);
    dwMrq(drawMasjidName(),50,3,1);                             // addr: 1 show Masjid Name
    anim_DT(2);                                                 // addr: 2 show date time
    fType(0);
    dwMrq(drawDayDate()   ,50,1,3);                             // addr: 3 show Hijriah date
    //dwMrq(msgPuasa(hd_puasa,ty_puasa),35,0,4);                  
    drawSholat(4);                                              // addr: 4 show sholat time
    fType(0);
    dwMrq(drawSholat_S1(9) ,50,2,5);                            // addr: 5 show Remander Puasajadwal sholat running text
    dwMrq(drawInfo(130)    ,50,1,6);                             // addr: 6 show Info 1
    anim_DT1(7);                                                 // addr: 7 show date time    
    dwMrq(drawInfo(280)   ,50,1,8);                             // addr: 8 show Info 2
    drawSholat(9);                                              // addr: 9 show sholat time
    dwMrq(drawInfo(430)   ,50,1,10);                            // addr: 10 show Info 3


    drawAzzan(100);                                             // addr: 100 show Azzan
    drawIqomah(101);                                            // addr: 101 show Iqomah
    dwMrq(drawInfo(580),50,0,102); //Message Sholat biasa       // addr: 202 show Message Sholah
    dwMrq(drawInfo(730),50,0,103); //Message Sholat jumat       // addr: 203 show Message Jum'at

    
    blinkBlock(104);                                            // addr: 104 show Blink  Sholat    

    
      
    // =========================================
    // Display Control Block ===================
    // =========================================
    if(RunFinish==1) {RunSel = 2; RunFinish =0;}                      //after anim 1 set anim 2
    if(RunFinish==2) {RunSel = 3; RunFinish =0;}                      //after anim 2 set anim 3
    if(RunFinish==3) {RunSel = 4; RunFinish =0;}                      //after anim 3 set anim 5 or anim 4 if puasa
         //{
          //if (ty_puasa!=0)  {RunSel = 4; RunFinish =0;}
          //else {RunSel = 5; RunFinish =0;}
         //}
    if(RunFinish==4)  {RunSel = 5;  RunFinish =0;}                      //after anim 4 set anim 5
    if(RunFinish==5)  {RunSel = 6;  RunFinish =0;}                      //after anim 5 set anim 6
    if(RunFinish==6)  {RunSel = 7;  RunFinish =0;}                      //after anim 6 set anim 7
    if(RunFinish==7)  {RunSel = 8;  RunFinish =0;}                      //after anim 7 set anim 8
    if(RunFinish==8)  {RunSel = 9;  RunFinish =0;}                      //after anim 8 set anim 9
    if(RunFinish==9)  {RunSel = 10; RunFinish =0;}                      //after anim 9 set anim 10
    if(RunFinish==10) {RunSel = 1; RunFinish =0;}                      //after anim 10 set anim 1
    //if(RunFinish==11) {RunSel = 1;  RunFinish =0;}
    
    
    if(RunFinish==100 and jumat )     {RunSel = 103; RunFinish = 0; reset_x = 1;}  //after Azzan Jumat (anim 100)
    else if(RunFinish==100)           {RunSel = 101; RunFinish =0;}               //after Azzan Sholah (Iqomah)
        
    if(RunFinish==101) {RunSel = 102; RunFinish =0; reset_x=1;}       //after Iqomah(anim 101) set Message Sholah (anim 102)   
    if(RunFinish==102) {RunSel = 104; RunFinish =0;}                  //after Message Sholah (anim 102) set Blink Sholah(anim 104) 
    if(RunFinish==103) {RunSel = 104; RunFinish =0;}                  //after Messagw Jum'at (anim 103) set Blink Sholah(anim 104)
    if(RunFinish==104) {RunSel = 1; RunFinish =0;}                    //after Blink Sholah back to anim 1 

    
    
    // =========================================
    // Swap Display if Change===================
    // =========================================
    if(DoSwap){Disp.swapBuffers();} // Swap Buffer if Change
//if(state ==1 and Prm.BZ == 1)
if(state ==1 )
      {Buzzer(1);}
    else 
      {Buzzer(0);}

    serialEvent();
  }


// =========================================
// DMD3 P10 utility Function================
// =========================================
void ICACHE_RAM_ATTR wiah()
  { 
  Disp.refresh();
  timer0_write(ESP.getCycleCount() + 40000);
  }


void Disp_init() 
  { Disp.setDoubleBuffer(true);
    Disp.start();
    timer0_attachInterrupt(wiah);
    timer0_write(ESP.getCycleCount() + 40000);
    //setBrightness(int(Prm.BL));
    fType(1);  
    Disp.clear();
    Disp.swapBuffers();
    }

void setBrightness(int bright)
  { Disp.setBrightness(bright);}

 void scan()
{
  Disp.refresh();
}
 
// =========================================
// Time Calculation Block===================
// =========================================
void updateTime()
  { now = RTC.now();
    floatnow = (float)now.hour() + (float)now.minute()/60 + (float)now.second()/3600;
    daynow   = Clock.getDoW();    // load day Number
  }
  
void Timer_Minute(int repeat_time) //load every  1 minute
  { 
    //static uint16_t   lsRn;
    //uint16_t          Tmr = millis();
    unsigned long   lsRn = 0;
    unsigned long   Tmr = millis();
    if((Tmr-lsRn)>=(repeat_time*60000))
          {
        lsRn =Tmr;
        update_All_data();
      }
  }

void update_All_data()
  {
  uint8_t   date_cor = 0;
  updateTime();
  sholatCal();                                                // load Sholah Time
  //check_puasa();                                            // check jadwal Puasa Besok
  if(floatnow>sholatT[6]) {date_cor = 1;}                     // load Hijr Date + corection next day after Mhagrib 
  nowH = toHijri(now.year(),now.month(),now.day(),date_cor);  // load Hijir Date
  
  if ((floatnow > (float)19) or (floatnow < (float)4) )    {setBrightness(15);}
      else                                                   {setBrightness(EEPROM.read(18));}
      
                          //   {setBrightness(int(Prm.BL));}  
      
   }
  
    
void check_azzan()
  { //Check Waktu Sholat
    SholatNow  = -1;
    for(int i=0; i <=7; i++)
      {
        if (i!=0 and i!=2 and i!=3)  // bukan imsak, terbit dan bukan dhuha
          {
            if(floatnow >= sholatT[i])
              {
                SholatNow = i;
                if(!azzan and (floatnow > sholatT[i]) and (floatnow < (sholatT[i]+0.03))) 
                  { 
                    if(daynow ==6 and SholatNow ==4 and Prm.MT==1) {jumat=true;}
                    azzan =true;
                    RunSel = 100;
                  }  
              }
          }
      }
  }
