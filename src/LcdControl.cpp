#include "Arduino.h"
#include "LcdControl.h"
#include <LiquidCrystal_I2C.h>

// TODO コンストラクタ
LcdControl::LcdControl(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows):
    lcd(lcd_addr, lcd_cols, lcd_rows){}

//  メソッド一覧
void LcdControl::setup(){
    lcd.init();
    lcd.backlight();
}
// 2行表示メソッド
void LcdControl::printOneLineOnLcd(const char * text, int column, int line){
    lcd.clear();
    lcd.setCursor(column, line);
    lcd.print(text);
}
// 1行表示メソッド
void LcdControl::printTowLineOnLcd(const char * text_one, const char * text_tow, int column_one, int line_one, int column_tow, int line_tow){
    lcd.clear();
    lcd.setCursor(column_one, line_one);
    lcd.print(text_one);
    lcd.setCursor(column_tow, line_tow);
    lcd.print(text_tow);
}
// アナログスティック表示専用
void LcdControl::printAnalogPositionOnLcd(const char * text_one_X, const char * text_one_Y, const char * text_tow_X, const char * text_tow_Y){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(text_one_X);
    lcd.setCursor(7,0);
    lcd.print(text_one_Y);
    lcd.setCursor(0,1);
    lcd.print(text_tow_X);
    lcd.setCursor(7,1);
    lcd.print(text_tow_Y);
}