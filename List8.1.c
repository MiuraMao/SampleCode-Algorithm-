/*文字列のためのハッシュ関数*/

//最良とはいえない
int hash(char *s)
{
    int i = 0;
    
    while (*s)
        i += *s++;
    return i % 100;
}