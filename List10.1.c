/*
*   b_tree.c -- B木による探索
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILD   5       /*五分木*/
#define HALF_CHILD  ((MAX_CHILD+1)/2)

/*KEYはキーとなるデータ型、DATAはデータ本体を表すデータ型にtypedefする*/
typedef int     KEY;
typedef int     DATA;

typedef struct node{
    int nodekind;       /*節の種類（INTERNAL, LEAF）*/
    union {
        struct {            /*内部節の場合*/
            int         _nchilds;           /*この節がもっている子の数*/
            struct node *_child[MAX_CHILD];     /*部分木*/
            KEY         _low[MAX_CHILD];        /*各部分木の最小の要素*/
        }_internal;
        struct {            /*葉の場合*/
            KEY         _leaf_key;      /*葉がもっているキーの値*/
            DATA        _your_data;     /*葉に格納するデータ*/
        }_leaf;
    }_u;
}NODE;

/**節の種類を表す定数（メンバnodekindにセットする）**/
#define INTERNAL    1      /*内部節*/
#define LEAF        2      /*葉*/

/**内部節のメンバを参照するためのマクロ定義**/
#define nchilds     _u._internal._nchilds
#define child       _u._internal._child
#define low         _u._internal._low

/**葉のメンバを参照するためのマクロ定義**/
#define leaf_key    _u._leaf._leaf_key
#define your_data   _u._leaf._your_data

NODE    *root   = NULL;

error(char *s)
{
    fprintf(stderr, s);
    exit(1);
}

/*
* alloc_node -- 構造体NODE型を当てて返す
*/
NODE    *alloc_node(void)
{
    NODE *p;

    if((p = malloc(sizeof(NODE))) == NULL)
        error("メモリを使い果たしてしまった\n");
        return p;
}

/*
* keyequal -- ２つのキーが等しいかどうか比較する
*/
int     keyequal(KEY a, KEY b)
{
    return a == b;
}

/*
* keylt -- ２つのキーa、bが"a < b"の関係を満たしているかどうか調べる
*/
int     keylt(KEY a, KEY b)
{
    return a < b;
}

/*
* locate_subtree -- 節pにおいて、キーkeyをもつデータは何番目の部分木に入るかを返す
*
*/
int     locate_subtree(NODE *p, KEY key)
{
    int i;

    for (i = p->nchilds - 1; i> 0; i--)
        if (key >= p->low[i])
            return i;
    return 0;
}

/*
* search -- B木を探索する
*           節へのポインタを返す
*           見つからなければ、NULLを返す
*           key：探索するキー
*/
NODE    *search(KEY key)
{
    NODE *p;
    int  i;

    /*空の木であれば、即座にNULLを返す*/
    if (root == NULL)
        return NULL;
    else{
        /*根から始めて、葉にたどりつくまで内部節をたどる*/
        p = root;
        while (p->nodekind == INTERNAL){
            i = locate_subtree(p, key);
            p = p->child[i];
        }
        /*与えられたキーと、葉にセットされているキーを比較する*/
        if (key == p->leaf_key)
            return p;           /*探索成功*/
        else
            return NULL;        /*探索失敗*/
    }
}

