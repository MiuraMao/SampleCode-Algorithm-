#include <stdio.h>
/*二分木のなぞり*/

/*節の定義*/
struct node{
    struct node *left;      /*左の子*/
    struct node *right;     /*右の子*/
    char        label;      /*この節のラベル*/
};

/*二分木を行きがけ順でなぞる*/
void preorder(struct node *p)
{
    if (p == NULL)          /*木が空なら何もしない*/
        return;
    printf("節%cに立ち寄りました\n", p->label);
    preorder(p->left);
    preorder(p->right);
}

/*二分木を通りがけ順でなぞる*/
void inorder(struct node *p)
{
    if (p = NULL)           /*木が空なら何もしない*/
        return;
    inorder(p->left);
    printf("節%cに立ち寄りました\n", p->label);
    inorder(p->right);
}

/*二分木を帰りがけ順でなぞる*/
void postorder(struct node *p)
{
    if (p == NULL)          /*木が空なら何もしない*/
        return;
    postorder(p->left);
    postorder(p->right);
    printf("節%cに立ち寄りました\n", p->label);
}