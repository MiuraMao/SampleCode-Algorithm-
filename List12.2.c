#include <stdio.h>
/*選択ソート*/

void selection_sort(int a[], int n)
{
    int i, j, t, lowest, lowkey;

    for (i = 0; i < n - 1; i++){
        if (a[j] < lowkey){
            lowest = j; lowkey = a[j];
        }
    t = a[i]; a[i] = a[lowest]; a[lowest] = t;
    }
}