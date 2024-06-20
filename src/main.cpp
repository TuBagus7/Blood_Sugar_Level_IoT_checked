#include <Arduino.h>
#include <WiFi.h> // library untuk wifi
#include "Wire.h"// library untuk LCD
#include <WiFiClientSecure.h> // Library untuk keamanan WiFi
#include <UniversalTelegramBot.h> // Library untuk Telegram
#include <ArduinoJson.h>// Library untuk API
#include <Preferences.h> // untuk password dan ssid serta konsumsi daya rendah
#include <LiquidCrystal_I2C.h>//library LCD 12C

LiquidCrystal_I2C lcd(0x27, 16, 2); // inisialisasi LCD dengan ukuran 16 X 2

// Configuration for WiFi / wifi setup
#define WIFI_SSID "Redmi Note 10 Pro"
#define WIFI_PASSWORD "1sampai8"

// Telegram Bot configuration
#define BOT_TOKEN "7080193445:AAF_JkqaSqixo59IP3Kx3wjsFuguIz8qRXk"//di dapat dari BotFather telegram
#define CHAT_ID "5382077795" //di dapat dari IDBot telegram

WiFiClientSecure secured_client;// kita panggil library keamanan WiFi untuk di deklarasikan
UniversalTelegramBot bot(BOT_TOKEN, secured_client);// untuk membuat telegram bot object

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done

int adcsensor;
const int bt = 0;
float btx, datafix, cacah;
int fix;
float kalibrasi;

void proses() {// function untuk kalibrasi
  adcsensor = analogRead(35);
  lcd.setCursor(0, 0);
  lcd.print("ADC= ");
  lcd.print(adcsensor);
  lcd.setCursor(0,1);
  for(int i =0; i <= cacah; i++){
    lcd.print("-");
  }
  lcd.print("      ");
  cacah++;
  datafix = adcsensor + datafix;
  delay(100);
  
  if(cacah > 10){
  fix = datafix / 10.0;
  kalibrasi = (fix + 1497.5) / 17.707;
  return;
}
  proses();
}   

void sendTelegramMessage() {
  String message = "Blood Sugar Level: " + String(fix) + " mg/dl";
  bot.sendMessage(CHAT_ID, message, "");
}


void setup() {
//  WiFi.begin(ssid, pass);
  Serial.begin(115200);
  lcd.begin();
  pinMode(bt, INPUT_PULLUP);
  pinMode(adcsensor, INPUT);
  

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

}

void loop(){//function untuk membaca hasil dari kalibrasi
  btx = digitalRead(bt);
  Serial.print("button : ");
  Serial.println(btx);

  lcd.setCursor(0, 0);
  lcd.print("KADAR GULA:");
  lcd.setCursor(0, 1);
  lcd.print("mg/dl= ");
  lcd.print(kalibrasi);
  lcd.print("      ");
  // bot.sendMessage(CHAT_ID, "ADC: ", String(fix));

  Serial.print("ini Kalbrasi :" );
  Serial.println(kalibrasi);
  
  if (btx == 0) {
    lcd.clear();
    cacah = 0;
    datafix = 0;
    fix = 0;
    kalibrasi = 0;
    proses();
    sendTelegramMessage(); // Call function to send message to Telegram
  }
  
}
