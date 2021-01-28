#include <stdio.h>
/*二分探索木から最小の要素を削除する関数deletemin*/

typedef struct node {
    KEY data;
    struct node *left, *right;
}NODE;
//適当な型で宣言
typedef int KEY;

//二分探索木から最小の要素を削除し、削除した節へのポインタを返す
NODE *deletemin(NODE **p)
{
    NODE *x;

    while ((*p)->left != NULL)
        p = &(*p)->left;
    x = *p;
    *p = (*p)->right;
    return x;

}