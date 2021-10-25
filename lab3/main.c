#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "allpos.h"


int main(){
    FILE *file;
        file = fopen("access_logmini", "r");
    int k = 0;
    char str[500];
    allpos pos = init();
    while (!feof(file)){
        fgets(str, 500, file);
        //printf("%s\n", str);
        pos = split(str);
        char status = str[pos.statuss];
        printf("%s\n", slice_time(str, pos.times, pos.timee));
        if (status == '5')
            k ++;
    }

    printf("%d", k); // ответ 76
    fclose(file);
    return 0;
}