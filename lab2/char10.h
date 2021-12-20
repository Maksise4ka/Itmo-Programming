// Структура данных, являющаяся 309 - значным числом,
// каждая цифра представляет собой char

typedef struct char10{
    char *n;
    int index, size;
} char10;

char10 init10();

void free_char10(char10);

char10 add10(char10, char10);

int find10(char10);

void printf10(char10);

void fprintf10(char10, FILE*);