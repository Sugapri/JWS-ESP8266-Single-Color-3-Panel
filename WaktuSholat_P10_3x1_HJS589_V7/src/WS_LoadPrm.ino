/*------------------------------------------
  // Function Setup Parameter EEPROM
  ------------------------------------------*/
/* Struktur Data EEPROM
        1   byte add  0   unit8_t       Parameter State  212 if ok
        4   byte add  1   float         Latitude         code NLA
        4   byte add  5   float         Longitude        code NLO
        4   byte add  9   float         Altitude         code NAL
        4   byte add 13   float         TimeZone         code NTZ
        1   byte add 17   uint8_t       Masjid Type      code NMT
        1   byte add 18   uint8_t       Led Brighnest    code NBL
        1   byte add 19   uint8_t       Ihtiyati Time    code NIH  -- waktu pengaman
        1   byte add 20   uint8_t       Sholat Time      code NSO  -- waktu tunggu sholat
        1   byte add 21   uint8_t       Jum'at Time      code NJM  -- waktu tunggu khutbah jumat
        1   byte add 22   uint8_t       IQ Subuh         code NI0  -- waktu iqomah subuh
        1   byte add 23   uint8_t       IQ Dhuhur        code NI4  -- waktu iqomah Dhuhur
        1   byte add 24   uint8_t       IQ Ashar         code NI5  -- waktu iqomah Ashar
        1   byte add 25   uint8_t       IQ Maghrib       code NI6  -- waktu iqomah Magrib
        1   byte add 26   uint8_t       IQ Isya          code NI7  -- waktu iqomah Isya
        1   byte add 27   uint8_t       Buzzer           code NBZ  -- Buzzer on/off
        1   byte add 28   uint8_t       Show Imsak       code NSI  -- Imsak on/off
        1   byte add 29   uint8_t       Show Terbit      code NST  -- Terbit on/off
        1   byte add 30   uint8_t       Show Dhuha       code NSU  -- Dhuha on/off
        1   byte add 31   int8_t        Corection HijrD  code NCH  -- Corection Hijriah Date

        26   byte add 29   -------  EMPTY  --------
        50  byte add 55   char          Masjid Name      code CMN
        150 byte add 130  char          Info 1           code CN1
        150 byte add 280  char          Info 2           code CN2
        150 byte add 430  char          Info 3           code CN3
        150 byte add 580  char          Sholat Message   code CSM  -- pesan mejelang sholat biasa
        150 byte add 730  char          Jumat Message    code CJM  -- pesan setelah azan
        144 byte add 880  -------  EMPTY  --------

        Memory Size arduino Uno
          Flash  32k bytes (of which .5k is used for the bootloader)
          SRAM   2k bytes
          EEPROM 1k byte
*/

void serialEvent() {
  if (!client.connected()) {
    client = server.available();
    return;
  }

  if (client.available()) {
    uint8_t i = 0;
    while (client.available()) {
      CH_Prm[i] = (uint8_t)client.read();
      CH_Prm[++i] = 0;
      

    }
    Serial.println(CH_Prm);
    LoadPrm();

  }

}

