#include <DMD.h>
#include <SystemFont5x7.h>
#include <Arial_Black_16.h>

#include <Wire.h>
#include <SPI.h>
//#include <DMD.h>        
#include <TimerOne.h>
//#include "SystemFont5x7.h"
//#include "font.h"
//#include "Arial14.h"
//#include "Arial_Black_16.h"
#include "RTClib.h"
#include <PString.h>

//Init Settings DMD
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
double fajr, sunRise, zuhr, asr, maghrib, isha;

RTC_DS1307 RTC;

int hours, mm, ss;
char buffer[10];
PString str(buffer, sizeof(buffer));

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

//Init Settings GPS Location
double setLongitude = 110.50;
double setLatitude = -7.32;
int setTimeZone = 7;
double setFajarTwilight = -20;
double setIshaTwilight = -18;

void setup() {
  Timer1.initialize(2000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);

  Wire.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  RTC.begin(); 
  Wire.beginTransmission(0X68);
  Wire.write(0x07);
  Wire.write(0x10);
  Wire.endTransmission();
}

void loop() {
  DateTime now = RTC.now();
  int count = 0;
  int a = 0;
  //hours = now.hour();
  //mm = now.minute();
  //ss = now.second();
  int x = 0;
  while (count < 5) {
    a++;
    DateTime now = RTC.now();
    hours = now.hour();
    mm = now.minute();
    ss = now.second();
    dmd.selectFont(Arial_Black_16);
    str.begin();
    if (hours < 10) {
      str.print(0);
    }
    str.print(hours);
    dmd.drawString(1, 1, str, str.length(), GRAPHICS_NORMAL );

    //delay(5000);
    str.begin();
    str.print(":");
    if (a >= 21) {
      dmd.drawString(19, 0, str, str.length(), GRAPHICS_OR );
    }
    else {
      dmd.drawString(19, 0, str, str.length(), GRAPHICS_NOR );
    }

    str.begin();
    if (mm < 10) {
      str.print(0);
    }
    str.print (mm);
    dmd.drawString(23, 1, str, str.length(), GRAPHICS_NORMAL );

    str.begin();
    str.print(":");
    if (a >= 21) {
      dmd.drawString(42, 0, str, str.length(), GRAPHICS_OR );
    }
    else {
      dmd.drawString(42, 0, str, str.length(), GRAPHICS_NOR );
    }

    str.begin();
    if (ss < 10) {
      str.print(0);
    }
    str.print(ss);
    dmd.drawString(46, 1, str, str.length(), GRAPHICS_NORMAL );
    
    if(a==72) {
      a=0;
      count++;
    }
  }
  count = 0;
  dmd.clearScreen( true );
  while (count < 40) {
    a++;
    DateTime now = RTC.now();
    calcPrayerTimes(now.year(), now.month(), now.day(), setLongitude, setLatitude, setTimeZone, setFajarTwilight, setIshaTwilight, fajr, sunRise, zuhr, asr, maghrib, isha);
    int hours, minutes;

    hours = now.hour();
    mm = now.minute();
    ss = now.second();
    
    str.begin();
    dmd.selectFont(SystemFont5x7);
    if (hours < 10) {
      str.print(0);
    }
    str.print (hours);
    dmd.drawString(1, 1, str, str.length(), GRAPHICS_NORMAL );

    str.begin();
    str.print(":");
    if (a > 5) {
      dmd.drawString(13, 1, str, str.length(), GRAPHICS_OR );
    }
    else {
      dmd.drawString(13, 1, str, str.length(), GRAPHICS_NOR );
    }

    str.begin();
    if (mm < 10) {
      str.print(0);
    }
    str.print (mm);
    dmd.drawString(18, 1, str, str.length(), GRAPHICS_NORMAL );

    if (count >= 32) {
      x = 1;
      //doubleToHrMin(fajr, hours, minutes);
      doubleToHrMin(isha, hours, minutes);
    }
    else if (count >= 24) {
      // doubleToHrMin(zuhr, hours, minutes);
      doubleToHrMin(maghrib, hours, minutes);
      x = 2;
    }
    else if (count >= 16) {
      doubleToHrMin(asr, hours, minutes);
      x = 3;
    }
    else if (count >= 8) {
      //doubleToHrMin(maghrib, hours, minutes);
      doubleToHrMin(zuhr, hours, minutes);
      x = 4;
    }
    else if (count >= 0) {
      //doubleToHrMin(isha, hours, minutes);
      doubleToHrMin(fajr, hours, minutes);
      x = 5;
    }
    
    dmd.selectFont(SystemFont5x7);;
    str.begin();
    if (hours < 10) {
      str.print("0");
    }
    str.print(hours);
    dmd.drawString(35, 1, str, str.length(), GRAPHICS_NORMAL );
    dmd.drawString(47, 1, ":", 1, GRAPHICS_NORMAL );
    
    str.begin();
    if (minutes < 10) {
      str.print("0");
    }
    str.print(minutes);
    dmd.drawString(52, 1, str, str.length(), GRAPHICS_NORMAL );
    dmd.selectFont(SystemFont5x7);
    str.begin();
    
    switch (x) {
      case 1:
        str.print("   ISYA  ");
        dmd.drawString(2, 9, str, str.length(), GRAPHICS_NORMAL );
        break;
      case 2:
        str.print(" MAGHRIB ");
        dmd.drawString(5, 9, str, str.length(), GRAPHICS_NORMAL );
        break;
      case 3:
        str.print("  ASHAR  ");
        dmd.drawString(5, 9, str, str.length(), GRAPHICS_NORMAL );
        break;
      case 4:
        str.print(" DZUHUR ");
        dmd.drawString(7, 9, str, str.length(), GRAPHICS_NORMAL );
        break;
      case 5:
        str.print("  SHUBUH ");
        dmd.drawString(1, 9, str, str.length(), GRAPHICS_NORMAL );
        break;
    }
    if (a == 10) {
      a = 0;
      count++;
    }
  }
  dmd.clearScreen( true );
}


