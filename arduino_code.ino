#include "WiFi.h"
#include <HTTPClient.h>
const char* ssid = "nour";
const char* password = "12435687";
String Web_App_URL = "https://script.google.com/macros/s/AKfycbwWJ53aagUFELEZg78X_tLmpeNJ2JGjKLZYrpJxd8UUPR743YFk7b4U-5AjzVtSELX2/exec";
#define TdsSensorPin 32
#define ld 23
#define VREF 3.3
#define ADC_RESOLUTION 4096
const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;
float temperature = 25.0;
float knownTDSValue1 = 0.0;
int knownAnalogValue1 = 0;
float knownTDSValue2 = 1000.0;
int knownAnalogValue2 = 1000;
float averageVoltage = 0;
int tdsValue = 0;

#define SENSOR  2
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
int cnt =0;
float calc(){
      totalMilliLitres =0;
      for(int i = 0 ; i < 5 ; ++i){
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = (pulse1Sec) / calibrationFactor;
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    delay(1000);
      }
    return totalMilliLitres;
}
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}
String leakage(){
  float init = getDistance();
  float flow = calc();
  float fin =   getDistance();
  init = min(fin , init);
  Serial.print("final dist is : ");
  Serial.println(fin);
  Serial.print("initial dist is : ");
  Serial.println(init);
  Serial.print((fin-init)*18.5*18.5);
  Serial.print("  compared with ");
  Serial.println(flow);
  if((fin-init)*18.5*18.5-flow > 50 ) return "TRUE";
  else return "FALSE";
}
float getDistance() {
  float tot=0;
  int cnt=5;
  while(cnt--){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED / 2;
    tot += distanceCm;
    delay(10);
  }
  tot /=5;
  return tot;
}
float getCalibratedTDSValue() {
    int rawValue = analogRead(TdsSensorPin);
    float voltage = (rawValue * VREF) / ADC_RESOLUTION;
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = voltage / compensationCoefficient;
    float tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage) * 0.5;
    float calibratedTDS = ((knownTDSValue2 - knownTDSValue1) / (knownAnalogValue2 - knownAnalogValue1)) * (rawValue - knownAnalogValue1) + knownTDSValue1;
    return calibratedTDS;
}
int calcPercent(){
  return map(getDistance() , 28.06 , 0 , 0 , 100);
}
void setup() {
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(ld , OUTPUT);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  pinMode(TdsSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println();
  delay(1000);
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int connecting_process_timed_out = 20;
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  Serial.println("WiFi connected");
  Serial.println("------------");
  delay(100);
  delay(3000);
}

void loop() {
    tdsValue = getCalibratedTDSValue();
    Serial.print("TDS Value before sending: ");
    Serial.println(tdsValue);
    if(tdsValue > 300) digitalWrite(ld , HIGH);
    else digitalWrite(ld , LOW);
    String Send_Data_URL = Web_App_URL + "?sts=write";
    Send_Data_URL += "&tds=" + String(tdsValue);
    Send_Data_URL += "&water="+leakage();
    Send_Data_URL += "&vol="+String(calcPercent());
    Serial.print("Send Data URL: ");
    Serial.println(Send_Data_URL);
    HTTPClient http;
    http.begin(Send_Data_URL.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Response: " + payload);
    }
    http.end();
}