/*
* isnert_aux -- 節*pnodeに対して、キー値keyをもつ要素を挿入する
*               挿入した要素が置かれる節へのポインタを返す
*               もし新しい節を作ったなら、それを*newnodeにセットして
*               *newnodeの指す部分木中の最小要素を*lowestに返す
*               新しい節を作らなかったら、*newnodeにはNULLを返す
*/
NODE    *insert_aux(NODE **pnode, KEY key, NODE **newnode, KEY *lowest)
{
    NODE *node;

    *newnode = NULL;        /*とりあえずNULLにする*/
    node = *pnode;
    if (node->nodekind == LEAF){
    /*この節は、葉である*/
        /*すでに、登録済であれば、何もしないでNULLを返す*/
        if (keyequal(node->leaf_key, key))
            return NULL;
        else{
        /*新たに葉を割り当てて、*newnode、*lowestをセットして、*/
        /*割り当てた葉へのポインタを返す*/
            NODE *new;
            new = alloc_node();
            new->nodekind = LEAF;
            new->leaf_key = key;
            /*もし、割り当てた葉newの方が小さいなら、newとnodeの位置を*/
            /*入れ替える*/
            if (key < node->leaf_key){
                /*元のnodeには、新しく割り当てた葉をnewを入れる*/
                *pnode = new;
                /*新たに割り当てた葉として、nodeを報告する*/
                *lowest = node->leaf_key;
                *newnode = node;
            }else{
                /*新たに割り当てた葉として、newを報告する*/
                *lowest = key;
                *newnode = new;
            }
            /*いずれの場合ｎしても、新たに割り当てた葉newが関数値となる*/
            return new;
        }
    }else{
    /*子の節は、内部節である*/
        int pos;        //何番目の部分木に挿入するか？ 再帰呼び出したinsert_auxへのパラメータ
        NODE *xnode;    //パラメータ　newnode
        KEY  xlow;      //パラメータ　lowest
        NODE *retv;     //関数値
        int i, j;

        /*どの部分木に挿入するかを決める*/
        pos = locate_subtree(node, key);
        /*部分木に対して、自分自身を再帰呼び出しする*/
        retv = insert_aux(&(node->child[pos]), key, &xnode,&xlow);
    /*もし、分割が行われていなければ、そのままリターンする*/
        if (xnode = NULL)
            return retv;
    /*分割が行われていたので、節nodeにそれ（xnode）を挿入する*/
        /*節nodeに追加の余地があるか？*/
        if (node->nchilds < MAX_CHILD){
            /*追加の余地があったので、適切な位置に挿入する*/
            for (i = node->nchilds - 1; i > pos; i--){
                node->child[i+1] = node->child[i];
                node->low  [i+1] = node->low  [i];
            }
            node->child[pos+1] = xnode;
            node->low  [pos+1] = xlow;
            return retv;
        }else{
            /*追加の余地がないので、節nodeを２つに分割しなければならない*/
            NODE *new;
            /*新しい内部節newを割り当てる*/
            new = alloc_node();
            new->nodekind = INTERNAL;
            /*節xnodeがどちらの節に挿入されるかで、場合分けする*/
            if (pos < HALF_CHILD - 1){
            /*節xnodeが節nodeの側に挿入される*/
                /*まず、HALF_CHILD-1～MAX_CHILD-1番目の部分木を、節nodeから、節newへ移す*/
                for (i = HALF_CHILD-1, j = 0; i < MAX_CHILD; i++, j++) {
                    new->child[j] = node->child[i];
                    new->low  [j] = node->low  [i];
                }
                /*0～HALF_CHILD-2番目の部分木の間の適切な位置に、節xnodeを挿入する*/
                for (i = HALF_CHILD-2; i > pos; i--){
                    node->child[i+1] = node->child[i];
                    node->child[i+1] = node->low[i];
                }
                node->child[pos+1] = xnode;
                node->low  [pos+1] = xlow;     
            }else{
            /*節xnodeは節newの側に挿入される*/
                /*HALF_CHILD～MAX_CHILD-1番目の部分木を節newに移動する。同時に節xnodeを適切な場所に挿入する*/
                j = MAX_CHILD - HALF_CHILD;
                for (i = MAX_CHILD-1; i >= HALF_CHILD; i--){
                    if (i == pos){
                        new->child[j] = xnode;
                        new->low  [j--] = xlow;
                    }
                    new->child[j] = node->child[i];
                    new->low  [j--] = node->low[i];
                }
                if (pos < HALF_CHILD){
                    new->child[0] = xnode;
                    new->low  [0] = xlow;
                }
            }
            /*子の数nchildsを変更する*/
            node->nchilds = HALF_CHILD;
            new->nchilds = (MAX_CHILD + 1) - HALF_CHILD;
            /*分割して作られた節をパラメータ*newnodeに返す　また、その最小値をパラメータ*lowestに返す*/
            *newnode = new;
            *lowest = new->low[0];
            return retv;
        }
    }
}

