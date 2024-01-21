#ifndef CONVERSION_H
#define CONVERSION_H
// TODO クラスの宣言パブリックで使用できるようにしている？
class ConversionClass{
    // int myData; // public data グローバル変数的なやつ？も作れる外部から呼び出し＆代入可能らしい
    public:
        int conversion(int analog_read_num);
        // protected（保護）、private（非公開）もある使い方はまだわからん
};
#endif