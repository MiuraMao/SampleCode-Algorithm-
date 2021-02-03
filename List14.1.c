#include <stdio.h>
/*再帰版クイックソート*/
//安定な整列アルゴリズムでない

/*配列a[l]~a[r]を分割する。枢軸の添え字を返す*/
int partition(int a[], int l, int r)
{
    int i, j, pivot, t;

    /*ポインタiとjを初期化する*/
    i = l -1;
    j = r;
    /*いちばん右端の要素を枢軸にする*/
    pivot = a[r];
    /*ポインタiとjがぶつかるまで繰り返す*/
    for (;;){
        /*ポインタiを右へ進める*/
        while (a[++i] < pivot)
            ;
        /*ポインタjを左へ進める*/
        while (i < j-- && pivot < a[j])
            ;
        /*ポインタiとjがぶつかったらループを抜ける*/
        if (i >= j)
            break;
        /*iの指す要素とjの指す要素を交換する*/
        t = a[i]; a[i] = a[j]; a[j] = t;
    }
    /*a[i]と枢軸を交換する*/
    t = a[i]; a[i] = a[r]; a[r] = t;
    return i;
}

/*実際にクイックソートを行う関数*/
void quick_sort_l(int a[], int l, int r)
{
    int v;
    /*整列する要素が１つなら、何もしないでリターンする*/
    if (l > r)
        return;
    
    /*枢軸vを基準に分割する*/
    v = partition(a, l, r);

    /*左の部分配列a[l]~a[v-1]を整列する*/
    quick_sort_l(a, l, v-1);

    /*右の部分配列a[v+1]~a[r]を整列する*/
    quick_sort_l(a, v+1, r);
}

/*クイックソート*/
void quick_sort(int a[], int n)
{
    quick_sort_l(a, 0, n - 1);
}
