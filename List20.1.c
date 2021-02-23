/*擬似コード（pseudocode）*/
/*８クイーン(Eight Queens Puzzle)の解法（解を１つだけ見つける）*/

/*関数トライはa行目以降のクイーンすべてを置くことに成功したら、SUCCESSを返し、
　失敗したらFAILを返す*/

int try(a)
{
    for (場所(a, 0), (a, 1)......(a, 7)について繰り返す){
        if (この場所は、他のクイーンの利き筋になっていない){
            この場所にクイーンを置く

            if (a == 7){     //すべてのクイーンが置けた
                return SUCCESS;
            }else{
                if (try(a + 1) == SUCCESS)  //a+1行目以降のすべてに置けた
                    return SUCCESS;
                else
                    失敗したので、クイーンを盤から取り除く
            }
        }
    }
    //結局a行目にはクイーンが置ける場所はなかった
    return FAIL;
}