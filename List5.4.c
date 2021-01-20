# include <stdio.h>
/*循環リストをたどる*/

struct CELL{
    struct CELL *next;
    int         value;
};

int main (void){
/*ループの末尾で終了条件を判定*/
struct CELL *ptr, *p;

if (ptr != NULL)
    p = ptr;
    do {
        /*pが指すセルを処理する*/
        p = p->next;
    }while(p != ptr);

//間違った例
//ループの先頭で終了条件を判定している
//whileループの条件が成立しない
if (ptr != NULL)
    p = ptr;
    while (p != ptr){
        p = p->next;
    }
}