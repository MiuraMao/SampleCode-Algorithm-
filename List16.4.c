//Edit Test
//Delete Test
#include <stdio.h>
/*ヒープソートを行う関数heapsort*/

/*ヒープソートの対象となる配列の大きさ*/
#define MAX_HEAP    1000

/*ヒープソートの対処となる配列。実際に使用するのはa[1]~a[MAX_HEAP]のみで、a[0]は使用しないことに注意*/
int a[MAX_HEAP];

/*配列aに登録されている要素の個数。配列のうちa[1]~a[n]に要素が入っている*/
int n = 0;

/*ヒープの先端の要素を必要なところまで沈める。ただし、a[from]をヒープの先頭、a[to]をヒープの最後の要素とする*/
void downheap(int from, int to)
{
    int i, j;
    int val;

    /*沈められる要素の値をvalにセットしておく*/
    val = a[from];

    /*根から始めて、節iが子をもっている限り繰り返す*/
    i = from;
    while (i <= to/2){

        /*節iの子のうち小さい方を節jとする*/
        j = i * 2;
        if (j+1 <= to && a[j] > a[j+1])
            j++;
        
        /*もし、親が子より大きくないという関係が成り立てば、これ以上沈める必要はない*/
        if (val <= a[j])
            break;
        
        /*節iに節jの値を入れて、節jに注目する*/
        a[i] = a[j];
        i = j;
    }

    /*先頭にあった要素を節iに入れる*/
    a[i] = val;
}

/*配列をヒープソートする。a[1]~a[n]をソートする*/
void heapsort()
{
    int i;
    int tmp;

    for (i = n/2; i >= 1; i--)
        downheap(i, n);
    
    for(i = n; i >= 2; i--){
        tmp = a[1];
        a[1] = a[i];
        a[i] = tmp;
        downheap(1, i-1);
    }

}