/*
* insert -- B木に要素を挿入する
*           挿入した要素が置かれる節へのポインタを返す
*           すでに要素が登録されているのなら、何もしないでNULLを返す
*           key：挿入するデータ
*           ※実際にB木をたどって要素を挿入するのは、関数insert_auxである
*/
NODE    *insert(KEY key)
{
    /*木が空の場合には、葉を作ってそれを返す*/
    if (root == NULL){
        root = alloc_node();
        root->nodekind = LEAF;
        root->leaf_key = key;
        return root;
    }else {
        /*木が空でない場合には、関数insert_auxを呼び出して、要素の挿入を行う*/
        NODE    *retv, *new, *newnode;
        KEY     lowest;

        retv =  insert_aux(&root, key, &newnode, &lowest);
        /*もし、分割が行われたなら。木の高さを一段高くする*/
        if (newnode != NULL){
            new = alloc_node();
            new->nodekind = INTERNAL;
            new->nchilds = 2;
            new->child[0] = root;
            new->child[1] = newnode;
            new->low[1] = lowest;
            root = new;
        }
        return retv;
    }
}

/*
* merge_nodes -- 節pのx番目とx+1番目の部分木とを再編成する
*               もし、併合が必要なら、すべての要素をx番目の部分木に
*               集めて、値１を返す
*/
int     merge_nodes(NODE *p, int x)
{
    NODE    *a,     //x番目の部分木
            *b;     //x+1番目の部分木
    int     an,     //部分木aの子の数
            bn;     //部分木bの子の数
    int     i;

    a = p->child[x];
    b = p->child[x+1];
    b->low[0] = p->low[x+1];
    an = a->nchilds;
    bn = b->nchilds;
    if (an + bn <= MAX_CHILD){
    /*部分木aとbとを併合しなければならない*/
        /*bの子をすべてaへ移動する*/
        for (i = 0; i < bn; i++){
            a->child[i+an] = b->child[i];
            a->low  [i+an] = b->low  [i];
        }
        a->nchilds += bn;       //子の数を更新する
        free(b);                //節bを削除する
        return 1;               //併合したことを通知する
    }else{
    /*部分木aとbとで、節を再分配する*/
        int n,      //部分木aに分配すべき子の点
            move;   //移動する要素の個数

        n = (an + bn) / 2;
        if (an > n){
        /*部分木aから、部分木bへと移動する*/
            move = an - n;      //move個の子をaからbへ移す
            /*bの要素を右にずらす*/
            for (i = bn -1;i >= 0; i--){
                b->child[i+move] = b->child[i];
                b->low  [i+move] = b->low  [i];
            }
            /*aからbへmove個の子を移動する*/
            for (i = 0; i < move; i++){
                b->child[i] = a->child[i+n];
                b->low  [i] = a->low[i+n];
            }
        }else{
        /*部分木bから、部分木aへと移動する*/
            move = n - an;      //move個の子をbからaへ移す
            /*bからaへmove個の子を移動する*/
            for (i = 0; i < move; i++){
                a->child[i+an] = b->child[i];
                a->low  [i+an] = b->low  [i];
            }
            /*bの要素を左へつめ合わせる*/
            for (i = 0; i < bn - move; i++){
                a->child [i] = b->child[i+move];
                a->low   [i] = b->low  [i+move];
            }
        }
        /*子の個数を更新する*/
        a->nchilds = n;
        b->nchilds = an + bn -n;
        /*部分木bの最小値を、節pにセットする*/
        p->low[x+1] = b->low[0];
        return 0;  
    }
}

/*
* delete_aux -- 節nodeに対して、キーkeyを持つ要素を削除する
*               削除による節nodeの変化を*resultに返す
*/
/*resultの値*/
#define OK          1       //nodeには何の変化もない
#define REMOVED     2       //nodeそのものが削除された
#define NEED_REORG  3       //nodeの子が少なく（HAALF_CHILD以下）なったので、再編成が必要となった

