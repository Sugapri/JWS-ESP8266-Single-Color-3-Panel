 // =========================================
// Drawing Content Block====================    
// =========================================

void drawAzzan(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return;
    uint8_t           ct_limit =480;  //harus angka genap
    static uint8_t    ct;
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();

    if((Tmr-lsRn) > 500 and ct <= ct_limit)
      {
        lsRn = Tmr;
        if((ct%2) == 0)
          { Disp.drawRect(13,2,82,13);
            fType(0);
            dwCtr(0,0,"ADZAN");
            fType(0);
            if(jumat) {dwCtr(0,8,sholatN(8));}
            else      {dwCtr(0,8,sholatN(SholatNow));}
            if (ct <= 8) 
            {  //ct <= 4 BUNYI BEEP 3 x        
            Buzzer (1);}
       }
    else 
       {
      Buzzer(0);
      }
        DoSwap = true; 
        ct++;
      }
    if ((Tmr-lsRn)>2000 and (ct > ct_limit))
      {dwDone(DrawAdd);
       ct = 0;
      Buzzer(1);
  }
  }

void drawIqomah(int DrawAdd)  // Countdown Iqomah (9 menit)
  {  
    // check RunSelector
    if(!dwDo(DrawAdd)) return;

    static uint32_t   lsRn;
    uint32_t          Tmr = millis();
    static int        ct;
    int               mnt, scd,cn_l ;
    char              locBuff[6];           
    
    cn_l  = (Iqomah[SholatNow]*60);
    Disp.drawRect(8,2,87,13);
    if((Tmr-lsRn) > 1000 and ct <=cn_l)
      {
          lsRn = Tmr;        
          mnt = floor((cn_l-ct)/60); 
          scd = (cn_l-ct)%60;
          if(mnt>0) {sprintf(locBuff,"%d:%02d MENIT",mnt,scd);}
            else    {sprintf(locBuff,"-%d DETIK",scd);}
          fType(0);
          dwCtr(0,0,"IQOMAH");
          //fType(1);
          dwCtr(0,8,locBuff);
          if (ct> (cn_l-5))
           Buzzer(1);// Buzzer on 2 seccon before Iqomah
           
          else Buzzer (0);
          ct++;
          DoSwap = true;
             
      }
    if (ct > cn_l)
      {
       dwDone(DrawAdd);
       ct = 0;
      //digitalWrite(BUZZ, LOW);
      Buzzer(1);
      }    
  }
    
char* drawSholat_S1(int y) // Box Sholah Time
{
     //SholatNow  = -1;
     static const char  sholat[8][8] = {"IMSAK","SHUBUH","TERBIT","DHUHA","DZUHUR","ASHAR","MAGHRIB","ISYA'",};  
     static char  BuffTime[150];
    float   stime1   = sholatT[0];
    float   stime2   = sholatT[1];
    float   stime3   = sholatT[4];
    float   stime4   = sholatT[5];
    float   stime5   = sholatT[6];
    float   stime6   = sholatT[7];
    float   stime7   = sholatT[2];
    float   stime8   = sholatT[3];
    {
        uint8_t shour1   = floor(stime1);
        uint8_t shour2   = floor(stime2);
        uint8_t shour3   = floor(stime7);
        uint8_t shour4   = floor(stime8);
        uint8_t shour5   = floor(stime3);
        uint8_t shour6   = floor(stime4);
        uint8_t shour7   = floor(stime5);
        uint8_t shour8   = floor(stime6);
       
        uint8_t sminute1 = floor((stime1-(float)shour1)*60);
        uint8_t sminute2 = floor((stime2-(float)shour2)*60);
        uint8_t sminute3 = floor((stime7-(float)shour3)*60);
        uint8_t sminute4 = floor((stime8-(float)shour4)*60);
        uint8_t sminute5 = floor((stime3-(float)shour5)*60);
        uint8_t sminute6 = floor((stime4-(float)shour6)*60);
        uint8_t sminute7 = floor((stime5-(float)shour7)*60);
        uint8_t sminute8 = floor((stime6-(float)shour8)*60);
        
        //sprintf(BuffTime,"%s %02d:%02d"   ,sholat[0],shour1,sminute1); //wes muncul
        sprintf(BuffTime,"%s %02d:%02d   %s %02d:%02d   %s %02d:%02d   %s %02d:%02d   %s %02d:%02d   %s %02d:%02d   %s %02d:%02d   %s %02d:%02d\0",sholat[0],shour1,sminute1, sholat[1],shour2,sminute2, sholat[2],shour3,sminute3, sholat[3],shour4,sminute4, sholat[4],shour5,sminute5, sholat[5],shour6,sminute6, sholat[6],shour7,sminute7, sholat[7],shour8,sminute8); 

         
        return BuffTime;
    }
  }
 
  
