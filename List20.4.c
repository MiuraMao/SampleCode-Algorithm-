/*nクイーン（n-Queen）の解法（すべての解を表示する）*/

/*a行目以降すべての行にクイーンを置いてみる（すべての解を表示する）*/
void try_all(int a)
{
    int b;

    /*左から右に向って順番にクイーンが置けるかどうかを調べる*/
    for (b = 0; b < N; b++){

        /*a行目のb番目に置けるかどうかを調べる*/
        if (col[b] == FREE && up[a + b] == FREE && down[a - b + (N-1)] == FREE){

            /*置くことができた。場所を記録して、利き筋をセットする*/
            pos[a] = b;
            col[b] = NOT_FREE;
            up[a + b] = NOT_FREE;
            down[a - b + (N-1)] = NOT_FREE;

            /*N個のクイーンをすべて置くことができれば成功である*/
            if (a + 1 >= N)
                print_queens();
            else
                try_all(a + 1);

            /*クイーンを取り除く*/
            pos[a] = -1;
            col[b] = FREE;
            up[a + b] = FREE;
            down[a - b + (N-1)] = FREE;
        }
    }
}