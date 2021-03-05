/*出力ファイル記述子のキャッシング(File Cache)*/

/*
 cache.c    --出力用ファイルをキャッシュする
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int cache_hit = 0;
int cache_missed = 0;

/*ファイル名の長さ*/
#define FILENAME_LEN    64

/*キャッシュされるファイルの数（同時にオープンできるファイルの数以下でなければならない）*/
#define CACHE_SIZE      10

/*キャッシュのエントリを表す型*/
typedef struct cache {
    char filename[FILENAME_LEN+1];  /*ファイル名*/
    FILE *fp;                       /*ファイル記述子（NULLなら、このエントリは未使用である）*/
    struct cache *prev;             /*前の要素を指す*/
    struct cache *next;             /*次の要素を指す*/
} cache_t;

/*キャッシュの実体*/
cache_t     cache[CACHE_SIZE];

/*キャッシュリストへのヘッダ*/
cache_t     cache_list;

/*posの直前にxを挿入する*/
void insert_cache(cache_t *pos, cache_t *x)
{
    pos->prev->next = x;
    x->prev = pos->prev;
    x->next = pos;
    pos->prev = x;
}

/*xを削除する。削除された要素（つまりx）を返す*/
cache_t *remove_cache(cache_t *x)
{
    x->prev->next = x->next;
    x->next->prev = x->prev;
    return x;
}

/*初期化処理を行う。プログラムの開始時に、必ず１回だけ呼び出すこと*/
void init_cache()
{
    int i;

    /*キャッシュをリストに連結しておく*/
    cache_list.next = cache_list.prev = &cache_list;
    for (i = 0; i < CACHE_SIZE; i++){
        cache[i].fp = NULL;     //キャッシュの実体
        insert_cache(&cache_list, &cache[i]);
    }
}

/*終了化処理を行う。プログラムの終了時に、呼び出すこと*/
void end_cahce()
{
    int i;

    /*オープンされているファイルをすべてクローズする*/
    for (i = 0; i <CACHE_SIZE; i++){
        if (cache[i].fp != NULL)
            fclose(cache[i].fp);
    }
}

/*ファイルfilenameをアペンドモードでオープンして、ファイル記述子を返す。
　ファイルが存在しなければ、新規に作成する。ファイルのオープンに失敗したらNULLを返す*/
FILE *open_file(char *filename)
{
    cache_t *p;
    FILE *fp;

    /*すでにそのファイルがオープンされているかチェックする*/
    for (p = cache_list.next; p != &cache_list; p = p->next){
        if (strcmp(filename, p->filename) == 0){

            /*このファイルはすでにオープンされていた。エントリをキャッシュリストの末尾に移動する*/
            printf("Cache hit [%s]\n", filename);
            cache_hit++;

            remove_cache(p);
            insert_cache(&cache_list, p);   //リストの先頭の直前（＝リストの末尾）
            return p->fp;            //ファイルの記述子を返す
        }
    }

    /*リストの先頭のキャッシュエントリを取り出して、リストの末尾に移動する*/
    p = remove_cache(cache_list.next);      //削除する
    insert_cache(&cache_list, p);           //リストの先頭の直前（＝リストの末尾）

    /*元のファイルをクローズする*/
    if (p->fp != NULL){
        printf("Closing [%s]\n", p->filename);
        fclose(p->fp);
        p->fp = NULL;
    }

    /*ファイルをアペンドモードでオープンする*/
    fp = fopen(filename, "a");
    if (fp == NULL)
        return NULL;
    
    /*キャッシュにファイル記述子とファイル名をセットする*/
    printf("Opening [%s]\n", filename);
    p->fp = fp;
    strcpy(p->filename, filename);

    cache_missed++;
    return fp;
}
