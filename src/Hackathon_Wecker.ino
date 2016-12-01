#include <Time.h>
#include <TimeLib.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#define AP_SSID "Joshua's iPhone"  // AP Netzwerk


//** WEATHER **
int OUT_TEMP = 0;
String WEATHER_STATE;

//** ALARM **
int ALARM_HOUR;
int ALARM_MINUTE;
int RING_FOR = 60000;
boolean alarmDays [7] = { false, true, true, true, true, true, false }; // SUN, MON ...

//** TIME NTP **
unsigned int localPort = 80;
IPAddress timeServerIP;
const char* ntpServerName = "pool.ntp.org";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP udp;

//** TIMING **
long previousMillisMinute = 0;
long intervalMinute = 60000; // 1 Minute
long previousMillisTime = 0;
long intervalTime = 3600000; // 1 Stunde

//** ACTUAL TIME **
int HOUR;
int MINUTE;
long epochTime;

//TESTING PURPOSE
char date[15];


String _display() {
  if (!buttonPressed()) {
    char str[10];
    sprintf(str, "%d", MINUTE);
  
    char str2[10];
    sprintf(str2, "%d", HOUR);
    
    strcat(str2, ":");
    if (MINUTE < 10) {
      strcat(str2, "0");
    }
    strcat(str2, str);
    return str2;
  } else {
    char str3[15];
    return (strcat(strcat(str3, " "), date) + WEATHER_STATE);
  }
}

void loop() {
  
  int v = drehRead();
  if(v==0)
    v = readLightLevel() < 50 ? readLightLevel() + 2 : (double)(readLightLevel() / 100 * 5); //TODO
  matrixAnzeige(_display(), v);
  
  unsigned long currentMillisMinute = millis();
  if (currentMillisMinute - previousMillisMinute >= intervalMinute) {
    _adjustTime((currentMillisMinute - previousMillisMinute - intervalMinute) / 1000);
    Serial.println((currentMillisMinute - previousMillisMinute - intervalMinute) / 1000);
    MINUTE++;
    epochTime += 60;
    if (MINUTE >= 60) {
      MINUTE = 0;
      if (HOUR <= 24)
        HOUR++;
      else
        HOUR = 0;
    }
    previousMillisMinute = currentMillisMinute;
  }
  
  if (HOUR == ALARM_HOUR && MINUTE == ALARM_MINUTE) {
    alarm();
  }

  unsigned long currentMillisTime = millis();
  if (currentMillisTime - previousMillisTime >= intervalTime) {
    setTime();
    previousMillisTime = currentMillisTime;
  }
}

void setup() {
  kitInit();

  //setAlarmBySunrise();
  setAlarmByWeather();
  
  setTime();
  
  //TESTING
  ALARM_HOUR = 7;
  ALARM_MINUTE = 30;
}
