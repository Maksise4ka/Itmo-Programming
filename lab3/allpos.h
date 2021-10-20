// Структра, которая содержит индексы начала и конца источника запроса, времени запроса, запроса, статуса и количества байт
// Все границы включены

typedef struct allpos {
    int ids, ide, 
    times, timee,
    requests, requeste,
    statuss, statuse,
    bytess, bytese;
} allpos;

allpos init(){
    allpos a;
    a.ids = a.ide = 
    a.times = a.timee = 
    a.requests = a.requeste = 
    a.statuss = a.statuse =
    a.bytess = a.bytese = 0;
    return a;
}

// эти две функции нужны для вывода всех элементов строчки
void print_slice(char s[500], int left, int right){
    for(int i = left; i <= right; ++i){
        printf("%c", s[i]);
    }
    printf("   ");
}

void print_log(char s[500], allpos a){
    print_slice(s, a.ids, a.ide);
    print_slice(s, a.times, a.timee);
    print_slice(s, a.requests, a.requeste);
    print_slice(s, a.statuss, a.statuse);
    print_slice(s, a.bytess, a.bytese);
    printf("\n");
}


// нахождение индексов
allpos split(char str[500]){
    allpos a = init();
    int i = 0;
    while (str[i] != ' '){
        a.ide ++;
        i++;
    }

    while (str[i] != '[')
        i++;
    i ++;
    a.times = i;
    a.timee = i;
    while (str[i] != ']'){
        a.timee ++;
        i ++;
    }
    a.timee --;
    i ++;

    int j = strlen(str) - 1;
    a.bytese = j - 1;
    while (str[j] != ' '){
        j --;
    }
    a.bytess = j + 1;
    a.statuse = j - 1;
    j --;
    printf("%d ", j);
    while (str[j] != ' '){
        j --;
    }
    
    a.statuss = j + 1;
    a.requeste = a.statuss - 2;
    a.requests = a.timee + 2;

    return a;
}