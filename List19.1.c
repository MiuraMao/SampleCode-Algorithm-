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

/*正規表現を解析するパーサを初期化する*/
void initialize_regexp_parser(char *str)
{
    strbuff = str;
    get_token();
}

/*構文木のノードを作成する。opはノードが表す演算、leftは左の子、rightは右の子*/
tree_t *make_tree_node(op_t op, tree_t *left, tree_t *right)
{
    tree_t  *p;

    /*ノードを割り当てる*/
    p = (tree_t *)alloc_mem(sizeof(tree_t));    //構造体ポインタ

    /*ノードに情報を設定する*/
    p->op = op;
    p->u.x._left = left;
    p->u.x._right = right;
    return p;
}

/*構文木の葉を作る。cはこの葉が表す文字*/
tree_t  *make_atom(char c)
{
    tree_t  *p;

    /*葉を割り当てる*/
    p = (tree_t *)alloc_mem(sizeof(tree_t));

    /*葉に情報を設定する*/
    p->op = op_char;    //文字そのもの
    p->u.c = c;
    return p;
}

//typedef
tree_t  *regexp(), *term(), *factor(), *primary();

/*<regexp>をパースして、得られた構文木を返す。選択X|Yを解析する*/
tree_t *regexp()
{
    tree_t  *x;

    x = term();
    while (current_token == tk_union){
        get_token();
        x = make_tree_node(op_union, x, term());
    }
    return x;
}

/*<term>をパースして、得られた構文木を返す。連結XYを解析する*/
tree_t *term()
{
    tree_t  *x;
    if (current_token == tk_union || 
        current_token == tk_rpar ||
        current_token == tk_end)
        x = make_tree_node(op_empty, NULL, NULL);
    else{
        x = factor();
        while (current_token != tk_union &&
               current_token !=  tk_rpar &&
               current_token != tk_end){
                   x = make_tree_node(op_concat, x, factor());
        }
    }
    return x;
}

/*<factor>をパースして、得られた構文木を返す。繰り返しX*、X+を解析する*/
tree_t *factor()
{
    tree_t  *x;

    x = primary();
    if (current_token == tk_star){
        x = make_tree_node(op_closure, x, NULL);
        get_token();
    }else if (current_token == tk_plus){
        x = make_tree_node(op_concat, x, make_tree_node(op_closure, x, NULL));  //再帰
        get_token();
    }
    return x;
}

/*<primary>をパースして、得られた構文木を返す。文字そのもの、(X)を解析する*/
tree_t *primary()
{
    tree_t *x;

    if(current_token == tk_char){
        x = make_atom(token_char);      //葉を作る
        get_token();
    }else if (current_token == tk_lpar){
        get_token();
        x = regexp();
        if (current_token != tk_rpar)
            syntax_error("Close paren is expected.");
        get_token();
    }else {
        syntax_error("Normal character or open paren is expected.");
    }
    return x;
}

//条件付きコンパイル
#if     DEBUG
/*構文木を表示する（デバッグ用）*/
void dump_tree(tree_t *p)
{
    switch (p->op){
    case op_char : 
        printf("\"%c\"", p->u.c);
        break;
    case op_concat :
        printf("(concat ");
        dump_tree(p->u.x._right);
        printf(" ");
        dump_tree(p->u.x._right);
        printf(")");
        break; 
    case op_union :
        printf("(or ");
        dump_tree(p->u.x._left);
        printf(" ");
        dump_tree(p->u.x._right);
        printf(")");
        break;
    case op_closure :
        printf("(closure ");
        dump_tree(p->u.x._left);
        printf(")");
        break;
    case op_empty :
        printf("EMPTY");
        break;
    default :
        fprintf(stderr, "This cannot happen in <dump_tree>\n");
        exit(2);
    }
}
#endif     /*DEBUG*/

/*正規表現をパースして、正規表現に対応する構文木を返す。strは正規表現が入っている文字列*/
tree_t *parse_regexp(char *str)
{
    tree_t *t;

    /*パーサを初期化する*/
    initialize_regexp_parser(str);

    /*正規表現をパースする*/
    t = regexp();

    /*次のトークンがtk_endでなければエラー*/
    if (current_token != tk_end)
        syntax_error("Extra character at end of patten");

#if     DEBUG
    /*得られた構文木の内容を表示する*/
    dump_tree(t);
    printf("\n");
#endif      /*DEBUG*/

    /*生成した構文木を返す*/
    return t;
}

/*******************************************
 　 構文木からNFAを生成する
*******************************************/

/*EMPTYはε遷移を表す*/
#define EMPTY   -1

/*nlist_tはNFAにおける遷移を表す型。文字cによって状態toへと遷移する*/
typedef struct nlist {
    char c;
    int to;
    struct nlist    *next       /*次データへのポインタ*/
} nlist_t;

/*NFAの状態数の上限*/
#define NFA_STATE_MAX   128

/*NFAの状態遷移表*/
nlist_t *nfa[NFA_STATE_MAX];

/*NFAの初期状態*/
int nfa_entry;

/*NFAの終了状態*/
int nfa_exit;

/*NFAの状態数*/
int nfa_nstate = 0;

/*ノードに番号を割り当てる*/
int gen_node()
{
    /*NFAの状態数の上限をチェックする*/
    if (nfa_nstate >= NFA_STATE_MAX){
        fprintf(stderr, "Too many NFA state.\n");
        exit(2);
    }
    return nfa_nstate++;
}

