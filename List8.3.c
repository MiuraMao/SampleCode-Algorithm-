#include <stdio.h>
/*オープンアドレス法によるプログラム*/

#define EMPTY   (KEY)0      /*空であることを示すキーの値*/
#define DELETED (KEY)1      /*削除済みであることを示すキーの値*/

/*ハッシュ表の定義*/
#define BUCKET_SIZE 70      /*ハッシュ表の大きさ*/
//適当な型で宣言
typedef int KEY;
typedef int DATA;

typedef struct bucket{
    KEY key;
    DATA data;
} BUCKET;
BUCKET  table[BUCKET_SIZE];

/*  rehash -- 再ハッシュを行う*/
int rehash(int h)
{
    return (h + 1) % BUCKET_SIZE;
}

/*  init -- ハッシュ表を初期化する*/
void init()
{
    int i;

    for (i = 0; i < BUCKET_SIZE; i++)
        table[i].key = EMPTY;   
}

/*   find -- ハッシュ表を探索する           */
/*           DATA型へのポインタを返す       */
/*           見つからなければ、NULLを返す   */
DATA    *find(KEY key)
{
    int h, count;
    KEY k;

    count = 0;
    h = hash(key);
    while ((k = table[h].key) != EMPTY){
        if (k != DELETED && keyequal(key, k))
            return &table[h].data;
        if (++count > BUCKET_SIZE)
            return NULL;
        h = rehash(h);
    }
    return NULL;
}

/*  insert -- ハッシュ表にデータを挿入する                    */
/*            登録に成功したら１を返す                        */
/*            登録に失敗（すでに同じキーを持つデータがある）    */
/*            したら０を返す                                 */
int insert(KEY  key, DATA  *data)
{
    int h, count;
    KEY k;

    count = 0;
    h = hash(key);
    while ((k = table[h].key) != EMPTY && k != DELETED){
        if (keyequal(key, k))
            return 0;
        if (++count > BUCKET_SIZE){
            fprintf(stderr, "hash table overflow\n");       //標準エラー出力stderr
            exit(1);
        }
        h = rehash(h);
    }
    table[h].key = key;
    table[h].data = *data;
    return 1;
}

/*  delete -- ハッシュ表から削除する            */
/*            削除が成功したら１を返す          */
/*            データが見つからなければ０を返す  */
int delete(KEY key)
{
    int h, count;
    KEY k;

    count = 0;
    h = hash(key);
    while ((k = table[h].key) != EMPTY){
        if (k != DELETED && keyequal(key, k)){
            table[h].key = DELETED;
            return 1;
        }
        if (++count > BUFSIZ)
            return 0;
        h = rehash(h);
    }
    return 0;
}