#include <stdio.h>
/*二分探索木から要素を削除する関数delete*/

typedef struct node {
    KEY data;
    struct node *left, *right;
}NODE;
//適当な型で宣言
typedef int KEY;
//グローバル変数rootを初期化
NODE *root = NULL;

int delete(KEY key)
{
    NODE **p, *x;

    p = &root;
    while (*p != NULL){
        if (keyequal(key, (*p)->data)){         //削除対象が見つかった
            x = *p;
            if (x->left == NULL && x->right == NULL)    //葉である
                *p = NULL;
            else if (x->left == NULL)
                *p = x->right;
            else if (x->right == NULL)
                *p = x->left;
            else {
                *p = deletemin(&x->right);      //関数deleteminは別途実装
                (*p)->right = x->right;
                (*p)->left = x->left;
            }
            free(x);
            return 1;
        }else if (keylt(key, (*p)->data))       //親の方が大きい
            p = &(*p)->left;
        else 
            p = &(*p)->right;
    }
    return 0;
}