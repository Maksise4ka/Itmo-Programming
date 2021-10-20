#include <stdio.h>
#include <string.h>
#include "allpos.h"


int main(){
    FILE *file;
    //file = fopen("access_logmini", "r");
    file = fopen("access_log_Jul95", "r");
    int k = 0;
    char str[500];
    allpos pos = init();
    while (!feof(file)){
        fgets(str, 500, file);
        //printf("%s\n", str);
        pos = split(str);
        char status = str[pos.statuss];
        if (status == '5')
            k ++;
    }

    printf("%d", k);
    fclose(file);
    return 0;
}