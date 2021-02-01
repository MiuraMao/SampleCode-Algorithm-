#include <stdio.h>
/*挿入ソート*/
//安定な整列アルゴリズム
//ほとんど整列されている場合適している
//（整列済みの配列の後ろに要素をいくつか追加し、再び整列させるなど）


void insertion_sort(int a[], int n)
{
    int i, j ,t;

    for (i = 1; i < n; i++){
        j = 1;
        while (j >= 1 && a[j-1] > a[j]){
            t = a[j]; a[j] = a[j-1]; a[j-1] = t;
            j--;
        }
    }

}