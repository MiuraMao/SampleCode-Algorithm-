#include <stdio.h>
/*二分探索木に要素を挿入する関数insert*/

typedef struct node {
    KEY data;
    struct node *left, *right;
}NODE;
//適当な型で宣言
typedef int KEY;
//グローバル変数rootを初期化
NODE *root = NULL;

NODE *insert(KEY key)
{
    NODE **p, *new;

    p = &root;
    while (*p !=NULL){
        if (keyequal(key, (*p)->data))
            return NULL;
        else if (keylt(key, (*p)->data))
            p = &(*p)->left;
        else
            p = &(*p)->right;
    }

    if ((new = malloc(sizeof(NODE))) == NULL)
        error("out of memory!");
    new->left = NULL;
    new->right = NULL;
    new->data = key;
    *p = new;                           //正しい場所に挿入する
    return new;
}
