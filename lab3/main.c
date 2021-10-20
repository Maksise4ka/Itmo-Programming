#include <stdio.h>
#include <string.h>
#include "allpos.h"


int main(){
    FILE *file;
    file = fopen("access_logmini", "r");
    

    char str[500];
    while (!feof(file)){
        fgets(str, 500, file);
        //printf("%s\n", str);
        allpos pos = split(str);
    }

    fclose(file);
    return 0;
}