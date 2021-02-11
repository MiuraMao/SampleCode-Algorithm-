#include <stdio.h>
/*基数ソートのプログラム*/
//機構上はビンソートだが、プログラムとしては分布数え上げソートを利用する

/*キーの分布を数え上げるための配列*/
#define M       15
#define MASK    0x0f
int count[M+1];

/*大きさnの配列aを基数ソートする。作業用に配列bを使用する
　16ビット長の値（unsigned short）を、4ビットずつ4回にわたって
　分布数え上げソートを適用する*/
void radix_sort(unsigned short a[], unsigned short b[], int n)
{
    int i, bit;

    /*下位から上位に向かって、4ビットずつ4回ループを実行する*/
    for (bit = 0; bit < 16; bit += 4){

        /*カウンタをすべて0にする*/
        for (i = 0; i <= M; i++)
            count[i] = 0;
        
        /*キーを数え上げる*/
        for (i = 0; i < n; i++)
            count[(a[i]>>bit) & MASK]++;        //右にシフト演算
        
        /*数え上げたキーの累積度数分布を求める*/
        for (i = 0; i < M; i++)
            count[i+1] += count[i];
        
        /*度数分布に従ってデータを配列aから配列bにコピーする*/
        for (i = n - 1; i >= 0; i--)
            b[--count[(a[i]>>bit) & MASK]] = a[i];

        /*データを作業用配列bから配列aにコピーする*/
        for (i = 0; i < n; i++)
            a[i] = b[i];
    }
}