void drawSholat_S(int sNum,int c) // Box Sholah Time
  {
    char  BuffTime[10];
    char  BuffShol[7];
    float   stime   = sholatT[sNum];
    uint8_t shour   = floor(stime);
    uint8_t sminute = floor((stime-(float)shour)*60);
    uint8_t ssecond = floor((stime-(float)shour-(float)sminute/60)*3600);
    //sprintf(BuffTime,"%02d:%02d:%02d",shour,sminute,ssecond);
    sprintf(BuffTime,"%02d:%02d",shour,sminute);
    //Disp.drawRect(c+1,2,94,13);
    //fType(0); 
    Disp.setFont(System6x7);
    dwCtr(c+12,0,sholatN(sNum));    // if font 5x7 pos 0
    dwCtr(c+12,8,BuffTime);          // if font 5x7 pos 9
    DoSwap = true;          
  }
  
void drawSholat(int DrawAdd)
  {
    // check RunSelector
//    int DrawAdd = 0b0000000000000100;
    if(!dwDo(DrawAdd)) return; 

    static uint8_t    x;
    static uint8_t    s; // 0=in, 1=out
    static uint8_t    sNum; 
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();
    uint8_t           c=32;
    uint8_t    first_sNum = 0; 
    int               DrawWd=DWidth - c;    

    if((Tmr-lsRn)>10) 
      {
        if(s==0 and x<(DrawWd/2)){x++;lsRn=Tmr;}
        if(s==1 and x>0){x--;lsRn=Tmr;}
      }
      
    if((Tmr-lsRn)>2000 and x ==(DrawWd/2)) {s=1;}
    if (x == 0 and s==1) 
      { 
        if (sNum <7){sNum++;}
        else 
          { 
           dwDone(DrawAdd);
           sNum=0;
          } 
        s=0;
      }

    if(Prm.SI==0) {first_sNum =1;}
    else {first_sNum =0;}
    if(Prm.SI==0 and sNum == 0) {sNum=1;}
    if(Prm.ST==0 and sNum == 2) {sNum=3;}
    if(Prm.SU==0 and sNum == 3) {sNum=4;}

 
    if(  (((sNum == first_sNum) and s ==0) or 
          ((sNum == 7)and s == 1)) 
          and x <=20) {
            //drawSmallTS
            (int(x/2));} 
    else {//drawSmallTS(10);
    }
    drawSholat_S(sNum, c);

    Disp.drawFilledRect(c,0,c+DrawWd/2-x,15,0);
    Disp.drawFilledRect(DrawWd/2+x+c,0,95,15,0); 
    Disp.drawFilledRect(30,15,0,0,0) ; 
    drawSmallTS(0);
    
  

         
  }
