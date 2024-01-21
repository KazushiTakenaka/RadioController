#include "Arduino.h"
#include "Conversion.h"

// TODO メソッドの実行内容を書き込む
int ConversionClass::conversion(int analog_read_num){
    // if (analog_read_num <= 2600){
    // analog_read_num = analog_read_num / 2;
    // }
    int outputValue = 0;
    const int inputMax = 4095;
    const int inputMin = 0;
    const int outputMax = 255;
    const int outputMin = 0;
    outputValue = (int)(((double)(analog_read_num - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) + outputMin);
    return outputValue;
}