#include <stdio.h>
/*二分探索木の全要素を昇順に表示する関数inorder*/

typedef struct node {
    KEY data;
    struct node *left, *right;
}NODE;
//適当な型で宣言
typedef int KEY;

inorder(NODE *p)
{
    if (p = NULL)
        return;
    else{
        inorder(p->left);
        printf("%d", p->data);
        inorder(p->right);
    }
}