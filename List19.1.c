/*正規表現のパターンマッチ*/

/*
    DFAによる正規表現のパターンマッチ
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEBUG   1

/*******************************************
   下請けルーチン
*******************************************/

/*sizeバイトのメモリを割り当てて、0クリアして返す
割り当て失敗したら、プログラムを停止する*/

void *alloc_mem(int size)
{
    void *p;

    if((p = malloc(size)) == NULL){
        fprintf(stderr, "Out of memory.\n");
        exit(2);
    }
    memset(p, 0, size);
    return p;
}

/*******************************************
   正規表現をパースして構文木を生成する

   次の構文で定義される正規表現を受理する
   <regexp>     ::= <term> | <term> "|" <regexp>
   <term>       ::= EMPTY  | <factor> <term>
   <factor>     ::= <pripary> | <pripary> "*" | <primary> "+"
   <pripary>    ::= CHARACTER | "(" <regexp> ")"
 *******************************************/

/*token_tはトークンを表す型*/
typedef enum{
    tk_char,        //文字（実際の文字はtoken_charにセットされる）
    tk_union,       // '|'
    tk_lpar,        //'('
    tk_rpar,        //')'
    tk_star,        //'*'
    tk_plus,        //'+'
    tk_end          //文字列の終わり
} token_t;

/*現在処理中のトークン*/
token_t current_token;

/*current_tokenがtk_charであるときの文字値*/
char token_char;

/*解析する文字列へのポインタ*/
char *strbuff;

/*op_tはノードの操作を表す型*/
typedef enum{
    op_char,        //文字そのもの
    op_concat,      //XY（連結）
    op_union,       //X|Y（選択）
    op_closure,     //X*（繰り返し）
    op_empty        //空
} op_t;

/*tree_tは構文木のノードを表す型*/
typedef struct tree {
    op_t    op;                     //このノードの操作
    union {
                                    //op == op_charのとき
        char c;                     //文字
        struct {                    //op == op_char以外のとき
            struct tree *_left;     //左の子
            struct tree *_right;    //右の子
        } x;
    } u;
} tree_t;

/*構文エラーの後始末をする。実際には、メッセージを表示して、プログラムを停止する*/
void syntax_error(char *s)
{
    fprintf(stderr, "Syntax error : %s\n", s);
    exit (1);
}

/*トークンを１つ読み込んで返す*/
token_t get_token()
{
    char c;

    /*文字列の終わりに到達したらtk_endを返す*/
    if (*strbuff == '\0')
        current_token = tk_end;
    else{
    /*文字をトークンに変換する*/
        c = *strbuff++;
        switch (c) {
        case '|' :      current_token = tk_union;   break;
        case '(' :      current_token = tk_lpar;    break;
        case ')' :      current_token = tk_rpar;    break;
        case '*' :      current_token = tk_star;    break;
        case '+' :      current_token = tk_plus;    break;
        default:        token_char = c;    
        }
    }
    return current_token;
}