void LoadPrm()
{
  String BT_Param;
  uint16_t  eeAdd = 0;
  uint8_t   eePut = 0;
  uint8_t   eeMax = 0;
  uint8_t   lenprm = strlen(CH_Prm) - 3;

  // Put Char Data
  if (CH_Prm[0] == 'C')
  {      if (CH_Prm[1] == 'M' and CH_Prm[2] == 'N')   { eeAdd = 55; eeMax = 74;    } //eeAdd = 49;eeMax = 80;
    else if (CH_Prm[1] == 'N' and CH_Prm[2] == '1')   { eeAdd = 130; eeMax = 149;  }
    else if (CH_Prm[1] == 'N' and CH_Prm[2] == '2')   { eeAdd = 280; eeMax = 149;  }
    else if (CH_Prm[1] == 'N' and CH_Prm[2] == '3')   { eeAdd = 430; eeMax = 149;  }
    else if (CH_Prm[1] == 'S' and CH_Prm[2] == 'M')   { eeAdd = 580; eeMax = 149;  }
    else if (CH_Prm[1] == 'J' and CH_Prm[2] == 'M')   { eeAdd = 730; eeMax = 149;  }

    int eeCount = 0;
    while ((eeCount <= lenprm) and (eeCount < eeMax))
    {
      EEPROM.write(eeAdd + eeCount, CH_Prm[3 + eeCount]);
      eeCount++;
    }
    EEPROM.write(eeAdd + eeCount, '\0');
  }

  // Put Numeric Data
  else if (CH_Prm[0] == 'N')
  {
    BT_Param = String(CH_Prm);

         if (BT_Param.substring(1, 2).equals("LA"))   { eeAdd = 1 ; eePut = 1;    }
    else if (BT_Param.substring(1, 2).equals("LO"))   { eeAdd = 5 ; eePut = 1;    }
    else if (BT_Param.substring(1, 2).equals("AL"))   { eeAdd = 9 ; eePut = 1;    }
    else if (BT_Param.substring(1, 2).equals("TZ"))   { eeAdd = 13; eePut = 1;    }
    else if (BT_Param.substring(1, 2).equals("MT"))   { eeAdd = 17; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("BL"))   { eeAdd = 18; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("IH"))   { eeAdd = 19; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("SO"))   { eeAdd = 20; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("JM"))   { eeAdd = 21; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("S1"))   { eeAdd = 22; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("S2"))   { eeAdd = 23; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("S3"))   { eeAdd = 24; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("S4"))   { eeAdd = 25; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("S5"))   { eeAdd = 26; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("BZ"))   { eeAdd = 27; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("SI"))   { eeAdd = 28; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("ST"))   { eeAdd = 29; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("SU"))   { eeAdd = 30; eePut = 2;    }
    else if (BT_Param.substring(1, 2).equals("CH"))   { eeAdd = 31; eePut = 2;    }



    if (eePut == 1) { EEPROM.put(eeAdd, BT_Param.substring(3, BT_Param.length()).toFloat());   }// put float
    if (eePut == 2) { EEPROM.put(eeAdd, (uint8_t)BT_Param.substring(3, BT_Param.length()).toInt());   }// put uint8_t
    if (eePut == 3) { EEPROM.put(eeAdd, (int8_t)BT_Param.substring(3, BT_Param.length()).toInt());    }// put uint8_t


  }
    else if (CH_Prm[0] == 'S' and CH_Prm[1] == 'D' and CH_Prm[2] == 'T')
  {
    BT_Param = String(CH_Prm);
    Clock.setClockMode(false);
    Clock.setDate(byte(BT_Param.substring(3, 5).toInt()));
    Clock.setMonth(byte(BT_Param.substring(5, 7).toInt()));
    Clock.setYear(byte(BT_Param.substring(7, 9).toInt()));
    Clock.setHour(byte(BT_Param.substring(9, 11).toInt()));
    Clock.setMinute(byte(BT_Param.substring(11, 13).toInt()));
    Clock.setSecond(byte(BT_Param.substring(13, 15).toInt()));
    Clock.setDoW(byte(BT_Param.substring(15, 16).toInt()));
  }
  EEPROM.commit();
  // Get New/updeted parameter*/
  GetPrm();

  if (CH_Prm[0] == 'S' and CH_Prm[1] == 'Q' and CH_Prm[2] == 'P')
  {
    SendPrm();
  }
}

void GetPrm()
{
  //    Serial.println("Get parameter");
  // Get Parameter and check
  EEPROM.get(1, Prm.L_LA); EEPROM.get(5, Prm.L_LO); EEPROM.get(9, Prm.L_AL); EEPROM.get(13, Prm.L_TZ);
  Prm.MT = EEPROM.read(17); Prm.BL = EEPROM.read(18);
  Prm.IH = EEPROM.read(19); Prm.SO = EEPROM.read(20);
  Prm.JM = EEPROM.read(21); Prm.I1 = EEPROM.read(22);
  Prm.I4 = EEPROM.read(23); Prm.I5 = EEPROM.read(24);
  Prm.I6 = EEPROM.read(25); Prm.I7 = EEPROM.read(26);
  Prm.BZ = EEPROM.read(27); Prm.SI = EEPROM.read(28);
  Prm.ST = EEPROM.read(29); Prm.SU = EEPROM.read(30);
  Prm.CH = EEPROM.read(31);
  if (EEPROM.read(0) != 212) //check value of Prm State .. set Default
  {
    set_default_prm();
  }
  /*if (now.year() < 2019)//check date time .. less than 1 jan 2018 set Default
    {
      set_default_time();
    }*/
  Iqomah[1] = Prm.I1;
  Iqomah[4] = Prm.I4;
  Iqomah[5] = Prm.I5;
  Iqomah[6] = Prm.I6;
  Iqomah[7] = Prm.I7;
  
  // user New Parameter
  setBrightness(int(Prm.BL));
  update_All_data();

}

