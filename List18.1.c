#include <stdio.h>
/*力まかせのアルゴリズムによる文字列探索*/
/*Brute-force Algorithm*/

/*長さtext_lenの文字列textから長さpat_lenの文字列patternを探索する（力まかせ法）*/

int brute_force_search(char *text, int text_len, char *pattern, int pat_len)
{
    /*変数iは注目しているテキストの位置、変数jは注目しているパターンの位置を表すポインタ*/
    int i, j;

    /*ポインタを初期化する*/
    i = 0; j = 0;

    /*テキストの最後尾に行き当たるか、パターンが見つかるまで繰り返す*/
    while (i < text_len && j <pat_len){

        /*テキストとパターンを1文字比較する*/
        if (text[i] == pattern[j]){

            /*一致した。テキストとパターンのポインタを進める*/
            i++; j++;

        }else{
            
            /*一致しなかった。テキストのポインタを現在注目している先頭から１つ進める。*/
            /*パターンのポインタを先頭に戻す*/
            i = i - j + 1; j = 0;
        }
    }

    /*もし、探索が成功したら、パターンが見つかった位置を返す。失敗したら-1を返す*/
    return (j == pat_len) ? (i - j) : -1;

}