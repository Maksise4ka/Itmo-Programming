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

int convert_to90(time t){
    int month = 7;
    int res = ((((t.year - 1990) * 365 + month * 30 + t.day) * 24 + t.hour) * 60 + t.min) * 60 + t.sec; 
    return res; // я здесь считаю что в месяце 30 дней и что високосных годов не существует:)
}


int a[1900000];
int main(){
    FILE *file;
    file = fopen("access_log_Jul95", "r");
    char str[500];
    int ans1 = 0;
    allpos pos = init();
    int i = 0;
    while (!feof(file)){
        fgets(str, 500, file);

        //1 вопрос
        pos = split(str);
        char status = str[pos.statuss];
        if (status == '5')
            ans1++;

        // 2 вопрос
        char *tmp = slice_time(str, pos.times, pos.timee);
        time t = init_time(tmp); 
        free(tmp);
        a[i] = convert_to90(t);
        i++;
        
    }

    // продолжение второго вопроса
    int n;
    scanf("%d", &n);
    int ansl = 0, ansr = 0, l = 0, r = 0, ans2 = 0;
    for(int i = 0; i < 1900000; ++i){
        if (a[i] == 0)
            break;
        if (a[i] - a[l] > n){
            int k = i - l;
            if (k > ans2){
                ansr = i - 1;
                ansl = l;
                ans2 = ansr - ansl + 1;
            }

        while (a[i] - a[l] > n)
            l += 1;
        }
    }

    printf("quest 1: %d \nquest 2: %d, from %d to %d", ans1, ans2, ansl, ansr);
    // ответ 76
    fclose(file);
    return 0;
}