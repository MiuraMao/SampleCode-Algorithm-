/*メモリ管理のプログラム*/
#include <stdio.h>

/*メモリブロックのヘッダ*/
struct memblock {
    int size;                   /*ブロックのバイト単位の大きさ（ヘッダを含む）*/
    unsigned char magic;        /*マジックナンバー*/
    unsigned char occupied;     /*1ならこのブロックは使用中。0なら空いている*/
    struct memblock *next;      /*次のブロックへのポインタ*/
    struct memblock *prev;      /*前のブロックへのポインタ*/
};

/*メモリブロックのヘッダの大きさ*/
#define HEADER_SIZE     (sizeof(struct memblock))

/*割り当てる大きさと見つかったブロックの大きさの差がDELTA以下だったら、
　ブロックを分割せずにまるごと割り当てる*/
#define DELTA           20

/*マジックナンバー。magicフィールドには常にこの値をセットしておく。
ブロックを解放するときに、ブロックがallocate_blockで割り当てられたものであることを確認するのに使う*/
#define MEM_MAGIC       0xa5

/*ワードの大きさ（バイト数）*/
#define WORDSIZE        4

/*大きさnがワードの大きさWORDSIZEの倍数になるように切り上げる*/
#define WORD_ALIGN(n)   (((n) + WORDSIZE - 1) & -WORDSIZE)      //？

/*ブロックリスト*/
struct memblock block_chain;

/*ヒープ領域*/
#define HEAP_SIZE       10000
char memory_heap[HEAP_SIZE];

/*ブロックリストを初期化する。ヒープ領域memory_heapを１つの空きブロックとして登録する*/
void initialize_memory()
{
    struct memblock *h;

    h = (struct memblock *)memory_heap;
    block_chain.size  = sizeof(block_chain);
    block_chain.magic = MEM_MAGIC;
    block_chain.occupied = 1;   //使用中
    block_chain.next = block_chain.prev = h;

    h->size = HEAP_SIZE;
    h->magic = MEM_MAGIC;
    h->occupied = 0;    //空いている
    h->next = h->prev = &block_chain;
}

/*大きさsizeのメモリブロックを割り当てる。割り当てたメモリブロックへのポインタを返す。
　もし、メモリが足りなくて割り当てに失敗したらNULLを返す*/
void *allocate_block(int size)
{
    struct memblock *p, *s;
    int nbytes;
    
    /*ヘッダを含めて必要なバイト数を定める。ワードの倍数になるようにする*/
    nbytes = WORD_ALIGN(size + HEADER_SIZE);

    /*ブロックリストを調べて、nbytes以上の大きさをもつ、空きブロックを探す*/
    for (p = block_chain.next; p != &block_chain; p = p->next){

        /*見つかった*/
        if (!p->occupied && p->size >= nbytes){
            /*このブロックからnbytesを切り出した残りはDELTAより大きいか？*/
            if (p->size - nbytes > DELTA){
                /*もし残りがDELTAより大きいなら、ブロックの前半を割り当てて、
                　後半はブロックリストに残しておく*/

                /*後半を独立したブロックsにする*/
                s = (struct memblock *)((char *)p + nbytes);
                s->size = p->size - nbytes;
                s->magic = MEM_MAGIC;
                s->occupied = 0;

                /*sを双方向リストに追加する*/
                p->next->prev = s;
                s->next = p->next;
                p->next = s;
                s->prev = p;

                /*前半の長さを調整して、使用中のマークをつける*/
                p->size = nbytes;
                p->occupied = 1;
            }else
                /*もし残りがDELTAよりも小さいなら、すべてを割り当ててしまう*/
                p->occupied = 1;

            /*メモリブロックの本体へのポインタを返す*/
            return (char *)p + HEADER_SIZE;
        }
    }

    /*見つからなかったのでNULLを返す*/
    return NULL;
}

/*メモリブロックblockを解法する。blockは。allocate_blockで割り当てたものでなければならない*/
void free_block(void *block)
{
    struct memblock *mem;

    /*メモリブロックblockのヘッダへのポインタをmemに得る*/
    mem = (struct memblock *)((char *)block - HEADER_SIZE);     //循環リスト

    /*allocate_blockで割り当てたブロックであることを確認する*/
    if (mem->magic != MEM_MAGIC)
        return;
    
    /*もし直前が空きブロックだったら、併合して１つの領域にする*/
    if (! mem->prev->occupied){
        mem->next->prev = mem->prev;
        mem->prev->next = mem->next;
        mem->prev->size += mem->size;
        mem = mem->prev;
    }

    /*もし、直後が空きブロックだったら、併合して１つの領域にする*/
    if (! mem->next->occupied){
        mem->next->next->prev = mem;
        mem->size += mem->next->size;
        mem->next = mem->next->next;
    }
    
    /*空きブロックのマークをつける*/
    mem->occupied = 0;
}