/*
void drawSholat2(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return; 
    char  BuffTime[10];
    static uint8_t    y;
    static uint8_t    x;
    static uint8_t    s; // 0=in, 1=out
    static uint8_t    sNum; 
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();
    uint8_t           c=15;
    uint8_t           first_sNum = 0; 
    int               DrawWh=DHeight+c; 
    int               DrawWd=DWidth;   

    if((Tmr-lsRn)>27) 
      {
        if(s==0 and y<7 ){y++;lsRn=Tmr;}
        if(s==1 and y>0 ){y--;lsRn=Tmr;}
      }
      
    if((Tmr-lsRn)>2500 and y == 7) {s=1;}
    if (y == 0  and s==1)
      { 
        if (sNum <7){sNum++;}
        else 
          { 
           dwDone(DrawAdd);
           sNum=0;
          } 
        s=0;
      }
 
    if(  (((sNum == first_sNum) and s ==0) or 
          ((sNum == 7)and s == 1)) 
          and y <=8 and x<=32) {drawSmallTS(9);} 
    else { drawSholat_S(sNum,y-7);}
    drawSmallTS(9);
  }
  */
void drawGreg_DS(uint16_t y)   //Draw Date
  { 
    char  Buff[20];
    sprintf(Buff,"%d-%s-%d",now.day(),MonthName(now.month()),now.year());
    
    dwCtr(32,y,Buff);
    DoSwap = true;
  }

void drawSmallTS(int y)
  {
    char BuffH[3];
    char BuffM[3];
    char BuffS[3];
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();
    //sprintf(BuffH,"%02d:%02d:%02d",now.hour(),now.minute(),now.second());
    sprintf(BuffH,"%02d",now.hour());
    sprintf(BuffM,"%02d",now.minute());
    //sprintf(BuffS,"%02d",now.second());
    
    fType(1);
    
    Disp.drawText(0,0,BuffH);
    Disp.drawText(17,0,BuffM);
    //Disp.drawText(30,y,BuffS);
    
    //if(Tmr-lsRn<500)Disp.drawLine((y-10)+1,7,(y-10)+9,7,1);
    if(Tmr-lsRn<500){
    Disp.drawRect   (14,5,15,6,1);    // Tampil Detik atau Dot
    Disp.drawRect   (14,9,15,10,1);  // Tampil Detik atau Dot
    }
    if(Tmr-lsRn>1000) lsRn = Tmr;
    DoSwap = true;
  } 
  

void drawGreg_TS(uint16_t y)   // Draw Time
  {
    char  Buff[20];
    
    sprintf(Buff,"%02d:%02d:%02d",now.hour(),now.minute(),now.second());    
    dwCtr(0,y,Buff);
    DoSwap = true; 
  }

void anim_DT(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return; 
    
    static uint8_t    y;
    static uint8_t    s; // 0=in, 1=out              
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();

    if((Tmr-lsRn)>100) 
      { 
        if(s==0 and y<7){lsRn=Tmr;y++;}
        if(s==1 and y>0){lsRn=Tmr;y--;}
      }
    if((Tmr-lsRn)>10000 and y ==7) {s=1;}
    if (y==7)
      {
       // Disp.drawRect(1,2,62,13);
      }
    if (y == 0 and s==1) {dwDone(DrawAdd); s=0;}
      fType(2);
     
      drawGreg_TS(7-y);       
    }

void anim_DT1(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return; 
    
    static uint8_t    y;
    static uint8_t    s; // 0=in, 1=out              
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();

    if((Tmr-lsRn)>100) 
      { 
        if(s==0 and y<7){lsRn=Tmr;y++;}
        if(s==1 and y>0){lsRn=Tmr;y--;}
      }
    if((Tmr-lsRn)>10000 and y ==7) {s=1;}
    if (y==7)
      {
       // Disp.drawRect(1,2,62,13);
      }
    if (y == 0 and s==1) {dwDone(DrawAdd); s=0;}
      fType (4);   // Huruf Arab
     
      drawGreg_TS(7-y);       
    }
    
