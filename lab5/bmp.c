#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"

char* sum(const char *s1, const char *s2){
    const int len1 = strlen(s1);
    const int len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); 
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); 
    return result;
}

bmp init(char *filename) {
    bmp img;
    img.file = fopen(filename, "r");
    img.size = 0;
    if (!img.file) {
        printf("Incorrect file");
        return img;
    }
    char c1 = fgetc(img.file);
    char c2 = fgetc(img.file); // c1 = "B" c2 = "M"
    if (c1 != 'B' || c2 != 'M'){
        printf("Incorrect file");
        return img;
    }
    char c;
    for(int i = 0; i < 4; ++i)
        img.size += (fgetc(img.file) << 8 * i);
    for(int i = 0; i < 4; ++i)
        c = fgetc(img.file); //по 2 байта на 2 резерва
    img.offset = 0; img.hSize = 0; img.height = 0; img.width = 0;
    for(int i = 0; i < 4; ++i)
        img.offset += (fgetc(img.file) << 8 * i);
    for(int i = 0; i < 4; ++i)
        img.hSize += (fgetc(img.file) << 8 * i);
    for(int i = 0; i < 4; ++i)
        img.width += (fgetc(img.file) << 8 * i);
    for(int i = 0; i < 4; ++i)
        img.height += (fgetc(img.file) << 8 * i);
    int header_size;
    fseek(img.file, 28, SEEK_SET);
    img.bCount = fgetc(img.file);
    img.bCount += (fgetc(img.file) << 8);
    return img;
}

void to(bmp img, char **a) {
    fseek(img.file, img.offset, SEEK_SET);
    for (int i = img.height - 1; i >= 0; --i)
        for (int j = 0; j < (img.width + 31) / 32 * 4; ++j) {
            int c = fgetc(img.file);
            for (int k = 0; k < 8; ++k)
                if (8 * j + k < img.width)
                    a[i][8 * j + k] = !((c >> (7 - k)) & 1);
        }
    rewind(img.file);
}

void from(bmp img, char **a) {
    fseek(img.file, img.offset, SEEK_SET);
    for (int i = img.height - 1; i >= 0; --i)
        for (int j = 0; j < (img.width + 31) / 32 * 4; ++j) {
            int byte = 0;
            for (int k = 0; k < 8; ++k)
                if (8 * j + k < img.width)
                    byte += ((!a[i][8 * j + k]) << (7 - k));
            fputc(byte, img.file);
        }
    rewind(img.file);
}

void generate(bmp img, char **a) {
    char **tmp;
    tmp = malloc(img.height * sizeof(char *));
    for (int i = 0; i < img.height; ++i) {
        tmp[i] = malloc(img.width);
        memset(tmp[i], 0, img.width);
    }
    int h = img.height, w = img.width;
    tmp[0][0] = a[1][0] + a[0][1] + a[1][1];
    tmp[h - 1][0] = a[h - 2][0] + a[h - 1][1] + a[h - 2][1];
    tmp[h - 1][w - 1] = a[h - 2][w - 1] + a[h - 1][w - 2] + a[h - 2][w - 2];
    tmp[0][w - 1] = a[1][w - 1] + a[0][w - 2] + a[1][w -2]; 
    for(int i = 1; i < h - 1; ++i) {
        tmp[i][0] = a[i + 1][0] + a[i - 1][0] + a[i][1] + a[i + 1][1] + a[i - 1][1];
        tmp[i][w - 1] = a[i + 1][w - 1] + a[i - 1][w - 1] + a[i][w - 2] + a[i + 1][w - 2] + a[i - 1][w - 2];
    }
    for(int i = 1; i < w - 1; ++i) {
        tmp[0][i] = a[0][i + 1] + a[0][i - 1] + a[1][i] + a[1][i - 1] + a[1][i + 1];
        tmp[h - 1][i] = a[h - 1][i + 1] + a[h - 1][i - 1] + a[h - 2][i] + a[h - 2][i + 1] + a[h - 2][i - 1];
    }
    for(int i = 1; i < h - 2; ++i)
        for(int j = 1; j < w - 2; ++j)
            tmp[i][j] = a[i - 1][j] + a[i + 1][j] + a[i][j - 1] + a[i][j + 1] + a[i - 1][j - 1] + a[i - 1][j + 1] + a[i + 1][j - 1] + a[i + 1][j + 1];

    for(int i = 0; i < h - 1; ++i)
        for(int j = 0; j < w - 1; ++j) {
            if (a[i][j] == 0 && tmp[i][j] == 3)
                a[i][j] = 1;
            else if (a[i][j] == 1 && tmp[i][j] < 2)
                a[i][j] = 0;
            else if (a[i][j] == 1 && (tmp[i][j] == 2 || tmp[i][j] == 3))
                a[i][j] = 1;
            else if (a[i][j] == 1)
                a[i][j] = 0;
        }
    
    
    // for(int i = 0; i < img.height; ++i) {
    //     for(int j = 0; j < img.width; ++j) {
    //         printf("%d", tmp[i][j]);
    //     }
    //     printf("\n");
    // }
    for(int i = 0; i < img.height; ++i)
        free(tmp[i]);
    free(tmp);
}

void createBmp(char *filename, bmp img, char **a) {
    FILE *file = fopen(filename ,"w");
    rewind(img.file);
    for(int i = 0; i < img.offset; ++i)
        fputc(fgetc(img.file), file);
    for (int i = img.height - 1; i >= 0; --i)
        for (int j = 0; j < (img.width + 31) / 32 * 4; ++j) {
            int byte = 0;
            for (int k = 0; k < 8; ++k)
                if (8 * j + k < img.width)
                    byte += ((!a[i][8 * j + k]) << (7 - k));
            fputc(byte, file);
        }
    fclose(file);
}

void dest(bmp *img) {
    free(img->file);
}

void startGame(bmp img, int max_iter, int dump_freq, char *dir_name) {
    char **a;
    a = malloc(img.height * sizeof(char *));
    for (int i = 0; i < img.height; ++i) {
        a[i] = malloc(img.width);
        memset(a[i], 0, img.width);
    }
    to(img, a);
    char tmp[10];
    for(int i = 1; i <= max_iter; ++i) {
        generate(img, a);

        if (i % dump_freq == 0) {
            sprintf(tmp, "%d", i);  
            //printf("%s\n", sum(sum(dir_name, "\\"), sum(sum("result", tmp), ".bmp")));
            createBmp(sum(sum(dir_name, "\\"), sum(sum("result", tmp), ".bmp")), img, a);
        }
    }
    createBmp("result.bmp", img, a);

    for(int i = 0; i < img.height; ++i)
        free(a[i]);
    free(a);
}

// int main() {
//     bmp img = init("glider_gun.bmp");
//     startGame(img, 1000, 100, "result");
//     return 0;
// }