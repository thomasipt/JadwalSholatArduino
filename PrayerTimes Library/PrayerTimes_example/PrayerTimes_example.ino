#include <stdio.h>
#include <Wire.h>
#include <Time.h>
#include "PrayerTimes.h"

double times[sizeof(TimeName) / sizeof(char*)];

void p(char *fmt, ... ) {
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}



void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println("PTimes:");

  int dst = 7;
  
  set_calc_method(MWL);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  //MEKKA
  float latitude=-7.55;
  float longitude=110.8243;
  get_prayer_times(2017, 4, 1, latitude, longitude, dst, times);

  for (int i = 0; i < sizeof(times) / sizeof(double); i++) {
    char tmp[10];
    int hours, minutes;
    get_float_time_parts(times[i], hours, minutes);
    
  if (i==0) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+2);}  
  if (i==1) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes-2);}
  if (i==2) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+2);}
  if (i==3) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+2);}
  if (i==4) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+3);}
  if (i==5) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+3);}
  if (i==6) {p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes+2);}
    
  //p("%d \t %10s %s \t %02d:%02d \n\r", i, TimeName[i], dtostrf(times[i], 2, 2, tmp), hours, minutes);
  }
  delay(10000);
}

