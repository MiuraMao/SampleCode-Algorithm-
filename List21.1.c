/*
    ナップザック問題（動的計画法による解法）
*/

#include <stdio.h>
#include <stdlib.h>

/*配列size、valueはそれぞれ品物の大きさと価値を表している。
　同じ添え字同士が対応している。たとえば、品物0の大きさはsizeof[0]、
　価値はvalue[0]に入っている*/

/*品物の大きさ*/
int size[] = {2, 3, 5, 7, 9};

/*品物の価値*/
int value[] = {2, 4, 7, 11, 14};

/*品物の種類の数*/
#define N   (sizeof(size) / sizeof(size[0]))

/*ナップザックの大きさの上限*/
#define MAX_M   200

int main (int argc, char **argv)
{
    int i, j;

    /*ナップザックの大きさ*/
    int m;

    /*現時点でナップザックに詰め込んだ品物の価値の合計*/
    int total[MAX_M];

    /*最後に選んだ品物*/
    int choice[MAX_M];

    /*品物iを入れたときの価値の合計*/
    int repack_total;

    if (argc != 2){
        fprintf(stderr, "usage: knapsack m\n");
        fprintf(stderr, "       ここで、mはナップザックの大きさ\n");
        exit(1);
    }

    /*コマンド引数からナップザックの大きさを知る*/
    m = atoi(argv[1]);
    printf("size of knapsack is %d\n", m);
    if (m >= MAX_M){
        fprintf(stderr, "ナップザックの大きさが大きすぎます\n");
        exit(1);
    }

    /*配列をクリアしておく*/
    for (i = 0; i <= m; i++){
        total[i] = 0;
        choice[i] = -1;
    }

    /*品物0～iまでを考慮に入れる*/
    for (i = 0; i < N; i++){

        /*大きさjのナップザックに対して、品物を詰め込んでみる*/
        for (j =size[i]; j <= m; j++){

            /*もし品物iを入れたとすると、価値の合計はいくらになるかを計算して、
            　変数repack_totalに入れる*/
            repack_total = total[j - size[i]] + value[i];

            /*品物iを入れたほうが（入れないより）価値が大きくなるのなら、
            　品物iを入れる*/
            if (repack_total > total[j]){
                total[j] = repack_total;
                choice[j] = i;      //最後に選んだ品物
            }
        }

        /*配列total、choiceの中身をダンプする*/
        printf("i = %d\n", i);
        printf("total = ");
        for (j = 0; j <= m; j++)
            printf("%2d", total[j]);
        printf("\nchoice = ");
        for (j = 0; j <= m; j++)
            printf("%2d", choice[j]);
        printf("\n");
    }

    /*どの品物をナップザックに入れたかを表示する*/
    for (i = m; choice[i] >= 0; i -= size[choice[i]])
        printf("品物 %d (価値%d)を詰め込む\n", choice[i], value[choice[i]]);
        printf("価値の合計 = %d\n", total[m]);
}