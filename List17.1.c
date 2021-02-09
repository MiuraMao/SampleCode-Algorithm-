#include <stdio.h>
/*ビンソートのプログラム*/

/*整列の対象となるデータの型*/
typedef struct {
    int key;        //整列のキー。0からMの範囲内でなければならない
                    //キーが-1のとき、ビンは空である
    int other;
} DATA;

/*キーは0からMまでの範囲の整数*/
#define M   100

/*ビンソートに使用するビン*/
DATA    bin[M+1];

/*大きさnの配列aをビンソートする*/
void bin_sort(DATA a[], int n)
{
    int i, j;

    /*ビンを空にする*/
    for (i = 0; i <= M; i++)
        bin[i].key = -1;

    /*配列aのデータを順番にビンに振り向ける*/
    for (i = 0; i < n; i++)
        bin[a[i].key] = a[i];
    
    /*データをビンから（昇順）に取り出して、配列aに戻す*/
    j = 0;
    for (i = 0; i <= M; i++)
        if (bin[i].key != -1)
            a[j++] = bin[i];
}