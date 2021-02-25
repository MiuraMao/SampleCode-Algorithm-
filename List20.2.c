/*nクイーン（n-Queen）（最初に発見した解のみを表示）*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS     1   /*成功*/
#define FAIL        0   /*失敗*/

#define FREE        1   /*この場所は利き筋になっていない（置ける）*/
#define NOT_FREE    0   /*この場所は利き筋になっている（置けない）*/

#define N           8   /*クイーンの数*/

/*各行に置かれたクイーンの位置*/
int pos[N];

/*クイーンが垂直方向に利いているかを示す配列*/
int col[N];

/*クイーンが右斜め下向きに利いているかを示す配列*/
int down[2 * N - 1];

/*クイーンが右斜め上向きに利いているかを示す配列*/
int up[2 * N - 1];

/*クイーンの位置と利き筋を初期化する*/
void init_board()
{
    int i;

    for (i = 0; i < N; i++)
        pos[i] = -1;
    for (i = 0; i < N; i++)
        col[i] = FREE;
    for (i = 0; i < 2 * N - 1; i++)
        down[i] = FREE;
    for (i = 0; i < 2 * N - 1; i++)
        up[i] = FREE;
}

/*クイーンの位置を出力する*/
void print_queens()
{
    int i, j;

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (pos[i] == j)
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

/*a行目以降すべての行にクイーンを置いてみる*/
int try(int a)
{
    int b;

    /*左から右に向って順番にクイーンが置けるかどうかを調べる*/
    for (b = 0; b < N; b++){
        
        /*a行目のb番目に置けるかどうか調べる*/
        if (col[b] == FREE && up[a + b] == FREE && down[a - b + (N-1)] == FREE){

            /*置くことができた。場所を記録して、利き筋をセットする*/
            pos[a] = b;
            col[b] = NOT_FREE;
            up[a + b] = NOT_FREE;
            down[a - b + (N-1)] = NOT_FREE;

            /*N個のクイーンをすべて置くことができれば成功である*/
            if (a + 1 >= N)
                return SUCCESS;
            else{
                /*a+1行目以降のすべての行に置けたら成功である*/
                if (try(a+1) == SUCCESS)
                    return SUCCESS;
                else{
                    /*失敗した。クイーンを取り除く*/
                    pos[a] = -1;
                    col[b] = FREE;
                    up[a + b] = FREE;
                    down[a- b + (N-1)] = FREE;
                
                }
            }
        }
    }
    /*結局この行には置ける場所がなかった*/
    return FAIL;
}

main()
{
    init_board();

    if (try(0) == SUCCESS)
        print_queens();
    else
        printf("Sorry, but there is no solution\n");
}
