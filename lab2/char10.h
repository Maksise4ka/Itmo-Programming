// Структура данных, являющаяся 309 - значным числом,
// каждая цифра представляет собой char

typedef struct char10{
    char *n;
    int index, size;
} char10;

char10 init10(){
    char10 num;
    num.index = 0;
    num.size = 309;
    num.n = (char *) malloc(311 * sizeof(char));
    memset(num.n, 0, 311);
    return num;
}

void free_char10(char10 x){
    free(x.n);
}

char10 add10(char10 x, char10 y){
    char10 result = init10();
    int prev = 0;
    for(int i = 0; i <= 309; ++i){ 
        result.n[i] = x.n[i] + y.n[i] + prev;

        if (result.n[i] >= 10){
            result.n[i] %= 10;
            prev = 1;
        }
        else
            prev = 0;

        if(result.n[i])
            result.index = i;
    }

    /*if (prev == 1){
        printf("UB");
        return init10();
    }*/
    return result;
}

int find10(char10 x){
    for(int i = 309; i >= 0; --i){
        int cur = x.n[i];
        if (cur)
            return i;
    }
    return 0;
}

void printf10(char10 x){
    x.index = find10(x);
    for(int i = x.index; i >= 0; --i)
        printf("%d", x.n[i]);
    printf("\n");
}

void fprintf10(char10 x, FILE *file){
    x.index = find10(x);
    for(int i = x.index; i >= 0; --i)
        fprintf(file, "%d", x.n[i]);
    fprintf(file, " ");
}
