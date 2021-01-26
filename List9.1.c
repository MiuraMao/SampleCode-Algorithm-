#include <stdio.h>
/*二分探索木を探索する関数serach*/

typedef struct node {
    KEY data;
    struct node *left, *right;
}NODE;

//適当な型で宣言
typedef int KEY;

//グローバル変数rootを初期化
NODE *root = NULL; 

//以下の関数が用意されているものとする
//int keyequal(KEY a, KEY b);
//int keylt(KEY a, KEY b);

NODE *serch(KEY key)
{
    NODE *p;

    p = root;                           //根に注目する
    while (p != NULL){
        if (keyequal(key, p->data))     //等しければ１
            return p;
        else if (keylt(key, p->data))   //a < bであれば１、そうでなければ０
            p = p->left;
        else
            p = p->right;
    }
    return NULL;                        //探索に失敗した
}