void set_default_prm()
{
  // Put Parameter start form addr 500
  EEPROM.put(0, 212);   // 
  byte d[31] = { 1, 100, 2, 50, 100, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1}; //ket masjid,kecerahan,waktu pengaman,waktu sholat, waktu jumat,Iq Subhuh, Iq Dhuhur, Iq Ashar, Iq Mahgrib, Iq Isyak,
  EEPROM.put(17, d);
  EEPROM.put(1, -7.35646f);  // -7.356462 kordinat Mushola Masholik
  EEPROM.put(5, 112.68950f); //  112.689503
  EEPROM.put(9, 0.0f);
  EEPROM.put(13, 7.0f);
  EEPROM.put(55,  "SELAMAT DATANG DI MASJID AL - MASHOLICH JL.GELURAN PLN 1 SIDOARJO\0"); //SELAMAT DATANG DI MASJID AL - MASHOLICH JL.GELURAN PLN 1 SIDOARJO\0
  EEPROM.put(130, "SESUNGGUHNYA SHALAT ITU MERUPAKAN KEWAJIBAN YANG DITENTUKAN WAKTUNYA ATAS ORANG ORANG YANG BERIMAN (AN-NISA:103)\0"); //SESUNGGUHNYA SHALAT ITU MERUPAKAN KEWAJIBAN YANG DITENTUKAN WAKTUNYA ATAS ORANG ORANG YANG BERIMAN (AN-NISA:103)\0
  EEPROM.put(280, "HP MOHON DI MATIKAN\0"); //BAGI JAMAAH YANG MEMBAWA HP MOHON SEGERA DI NON AKTIFKAN\0
  EEPROM.put(430, "SHALAT BERJAMA`AH 27 DERAJAT LEBIH UTAMA DARIPADA SHALAT SENDIRIAN (HR.BUKHARI)\0"); //SHALAT BERJAMA`AH 27 DERAJAT LEBIH UTAMA DARIPADA SHALAT SENDIRIAN (HR.BUKHARI)\0
  EEPROM.put(580, "MOHON SHAF LURUS DAN DIRAPATKAN\0"); //MOHON SHAF LURUS DAN DIRAPATKAN\0
  EEPROM.put(730, "Ayo Sholat jumat\0");
  EEPROM.commit();
}
/*
void set_default_prm()
      {
        // Put Parameter start form addr 500
        Prm = (struct_param){212,-7.35915,112.68451,0,7,1,150,1,15,40,5,5,5,5,5,1,1,1,1,0};   // -7.309152, 112.734512
        EEPROM.put(0,Prm);
        EEPROM.put(55, "SELAMAT DATANG DI MUSHOLA MASHOLIK JL.GELURAN PLN 1 SIDOARJO\0");
        EEPROM.put(130, "SESUNGGUHNYA SHALAT ITU MERUPAKAN KEWAJIBAN YANG DITENTUKAN WAKTUNYA ATAS ORANG ORANG YANG BERIMAN (AN-NISA:103)\0");
        EEPROM.put(280,"HP MOHON DI MATIKAN\0");
        EEPROM.put(430,"SHALAT BERJAMA`AH 27 DERAJAT LEBIH UTAMA DARIPADA SHALAT SENDIRIAN (HR.BUKHARI)\0");
        EEPROM.put(580,"MOHON SHAF LURUS DAN DIRAPATKAN\0");
        EEPROM.put(730,"Ayo Jumatan\0");

      }
*/
void set_default_time()
{
  Clock.setClockMode(false);  // set to 24h
  Clock.setYear(byte(21));
  Clock.setMonth(byte(11));
  Clock.setDate(byte(3));
  Clock.setDoW(byte(2));
  Clock.setHour(byte(12));
  Clock.setMinute(byte(0));
  Clock.setSecond(byte(0));
  Clock.turnOffAlarm(1);
  Clock.turnOffAlarm(2);
}

// for debug
void SendPrm()
{
   char Info1[150];
   Serial.println(sizeof(Prm));
    Serial.print(F("state\t: "));  Serial.println(Prm.state);

  Serial.print(F("NLA  : ")); Serial.print(Prm.L_LA, 7); Serial.print("\t");
  Serial.print(F("NLO  : ")); Serial.print(Prm.L_LO, 7); Serial.print("\n");
  Serial.print(F("NAL  : ")); Serial.print(Prm.L_AL, 7); Serial.print("\t");
  Serial.print(F("NTZ  : ")); Serial.print(Prm.L_TZ, 0); Serial.print("\n");
  Serial.print(F("NMT  : ")); Serial.print(Prm.MT); Serial.print("\t");
  Serial.print(F("NIH  : ")); Serial.print(Prm.IH); Serial.print("\t");
  Serial.print(F("NCH  : ")); Serial.print(Prm.CH); Serial.print("\n");
  Serial.print(F("NSO  : ")); Serial.print(Prm.SO); Serial.print("\t");
  Serial.print(F("NJM  : ")); Serial.print(Prm.JM); Serial.print("\n");
  Serial.print(F("NI1  : ")); Serial.print(Prm.I1); Serial.print("\t");
  Serial.print(F("NI4  : ")); Serial.print(Prm.I4); Serial.print("\t");
  Serial.print(F("NI5  : ")); Serial.print(Prm.I5); Serial.print("\n");
  Serial.print(F("NI6  : ")); Serial.print(Prm.I6); Serial.print("\t");
  Serial.print(F("NI7  : ")); Serial.print(Prm.I7); Serial.print("\t");
  Serial.print(F("NBZ  : ")); Serial.print(Prm.BZ); Serial.print("\n");
  Serial.print(F("NSI  : ")); Serial.print(Prm.SI); Serial.print("\t");
  Serial.print(F("NST  : ")); Serial.print(Prm.ST); Serial.print("\t");
  Serial.print(F("NSU  : ")); Serial.print(Prm.SU); Serial.print("\n");
  Serial.print(F("NBL  : ")); Serial.print(Prm.BL); Serial.print("\n");
  Serial.print(drawInfo(130));Serial.print("\n");
  Serial.print(drawInfo(280));Serial.print("\n");
  Serial.print(drawInfo(430));Serial.print("\n");
  Serial.print(drawInfo(580));Serial.print("\n");
  Serial.print(drawInfo(730));Serial.print("\n");
  Serial.print(F("=== end of parameters ==="));

}
