#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include "char10.h"

int max(int a, int b){
    if (a > b)
        return a;
     return b;
}

typedef struct uint1024_t{
    char *n;
    int index, size;
} uint1024_t;

uint1024_t init(){
    uint1024_t num;
    num.index = 0;
    num.size = 128;
    num.n = (char *) malloc(129 * sizeof(char));
    memset(num.n, 0, 129);
    return num;
}

void free_uint(uint1024_t x){
    free(x.n);
}

void bin_exp(uint1024_t x);

// определяет размер числа
int find_index(uint1024_t x){

  for(int i = 1024; i >= 0; -- i){
    int cur = (x.n[i / 8] >> (7 - i % 8)) & 1;
      if (cur)
        return i;
  }
  return 0;

}


uint1024_t from_uint(unsigned int x){
    uint1024_t num = init();
    while (x != 0){
        int cur = num.index;
        num.n[cur / 8] = (char) ((num.n[cur / 8] >> (7 - cur % 8)) | (x % 2)) << (7 - cur % 8);
        num.index ++;
        x /= 2;
    }
    num.index --;

    return num;
}

uint1024_t add_op(uint1024_t x, uint1024_t y){
    uint1024_t result = init();
    int prev = 0;

    for(int i = 0; i < 128; ++i){

        for(int j = 0; j < 8; ++j){
            int digx = ((x.n[i] >> (7 - j)) & 1);
            int digy = ((y.n[i] >> (7 - j)) & 1);
      
            result.n[i] = (char) ((result.n[i] >> (7 - j) ) | ((digx + digy + prev) % 2)) << (7 - j);

            if ((digx && digy) || ((digx || digy) && prev))
                prev = 1;
            else
                prev = 0;
      
        int cur = (result.n[i] >> (7 - j)) & 1;
        if (cur)
            result.index = i * 8 + j;

    }

  }

    if (prev){
        printf("UB\n");
        return init();
    }

    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y){
    uint1024_t result = init();
    int prev = 0;

    for(int i = 0; i < 128; ++i){

        for(int j = 0; j < 8; ++j){
      
            int digx = (x.n[i] >> (7 - j)) & 1;
            int digy = (y.n[i] >> (7 - j)) & 1;

            if ((digx && digy && !prev) || (digx && !digy && prev) || (!digx && !digy && !prev) || (!digx && digy && prev))
                result.n[i] = (char) ((result.n[i] >> (7 - j)) | (0)) << (7 - j);
            else
                result.n[i] = (char) ((result.n[i] >> (7 - j)) | (1)) << (7 - j);

            prev = 1 ?(digx < digy + prev) : 0;

            int cur = (result.n[i] >> (7 - j)) & 1;
            if (cur)
                result.index = 8 * i + j;

        }

    }

    if(prev == 1){
        printf("UB\n");
        return init();
    }

    return result;

}

uint1024_t mult_op(uint1024_t x, uint1024_t y){
    uint1024_t result = init();
    
    for(int i = x.index; i >= 0; --i){
    
        int cur = (x.n[i / 8] >> (7 - i % 8)) & 1;

        if(cur){
            uint1024_t tmp = init();
            for(int j = 0; j <= y.index; ++j){
                int cury = (y.n[j / 8] >> (7 - j % 8)) & 1;
                int cur_index = i + j;
                tmp.n[cur_index / 8] = (char) ((tmp.n[cur_index / 8] >> (7 - cur_index % 8 )) | (cury)) << (7 - cur_index % 8);
            }

            result = add_op(result, tmp);
    }

  }

    result.index = find_index(result);

    return result;
}

void printf_value(uint1024_t x){
    char10 result = init10();
    char10 degree = init10();
    degree.n[0] = 1;
    for(int i = 0; i <= x.index; ++i){
        int cur = (x.n[i / 8] >> (7 - i % 8)) & 1;
        if (cur){
            result = add10(result, degree);
        }
        degree = add10(degree, degree);
    }
    printf10(result);
    free_char10(result);
    free_char10(degree);
}

void scanf_value(uint1024_t* x){
    char *s = (char *) malloc(310 * sizeof(char));
    scanf("%s", s);
    uint1024_t result = init();
    uint1024_t digit = from_uint(10);
    for (int i = 0; i <= (int) strlen(s) - 1; ++i){
        result = mult_op(result, digit);
        uint1024_t cur_digit = from_uint((unsigned int) (s[i] - '0'));
        result = add_op(result, cur_digit);
    }

    *x = result;
    free(s);
}

// Те же самые принт и скан только в файле.
// Я это сделал, потому что не знаю, как читать
// и записывать данные с терминала
void fprintf_value(uint1024_t x, FILE *file){
    char10 result = init10();
    char10 degree = init10();
    degree.n[0] = 1;
    for(int i = 0; i <= x.index; ++i){
        int cur = (x.n[i / 8] >> (7 - i % 8)) & 1;
        if (cur){
            result = add10(result, degree);
        }
        degree = add10(degree, degree);
    }
    fprintf10(result, file);
    free_char10(result);
    free_char10(degree);
}

void fscanf_value(uint1024_t* x, FILE *file){
    char *s = (char *) malloc(310 * sizeof(char));
    fscanf(file, "%s", s);
    uint1024_t result = init();
    uint1024_t digit = from_uint(10);
    for (int i = 0; i <= (int) strlen(s) - 1; ++i){
        result = mult_op(result, digit);
        uint1024_t cur_digit = from_uint((unsigned  int) (s[i] - '0'));
        result = add_op(result, cur_digit);
    }

    *x = result;
    free(s);
}




//выводит число в двоичном виде в обратном порядке разрядов
void bin_exp(uint1024_t x){
    x.index = find_index(x);
    for(int i = 0; i <= x.index; ++i){
        printf("%d", (x.n[i / 8] >> (7 - i % 8 )) & 1);
    }

    printf("\n");
}

int main(void) {
    FILE *fin, *fout;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    uint1024_t a = init();
    uint1024_t b = init();
    uint1024_t sum = init();
    uint1024_t sub = init();
    uint1024_t mult = init();

    for(int i = 0; i < 100; ++i){
        int arg;
        
        fscanf(fin,"%d", &arg);
        
        fscanf_value(&a, fin);
        fscanf_value(&b, fin);
        fscanf_value(&sum, fin);
        fscanf_value(&sub, fin);
        if (arg == 5)
            fscanf_value(&mult, fin);
        
        fprintf_value(a, fout);
        fprintf_value(b, fout);
        fprintf_value(add_op(a, b), fout);
        fprintf_value(subtr_op(a,b), fout);
        
        if (arg == 5){
            fprintf_value(mult_op(a, b), fout);
        }

        fprintf(fout, "\n");

    }
    
    fclose(fin);
    fclose(fout);
}