int     delete_aux(NODE *node, KEY key, int *result)
{
    *result = OK;
    if (node->nodekind == LEAF){
    /*この節は、葉である*/
        /*この葉のキーとkeyが等しければ、削除する*/
        if (keyequal(node->leaf_key, key)){
            *result = REMOVED;       //nodeは削除された
            free(node);
            return 1;
        }else{
        /*キーが一致しない。つまり、与えられたキーをもつ要素は存在しなかった*/
            return 0;
        }
    }else{
    /*この節は、内部節である*/
        int pos;            //何番目の部分木から削除するか？
        int condition;      //部分木に何が起こったか？
        int retv;           //部分木の削除の結果
        int sub;            //部分木subとsub+1とを再編成する
        int joined;         //再編成の結果、部分木が併合されたか？
        int i;

        /*どの部分木から削除するかを決める*/
        pos = locate_subtree(node, key);
        /*部分木に対して、自分自身を再帰呼び出しする*/
        retv = delete_aux(node->child[pos], key, &condition);
        /*部分木に何の変化もなければ、リターンする*/
        if (condition == OK)
            return retv;

    /*部分木posを再編成する必要あるか？*/
        if (condition == NEED_REORG){
            sub = (pos == 0) ? 0 : pos - 1;  //真の場合０．偽の場合pos-1
            /*部分木subとsub+1を再編成する*/
            joined = merge_nodes(node, sub);
            /*もし、subとsub+1が併合されていたら、部分木sub+1をnodeから削除する必要がある*/
            if (joined)     //0だったら
                pos = sub+1;
        }
    /*部分木posが削除された*/
        if (condition == REMOVED || joined){
            /*nodeの部分木をつめ合わせる*/
            for (i = pos; i < node->nchilds-1; i++){
                node->child[i] = node->child[i+1];
                node->low  [i] = node->low  [i+1];
            }
            /*もし、nodeの部分木の数がHALF_CHILDより小さいなら再編成が必要である*/
            if (--node->nchilds < HALF_CHILD)
                *result = NEED_REORG;

        }
        return retv;
    }
}

/*
* delete -- B木から要素を削除する
*           削除に成功すれば１、要素が存在しなければ０を返す
*           key：削除するデータ
*/
int     delete(KEY key)
{
    int retv, result;
    NODE *p;

    if (root == NULL)       //木が空であるか？
        return 0;
    else{
        retv = delete_aux(root, key, &result);
        if (result == REMOVED)
            root = NULL;
        else if(result == NEED_REORG && root->nchilds == 1){
            p = root;
            root = root->child[0];
            free(p);
        }
        return retv;
    }
}

/*
* printtree -- B木の内容をプリントする（デバッグ用） 
*/
printtree(NODE *p)
{
    int i;

    if (p->nodekind == LEAF){
        printf("%04x leaf val = %d\n", p, p->leaf_key);
    }else {
        printf("%04x %02d [%04x] %d[%04x] %d[&04x] %d[&04x] %d[&04x]\n",
                p, p->nchilds, p->child[0],
                p->low[1], p->child[1],
                p->low[2], p->child[2],
                p->low[3], p->child[3],
                p->low[4], p->child[4]);
        for (i = 0; i < p->nchilds; i++)
            printtree(p->child[i]);
    }
}

/*
* main -- テスト用のメインルーチン
*
*         ">"というプロンプトが表示されるので、次のように入力すると、その結果が表示される
*           +n：nを挿入する
*           -n：nを削除する
*           n：nを探索する
*/
main()
{
/*  static    int     data[] = { 10, 20 30, 40, 50 };*/
    static    int     data[] = { 13, 5, 2, 7, 6, 21, 15 };
    int i, x;
    char    str[100];

    for (i = 0; i < sizeof(data)/sizeof(data[0]); i++)
        insert(data[i]);
    
    printf("+n : nを挿入する\n-n : nを削除する\nn : nを探索する\n");
    printtree(root);
    printf("\n>");

    while (gets(str) != NULL){
        x = abs(atoi(str));
        switch (str[0]){
            case '+':
                if (insert(x) != NULL)
                    printf("%d inserted.\n", x);
                else
                    printf("%d not inserted.\n", x);
                break;
            case '-':
                if(delete(x))
                    printf("%d deleted.\n", x);
                else
                    printf("%d not deleted.\n", x);
                break;
            default:
                if (search(x) != NULL)
                    printf("%d found.\n", x);
                else
                    printf ("%d not fond.\n", x);
                break;
        }
        printtree(root);
        printf("\n>");
    }
}