#include <stdio.h>
/*配列版マージソート*/

/*作業用領域の大きさ。この個数までの要素を整列することができる*/
#define MAX_ELEMENTS    10000

/*マージソートに使う作業用領域*/
int     b[MAX_ELEMENTS];

/*配列版のマージソート   a[low]~a[high]の要素を整列する*/
void merge_sort_array(int a[], int low, int high)
{
    int mid, i, j, k;

    /*要素が１つしかなければ、即時にリターンする*/
    if (low >= high)
        return;
    
    /*列を２つに分割する場所midを決める*/
    mid = (low + high) / 2;

    /*前半の要素a[low]~a[mid]を整列する*/
    merge_sort_array(a, low, mid);

    /*後半の要素a[mid+1]~a[high]を整列する*/
    merge_sort_array(a, mid+1, high);

    /*前半の要素をそのまま作業用配列bにコピーする*/
    for (i = low; i <= mid; i++)
        b[i] = a[i];
    
    /*後半の要素を逆順に作業用配列bにコピーする*/
    for (i = mid+1, j = high; i <= high; i++, j--)
        b[i] = a[i];
    
    /*作業用配列bの両端から取り出したデータをマージして配列aに入れる*/
    i = low; j = high;
    for (k = low; k <= high; k++)
        if (b[i] <= b[j])
            a[k] = b[i++];
        else
            a[k] = b[j--];
}
