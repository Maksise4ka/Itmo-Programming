#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "allpos.h"

typedef struct time{
    char month[3];
    int day;
    int year;
    int hour;
    int min;
    int sec;
    int utc;
} time;

int convert_char(char c){
    return c - 48;
}

time init_time(char *s){
    time t;
    t.day = convert_char(s[0]) * 10 + convert_char(s[1]);
    t.month[0] = s[3]; t.month[1] = s[4]; t.month[2] = s[5];
    t.year = convert_char(s[7]) * 1000 + convert_char(s[8]) * 100 + convert_char(s[9]) * 10 + convert_char(s[10]);;
    t.hour = convert_char(s[12]) * 10 + convert_char(s[13]);
    t.min = convert_char(s[15]) * 10 + convert_char(s[16]);
    t.sec = convert_char(s[18]) * 10 + convert_char(s[19]);
    t.utc = convert_char(s[23]);
    return t;
}

int main(){
    //int a[1900000];
    FILE *file;
        file = fopen("access_logmini", "r");
    int k = 0;
    char str[500];
    allpos pos = init();
    int i = 0;
    while (!feof(file)){
        fgets(str, 500, file);

        //1 вопрос
        pos = split(str);
        char status = str[pos.statuss];
        if (status == '5')
            k++;

        // 2 вопрос
        char *tmp = slice_time(str, pos.times, pos.timee);
        time t = init_time(tmp); 
        free(tmp);
        printf("%d %s %d %d %d %d %d\n", t.day, t.month, t.year, t.hour, t.min, t.sec, t.utc);

    }

    //free(a);
    printf("%d", k); // ответ 76
    fclose(file);
    return 0;
}