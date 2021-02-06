#include <stdio.h>
/*連結リスト版マージソート*/
//クイックソートでもできるが、配列のようにランダムアクセスができないので、マージソートの方が良い

/*連結リストのセル*/
struct node {
    int value;              /*整列のキーとなる値*/
    struct node *next;      /*次のセルへのポインタ*/
};

/*２つの連結リストaとbをマージする。マージされた連結リストの先頭要素へのポインタを返す*/
struct node *merge_list(struct node *a, struct node *b)
{
    struct node head, *p;

    /*ポインタpがダミーの要素を指すようにしておく*/
    p = &head;

    /*連結リストa,bのいずれかが空になるまで繰り返す*/
    while (a != NULL && b != NULL){

        /*連結リストa,bの先頭の要素を比較する*/
        if (a->value <= b->value){
            /*連結リストaの先頭の要素を取り除いて、マージ済み連結リストの末尾に連結する*/
            p->next = a;
            p = a;
            a = a->next;
        }else{
            /*連結リストbの先頭の要素を取り除いて、マージ済み連結リストの末尾に連結する*/
            p->next = b;
            p = b;
            b = b->next;
        }
    }

    /*残っている要素をマージ済み連結リストの最後尾に連結する*/
    if (a == NULL)
        p->next = b;
    else
        p->next = a;
    
    /*マージ済みの連結リストを関数値として返す*/
        return head.next;
}

/*連結リスト版のマージソート*/
/*連結リストxを整列する。整列された連結リストの先頭要素へのポインタを返す*/
struct node *merge_sort_list(struct node *x)
{
    struct node *a, *b, *p;

    /*連結リストに要素が全くないか、１つしか無い時はそのままリターンする*/
    if (x == NULL || x->next == NULL)
        return x;
    
    /*連結リストをスキャンするポインタを初期化する*/

    /*aは１番目の要素を指す*/
    a = x;

    /*bは３番目の要素（もし連結リストの長さが２のときは２番目の要素）を指す*/
    b = x->next;
    if (b!=NULL)
        b = b->next;
    
    /*ポインタbが連結リストの末尾に到達するまで、ポインタaを１つ進め、ポインタbを２つ進める*/
    /*ポインタbが末尾に到着したとき、ポインタaは、連結リストのほぼ中央の要素を指しているはずである*/
    while (b != NULL){
        a = a->next;
        b = b->next;
        if (b != NULL)
            b = b->next;
    }

    /*連結リストを、ポインタaが指す要素の直後で２つに切断する*/
    p = a->next; a->next = NULL;

    /*切断した連結リストを個別に整列して、その結果をマージする*/
    return merge_list(merge_sort_list(x), merge_sort_list(p));

}