double degToRad(double degree)   {return ((3.1415926 / 180) * degree);}

double radToDeg(double radian)   {return (radian * (180 / 3.1415926));}

double moreLess360(double value) {
  while (value > 360 || value < 0) {
    if (value > 360) {
      value -= 360;
    }
    else if (value < 0) {
      value += 360;
    }
  }
  return value;
}

double moreLess24(double value) {
  while (value > 24 || value < 0) {
    if (value > 24) {
      value -= 24;
    }
    else if (value < 0) {
      value += 24;
    }
  }
  return value;
}

void doubleToHrMin(double number, int &hours, int &minutes) {
  hours = floor(moreLess24(number));
  minutes = floor(moreLess24(number - hours) * 60);
}

void calcPrayerTimes(int year, int month, int day,
                     double longitude, double latitude, int timeZone,
                     double fajrTwilight, double ishaTwilight,
                     double &fajrTime, double &sunRiseTime, double &zuhrTime,
                     double &asrTime, double &maghribTime, double &ishaTime)
                     {
  double D = (367 * year) - ((year + (int)((month + 9) / 12)) * 7 / 4) + (((int)(275 * month / 9)) + day - 730531.5);

  double L = 280.461 + 0.9856474 * D;
  L = moreLess360(L);

  double M = 357.528 + (0.9856003) * D;
  M = moreLess360(M);

  double Lambda = L + 1.915 * sin(degToRad(M)) + 0.02 * sin(degToRad(2 * M));
  Lambda = moreLess360(Lambda);

  double Obliquity = 23.439 - 0.0000004 * D;
  double Alpha = radToDeg(atan((cos(degToRad(Obliquity)) * tan(degToRad(Lambda)))));
  Alpha = moreLess360(Alpha);

  Alpha = Alpha - (360 * (int)(Alpha / 360));
  Alpha = Alpha + 90 * (floor(Lambda / 90) - floor(Alpha / 90));

  double ST = 100.46 + 0.985647352 * D;
  double Dec = radToDeg(asin(sin(degToRad(Obliquity)) * sin(degToRad(Lambda))));
  double Durinal_Arc = radToDeg(acos((sin(degToRad(-0.8333)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));

  double Noon = Alpha - ST;
  Noon = moreLess360(Noon);

  double UT_Noon = Noon - longitude;

  ////////////////////////////////////////////
  // Calculating Prayer Times Arcs & Times //
  //////////////////////////////////////////

  // 2) Zuhr Time [Local noon]
  zuhrTime = UT_Noon / 15 + timeZone;

  // Asr Hanafi
  double Asr_Alt = radToDeg(atan(2 + tan(degToRad(latitude - Dec))));
  double Asr_Arc = radToDeg(atan(1.7 + tan(degToRad(latitude - Dec))));
  // Asr Shafii
  // double Asr_Alt = radToDeg(atan(1 + tan(degToRad(latitude - Dec))));
  // double Asr_Arc = radToDeg(acos((sin(degToRad(90 - Asr_Alt)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));
  Asr_Arc = Asr_Arc / 14.7;
  
  // 3) Asr Time
  asrTime = zuhrTime + Asr_Arc;

  // 1) Shorouq Time
  sunRiseTime = zuhrTime - (Durinal_Arc / 15);

  // 4) Maghrib Time
  maghribTime = zuhrTime + (Durinal_Arc / 14.7);

  double Esha_Arc = radToDeg(acos((sin(degToRad(ishaTwilight)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));

  // 5) Isha Time
  ishaTime = zuhrTime + (Esha_Arc / 15);

  // 0) Fajr Time
  double Fajr_Arc = radToDeg(acos((sin(degToRad(fajrTwilight)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));
  fajrTime = zuhrTime - (Fajr_Arc / 15);

  return;
}
