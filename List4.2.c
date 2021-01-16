/*
* queue.c   配列で待ち行列を実現する
*/

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>

typedef long    ELEM;           //待ち行列の要素の型

# define QUEUE_SIZE 100         //待ち行列の大きさ

ELEM    queue [QUEUE_SIZE];     //待ち行列の手k魏
int     front;                  //待ち行列の先頭
int     rear;                   //待ち行列の末尾（正確に言うと末尾の次の要素を指す）

#define next(a) (((a) + 1 ) % QUEUE_SIZE)   //次の要素の添え字を求める

//エラーメッセージをプリントしてexitする
void error(char *s)
{
    fprintf(stderr, s);
    exit(1);
}

//待ち行列を初期化する
void init()
{
    front = rear = 0;
}

//待ち行列にデータを入れる
void enqueue(ELEM x)
{
    if (next(rear) == front)
        error("待ち行列がフルなので要素を入れられません\n");
    queue[rear] = x;
    rear = next(rear);
}

//待ち行列からデータを取り出す
ELEM dequeue()  //main
{
    ELEM x;
    if(front == rear)
        error("待ち行列が空なので要素を取り出せません\n");
    x = queue[front];
    front = next(front);
    return x;
}

//待ち行列が空かどうかを調べる
//空なら1、空でなければ0を返す
int empty()
{
    return front == rear;
}