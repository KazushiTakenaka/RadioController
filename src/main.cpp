#include <Arduino.h>
// #include <ArduinoJson.h>
// #include <FS.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
#include <string>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Conversion.h"
#include "LcdControl.h"
#include "Secret.h"

// String MACadd = "94:B5:55:85:A5:3E";
// uint8_t address[6]  = {0x94, 0xB5, 0x55, 0x85, 0xA5, 0x3E};

// String MACadd;
// uint8_t address[6];

bool connected;
const int X_PIN_1 = 25;
const int Y_PIN_1 = 26;
const int SW_PIN_1 = 27;
const int X_PIN_2 = 4;
const int Y_PIN_2 = 0;
const int SW_PIN_2 = 14;
const int A_BUTTON_PIN = 12;
const int B_BUTTON_PIN = 13;
const int X_BUTTON_PIN = 16;
const int Y_BUTTON_PIN = 17;

void readJson();

LcdControl lcd(0x27,16,2);
const char * text_one_X = "";
std::string text_one_X_str = "";
const char * text_one_Y = "";
std::string text_one_Y_str = "";
const char * text_one_Z = "";
std::string text_one_Z_str = "";
const char * text_tow_X = "";
std::string text_tow_X_str = "";
const char * text_tow_Y = "";
std::string text_tow_Y_str = "";
const char * text_tow_Z = "";
std::string text_tow_Z_str = "";
void task1(void *pvParameters) {
  while (1) {
    lcd.printAnalogPositionOnLcd(text_one_X, text_one_Y, text_tow_X, text_tow_Y);
    delay(50);
  }
}
// LiquidCrystal_I2C lcd(0x27,16,2); // lcd初期設定

void setup() {
  Serial.begin(115200);
  String MACadd = MAC_ADDRESS_STR;
  uint8_t address[6] = {MAC_ADDRESS_BYTE[0], MAC_ADDRESS_BYTE[1], MAC_ADDRESS_BYTE[2], MAC_ADDRESS_BYTE[3], MAC_ADDRESS_BYTE[4], MAC_ADDRESS_BYTE[5]};
  // if(!SPIFFS.begin(true)){
  //   Serial.println("An Error has occurred while mounting SPIFFS");
  //   return;
  // }
  // JsonDocument doc;
  // readJson(SPIFFS, "/secret.json", doc);
  // JsonArray addressArray = doc["address"].as<JsonArray>();
  // for (int i = 0; i < 6; i++) {
  //   address[i] = (uint8_t)addressArray[i].as<int>();
  // }

  // Wire.begin(SDAピン番号, SCLピン番号);
  Wire.begin(); // I2C通信するときにいるやつ
  lcd.setup();
  // lcd.printOneLineOnLcd("hello");
  lcd.printTowLineOnLcd("Hello TENKI","Try Connect");
  uint8_t macBT[6]; // macアドレス用設定
  esp_read_mac(macBT, ESP_MAC_BT);// macアドレス
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]); // macアドレス表示
  // Bluetoothのローカルデバイス名の設定第2引数でマスター側の設定trueでマスター(送受信デバイスで同じ名前にする必要あり？)
  SerialBT.begin("ESP32", true); 
  Serial.println("device start");
  connected = SerialBT.connect(address);
  if(connected) {
    Serial.println("Connect OK");
    lcd.printTowLineOnLcd("Connect OK");
  } else {
    while(!SerialBT.connected(1500)) {
      connected = SerialBT.connect(address);
      Serial.println("No connect"); 
      lcd.printTowLineOnLcd("No connect","Wait or reboot.");
    }
  }
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  SerialBT.connect();

  xTaskCreateUniversal(
    task1,
    "task1",
    8192,
    NULL,
    1,
    NULL,
    APP_CPU_NUM
  );

  pinMode(X_PIN_1, INPUT);          // 25ピンを入力
  pinMode(Y_PIN_1, INPUT);          // 26ピンを入力
  pinMode(SW_PIN_1, INPUT_PULLUP); // 27ピン
  pinMode(X_PIN_2, INPUT);
  pinMode(Y_PIN_2, INPUT);
  pinMode(SW_PIN_2, INPUT_PULLUP);
  pinMode(A_BUTTON_PIN, INPUT_PULLUP);
  pinMode(B_BUTTON_PIN, INPUT_PULLUP);
  pinMode(X_BUTTON_PIN, INPUT_PULLUP);
  pinMode(Y_BUTTON_PIN, INPUT_PULLUP);
}

int X_POS_1;
int Y_POS_1;
int SW_POS_1;
int X_POS_2;
int Y_POS_2;
int SW_POS_2;
int A_BUTTON_POS;
int B_BUTTON_POS;
int X_BUTTON_POS;
int Y_BUTTON_POS;
// クラスインスタンス生成
ConversionClass conversion_obj;
int data_box; // 受信変数
int conversion_value_X1;
int conversion_value_Y1;
int conversion_value_X2;
int conversion_value_Y2;
int communication_count; // 通信途切れ確認用

