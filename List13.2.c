#include <stdio.h>
/*シェルソート*/
//減少する増分によるソート
//安定な整列アルゴリズムでない
// h = ..., 121, 40, 13, 4, 1 の数列を使用

void shell_sort(int a[], int n)
{
    int h, i, j, t;

    for (h = 1; h < n/9; h = h * 3 + 1)
        ;
    for (; h > 0; h /= 3){
        for (i = h; i < n; i ++){
            j = i;
            while (j >= h && a[j-h] > a[j]){
                t = a[j]; a[j] = a[j-h]; a[j-h] = t;
            }
        }
    }
}