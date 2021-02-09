#include <stdio.h>
/*分布数え上げソートのプログラム*/

/*整列の対象となるデータの型*/
typedef struct {
    int key;    //整列のキー。0からMの範囲内でなければならない
    int other;  //その他の情報
} DATA;

/*キーは0からMまでの範囲の整数*/
#define M   100

/*キーの分布を数え上げるための配列*/
int count[M+1];

/*大きさnの配列aを分布数え上げソートによって整列する。結果は配列bに得られる*/
void dist_count_sort(DATA a[], DATA b[], int n)
{
    int i;

    /*カウンタをすべて0にする*/
    for (i = 0; i <= M; i++)
        count[i] = 0;
    
    /*キーを数え上げる*/
    for (i = 0; i < n; i++)
        count[a[i].key]++;
    
    /*数え上げたキーの累積度数分布を求める*/
    for (i = 0; i < M; i++)
        count[i+1] += count[i];
    
    /*度数分布に従ってデータを配列aから配列bにコピーする*/
    for (i = n - 1; i >= 0; i--)
        b[--count[a[i].key]] = a[i];
}
