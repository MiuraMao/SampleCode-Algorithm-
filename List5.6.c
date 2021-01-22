#include <stdio.h>

/*成績表を操作する関数*/
#define MAX_GAKUSEI 2000
#define MAX_KAMOKU 400

struct SEISEKI{
    struct SEISEKI *glink;
    struct SEISEKI *klink;
    struct GAKUSEI *gakusei;
    struct KAMOKU  *kamoku;
    short seiseki;
};

struct GAKUSEI{
    struct SEISEKI *glink;
    char *name;
}gakusei[MAX_GAKUSEI];

struct KAMOKU{
    struct SEISEKI *klink;
    char *name;
}kamoku[MAX_KAMOKU];

/*初期化*/
void initialize(){
    int i;

    for (i = 0; i < MAX_GAKUSEI; i++)
        gakusei[i].glink = NULL;
    for (i = 0; i < MAX_KAMOKU; i++)
        kamoku[i].klink = NULL;
}

/*学生番号x、科目番号yの成績をaにする*/
void add(int x, int y, short a){
    struct SEISEKI *p;

    if ((p = malloc(sizeof(struct SEISEKI))) == NULL)
        eroor("メモリが足りません");
    p->glink  = gakusei[x].glink;
    gakusei[x].glink = p;
    p->klink = kamoku[y].klink;
    kamoku[y].klink = p;

    p->gakusei = &gakusei[x];
    p->kamoku = &kamoku[y];
    p->seiseki = a;
}

/*学生xの課目yの成績を得る（履修していないときは、-1を返す）*/
short get_seiseki(int x, int y){
    struct SEISEKI *p;
    for (p = gakusei[x].glink; p != NULL; p = p->glink)
        if (p->kamoku == &kamoku[y])
            return (p->seiseki);
        return -1;
}

/*ある課目（課目番号x）を履修している学生の名前と成績をプリントする*/
void print_gakusei(int x){
    struct SEISEKI *p;

    for (p = kamoku[x].klink; p != NULL; p = p->klink)
        printf("%s %d\n", p->gakusei->name, p->seiseki);
}
