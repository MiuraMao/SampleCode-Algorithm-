#include <stdio.h>
/*連結リストへの挿入*/

struct CELL{
    struct CELL *next;
};

int main(void){
/*ポインタxで指されたセルの直後に新しいセルを挿入する*/
struct CELL *p;
struct CELL *x;

if ((p = malloc(sizeof(struct CELL))) == NULL)
    fatal_error("メモリがたりない");
    //セルに値をセットする
    p->next = x->next;
    x->next = p;

/*リストの先頭に新しいセルを挿入する*/
    struct CELL *header, *p;
    if ((p = malloc(sizeof(struct CELL))) == NULL)
    //セルに値をセットする
    p->next = header;
    header = p;

}