void dwMrq(const char* msg, int Speed, int dDT, int DrawAdd)
  { 
    // check RunSelector
    static uint16_t   x; 
    if(!dwDo(DrawAdd)) return;
    if (reset_x !=0) { x=0;reset_x = 0;}      

       
    static uint32_t   lsRn;
    int fullScroll = Disp.textWidth(msg) + DWidth;    
    uint32_t          Tmr = millis();
    if((Tmr-lsRn)> Speed)
      { lsRn=Tmr;
        if (x < fullScroll) {++x;}
        else {  dwDone(DrawAdd); 
                x = 0;return;}
     if(dDT==1)
        {
        //fType(0); 
         
        Disp.drawText(DWidth - x, 8, msg);
        drawGreg_DS(0);
        
        if (x<=6)                     { drawSmallTS(6-x);}
        else if (x>=(fullScroll-6))   { drawSmallTS(6-(fullScroll-x));}
        else                          { //Disp.drawRect(1,10,62,15);//UUK
        
        Disp.drawFilledRect(30,15,0,0,0);
        drawSmallTS(0);
        }
                                        
        }
     else if(dDT==2)
        {    
        //fType(0);
        Disp.drawText(DWidth - x, 0 , msg);
        drawGreg_DS(8);
        
        Disp.drawFilledRect(30,15,0,0,0);
        if(x<=6)                        { drawSmallTS(x-6);}
        else if (x>=(fullScroll-6))    { drawSmallTS((fullScroll-x)-6);}
        else {
                                               drawSmallTS(0);   }
                   
        
        }
     else if(dDT==3)
        {
        //fType(3);
        Disp.drawText(DWidth - x, 1, msg);
        
        //Disp.setFont(Font1); 
        //Disp.drawFilledRect(31,15,0,0,0); // Pembatas tak terlihat 
                   
        //Disp.drawLine(0,0,95,0);
        //Disp.drawLine(0,15,95,15);       
       
                      
          }
      else  {
        //fType(0);
        Disp.setFont(System6x7); 
        Disp.drawText(DWidth - x, 3, msg);
        Disp.drawLine(0,0,95,0);
        Disp.drawLine(0,15,95,15);}
     
      DoSwap = true;
       
     }
  }

void blinkBlock(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return;

    static uint32_t   lsRn;
    static uint32_t   ct, ct_l;
    uint32_t          Tmr = millis();

    if(jumat)       {ct_l = Prm.JM * 60;}
    else            {ct_l = Prm.SO * 60;}
    jumat =false;
     
    if((Tmr-lsRn)> 1000)
      { lsRn=Tmr;
//        Disp.drawChar(1, 1 , ct);
        if((ct%2) == 0)
          { 

            Disp.drawFilledRect(DWidth-3, DHeight-3, DWidth-2, DHeight-2);}
        DoSwap = true; 
        ct++;}
    if (ct> ct_l) 
      { dwDone(DrawAdd);
        azzan = false;
        ct = 0;
      }
   // Disp.drawText(50,4,String(ct_l-ct)); 
  }

// =========================================
// Drawing Tools============================
// =========================================
boolean dwDo(int DrawAdd)
  { if (RunSel== DrawAdd) {return true;}
    else return false;}
  
void dwDone(int DrawAdd)
  { RunFinish = DrawAdd;
    RunSel = 0;}
    
void dwCtr(int x, int y,const char* Msg)
  { int   tw = Disp.textWidth(Msg);
    int   th = Disp.textHeight();
    int   c = int((DWidth-x-tw)/2);
    Disp.drawFilledRect(x+c-1,y,x+tw+c,y+th,0);
    Disp.drawText(x+c,y,Msg);}

void Buzzer(uint8_t state)
  {
      if(state ==1 and Prm.BZ == 1)
    //if(state ==1  || Prm.BZ == 1)
      {tone(BUZZ, 4500);
      delay (400);      
      }
    else 
      { noTone(BUZZ);
     }
  }
  
void fType(int x)
  {
    if(x==0) Disp.setFont(SystemFont5x7);
    else if(x==1) Disp.setFont(Angka6x14);
    else if(x==2) Disp.setFont(Bigest);
    else if(x==3) Disp.setFont(Arial_black_16);
    else Disp.setFont(arab16x10);

    
  }

 
