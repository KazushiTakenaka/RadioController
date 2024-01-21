#ifndef LCDCONTROL_H
#define LCDCONTROL_H
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

// TODO クラスの宣言パブリックで使用できるようにしている？
class LcdControl{
    // int myData; // public data グローバル変数的なやつ？も作れる外部から呼び出し＆代入可能らしい
    // protected（保護）、private（非公開）もある使い方はまだわからん
    // ベースクラスコンストラクタ(継承してる？)

    // public:
    //     LcdControl(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows):
    //         lcd(lcd_addr, lcd_cols, lcd_rows){}
    //     void printOnLcd(const char * text, int column, int line){
    //         lcd.clear();
    //         lcd.setCursor(column,line);
    //         lcd.print(text);
    //     }
    // ↓コンストラクタ
    public:
        LcdControl(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    private:
        LiquidCrystal_I2C lcd;
    public:
        void setup();
    public:
        void printOneLineOnLcd(const char * text, int column = 0, int line = 0);
    public:
        void printTowLineOnLcd(const char * text_one_line, const char * text_tow_line = "", int column_one = 0, int line_one = 0, int column_tow = 0, int line_tow = 1);
    public:
        void printAnalogPositionOnLcd(const char * text_one_X, const char * text_one_Y, const char * text_tow_X, const char * text_tow_Y);
};
#endif