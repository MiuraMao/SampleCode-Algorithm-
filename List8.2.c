#include <stdio.h>
/*チェイン法によるプログラム*/

/*ハッシュ表の定義*/
#define BUCKET_SIZE 50      /*ハッシュ表の大きさ*/

//適当な型で宣言
typedef int KEY;
typedef int DATA;

typedef struct cell{
    KEY key;
    DATA data;
    struct cell *next;
}CELL;
CELL *table[BUCKET_SIZE];

/*  init -- ハッシュ表を初期化する  */
void init()
{
    int i;

    for (i = 0; i < BUCKET_SIZE; i++)
        table[i] = NULL;
}

/*  find -- ハッシュ表を探索する            */
/*          DATAへのポインタを返す          */
/*          見つからなければ、NULLを返す    */
DATA *find(KEY key)
{
    CELL *p;

    for (p = table[hash(key)]; p != NULL; p = p->next)      //hashは別途実装
        if (keyequal(key, p->key))      //keyequalは別途実装
            return &p->data;
    return NULL;
}

/*  insert -- ハッシュ表にデータを挿入する                     */
/*            登録に成功したら1を返す                         */
/*            登録に失敗（すでに同じキーをもつデータがある）    */
/*            したら０を返す                                */
int insert(KEY key, DATA *data)
{
    CELL *p;
    int h;

    if (find(key) != NULL)
        return 0;
    if ((p = malloc(sizeof(CELL))) == NULL){
        fprintf(stderr, "out of memory\n");
        exit(2);
    }
    h = hash(key);
    p->key = key;
    p->data = *data;
    p->next = table[h];
    table[h] = p;
    return 1;
}

/*  delete -- ハッシュ表から削除する                */
/*            削除が成功したら１を返す              */
/*            データが見つからなければ０を返す      */
int delete(KEY key)
{
    int h;
    CELL *p, *q;

    h = hash(key);
    /*そのバケットは空か？*/
    if (table[h] == NULL)
        return 0;
    /*リストの先頭のセルが削除すべきデータか？*/
    if (keyequal(key, table[h]->key)){
        p = table[h];
        table[h] = p->next;
        free(p);
        return 1;
    }
    /*リストの２番目以降のセルについて順番にチェックする*/
    for (q = table[h], p = q->next; p != NULL; q = p, p = p->next){
        if (keyequal(key, p->key)){
            q->next = p->next;
            free(p);
            return 1;
        }
    }
    return 0;
}