// const char * text_one;
// std::string text_one_str;
// const char * text_tow;
// std::string text_tow_str;
int i = 0;
// std::string text_one;
// TODO 構造体の生成()
struct SendAnalogkData
  {
    int X1;
    int Y1;
    int SW1;
    int X2;
    int Y2;
    int SW2;
    int A_BTN;
    int B_BTN;
    int X_BTN;
    int Y_BTN;
  };

  

void loop() {
  // put your main code here, to run repeatedly:
  X_POS_1 = analogRead(X_PIN_1);     // X軸方向のアナログ値を読み取る
  Y_POS_1 = analogRead(Y_PIN_1);     // Y軸方向のアナログ値を読み取る
  SW_POS_1 = digitalRead(SW_PIN_1);  // センタースイッチの状態を読み取る
  X_POS_2 = analogRead(X_PIN_2);     // X軸方向のアナログ値を読み取る
  Y_POS_2 = analogRead(Y_PIN_2);     // Y軸方向のアナログ値を読み取る
  SW_POS_2 = digitalRead(SW_PIN_2);  // センタースイッチの状態を読み取る
  A_BUTTON_POS = digitalRead(A_BUTTON_PIN);  //Aボタン読み取り
  B_BUTTON_POS = digitalRead(B_BUTTON_PIN);  //Bボタン読み取り
  X_BUTTON_POS = digitalRead(X_BUTTON_PIN);  //Xボタン読み取り
  Y_BUTTON_POS = digitalRead(Y_BUTTON_PIN);  //Yボタン読み取り
  // メソッドを呼び出して値を変換
  conversion_value_X1 = conversion_obj.conversion(X_POS_1);
  conversion_value_Y1 = conversion_obj.conversion(Y_POS_1);
  conversion_value_X2 = conversion_obj.conversion(X_POS_2);
  conversion_value_Y2 = conversion_obj.conversion(Y_POS_2);
  // TODO 構造体の生成
  SendAnalogkData sendData;
  sendData.X1 = conversion_value_X1;
  sendData.Y1 = conversion_value_Y1;
  sendData.SW1 = SW_POS_1;
  sendData.X2 = conversion_value_X2;
  sendData.Y2 = conversion_value_Y2;
  sendData.SW2 = SW_POS_2;
  sendData.A_BTN = A_BUTTON_POS;
  sendData.B_BTN = B_BUTTON_POS;
  sendData.X_BTN = X_BUTTON_POS;
  sendData.Y_BTN = Y_BUTTON_POS;

  Serial.print("X1: ");           // シリアルモニタにそれぞれの値を出力
  Serial.print(conversion_value_X1);
  Serial.print("  Y1: ");
  Serial.print(conversion_value_Y1);
  Serial.print("  SW1: ");
  Serial.print(SW_POS_1);
  Serial.print("  X2: ");
  Serial.print(conversion_value_X2);
  Serial.print("  Y2: ");
  Serial.print(conversion_value_Y2);
  Serial.print("  SW2: ");
  Serial.print(SW_POS_2);
  Serial.print("  A_BTN: ");
  Serial.print(A_BUTTON_POS);
  Serial.print("  B_BTN: ");
  Serial.print(B_BUTTON_POS);
  Serial.print("  X_BTN: ");
  Serial.print(X_BUTTON_POS);
  Serial.print("  Y_BTN: ");
  Serial.println(Y_BUTTON_POS);
  // LCDプリント文字(一旦stringにしてからchar)
  text_one_X_str = std::string("X1=") + std::to_string(conversion_value_X1);
  text_tow_X_str = std::string("X2=") + std::to_string(conversion_value_X2);
  text_one_Y_str = std::string("Y1=" + std::to_string(conversion_value_Y1));
  text_tow_Y_str = std::string("Y2=" + std::to_string(conversion_value_Y2));
  text_one_Z_str = std::string("Z1=" + std::to_string(SW_POS_1));
  text_tow_Z_str = std::string("Z2=" + std::to_string(SW_POS_2));
  text_one_X = text_one_X_str.c_str();
  text_tow_X = text_tow_X_str.c_str();
  text_one_Y = text_one_Y_str.c_str();
  text_tow_Y = text_tow_Y_str.c_str();
  text_one_Z = text_one_Z_str.c_str();
  text_tow_Z = text_tow_Z_str.c_str();
  // lcd.printTowLineOnLcd(text_one, text_tow);
  // if(i==100){
  //   lcd.printTowLineOnLcd(text_one, text_tow);
  //   i = 0;
  // }
  // i++;
  // Bluetooth送信
  // SerialBT.println('\n' + send_str); // 文字列で送信するとき
  SerialBT.write((uint8_t*)&sendData, sizeof(sendData));
  // データが送られてきたら
  if (SerialBT.available()) {
    data_box = SerialBT.read();
    Serial.println(data_box);
    communication_count = 0;
  }else{
    communication_count++;
    if (communication_count >= 100){
      ESP.restart();
    }
    
  }

  delay(50);
}

// void readJson(fs::FS &fs, const char *path, JsonDocument &doc) {
//   File file = fs.open(path);
//   if (!file || file.isDirectory()) {
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   DeserializationError error = deserializeJson(doc, file);
//   if (error) {
//     Serial.println("Failed to parse JSON");
//   }

//   file.close();
// }