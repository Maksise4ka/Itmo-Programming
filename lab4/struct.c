#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

id3v2 init(char *filename) {
    id3v2 id3; 
    id3.file = fopen(filename, "r+");
    id3.size = -1;
    if (!id3.file) {
        printf("Incorrect filename");
        return id3;
    }
    char c1 = fgetc(id3.file);
    char c2 = fgetc(id3.file);
    char c3 = fgetc(id3.file); 
    while (c1 != 'I' || c2 != 'D' || c3 != '3') {
        c1 = fgetc(id3.file);
        c2 = fgetc(id3.file);
        c3 = fgetc(id3.file); 
    }
    c1 = fgetc(id3.file);
    c2 = fgetc(id3.file);
    c3 = fgetc(id3.file); // 3 байта на версию субверсию и 3 битовых флага
    char str_size[4];
    for(int i = 0; i < 4; ++i)
        str_size[i] = fgetc(id3.file);
    id3.size = 10 + (str_size[0] << 21) + (str_size[1] << 14) + (str_size[2] << 7) + str_size[3]; // 7 бит не используется
    id3.str = (char*) malloc(id3.size * sizeof(char) + 30);
    id3.str[0] = 'I'; id3.str[1] = 'D', id3.str[2] = '3';
    id3.str[3] = c1; id3.str[4] = c2, id3.str[5] = c3;
    for(int i = 0; i < 4; ++i)
        id3.str[i + 6] = str_size[i];
    for (int i = 10; i < id3.size; ++i) {
        char c = fgetc(id3.file);
        if (c != 1)
            id3.str[i] = c;
        //printf("%c", id3.str[i]);
    }
    //printf("\n");
    return id3;
}

int isFrame(char *s) { // Проверка на то, что данные 10 байт могут быть фреймом
    for (int i = 0; i < 4; ++i)
        if (!(s[i] >= 'A' && s[i] <= 'Z' || s[i] >= '0' && s[i] <= '9'))
            return 0;
    for (int i = 4; i < 8; ++i)
        if (s[i] >= 128)
            return 0;
    if (s[8] & 15 != 0 || s[9] & 128 != 0 || s[9] & 48 != 0)
        return 0;
    return 1;
}

int find(char *prop_name, id3v2 id3) {
    for(int i = 0; i < id3.size - 3; ++i) {
        if (prop_name[0] == id3.str[i] && prop_name[1] == id3.str[i + 1] && prop_name[2] == id3.str[i + 2] && prop_name[3] == id3.str[i + 3])
            if (isFrame(id3.str + i))
                return i;
    }
    return -1;
}

void get(char *prop_name, id3v2 id3) {
    int pos = find(prop_name, id3);
    if (pos == -1)
        printf("There's no such frame");
    else {
        int size = (id3.str[pos + 4] << 21) + (id3.str[pos + 5] << 14) + (id3.str[pos + 6] << 7) + id3.str[pos + 7];
        for (int i = pos + 10; i < pos + 10 + size; ++i)
            printf("%c", id3.str[i]);
    }
}

void show(id3v2 id3) {
    for (int i = 0; i < id3.size; ++i) {
        if (isFrame(id3.str + i)) {
            printf("%c%c%c%c -- ", id3.str[i], id3.str[i + 1], id3.str[i + 2], id3.str[i +3]);
            int size = (id3.str[i + 4] << 21) + (id3.str[i + 5] << 14) + (id3.str[i + 6] << 7) + id3.str[i + 7];
            for (int j = i + 10; j < i + 10 + size; ++j)
                printf("%c", id3.str[j]);
            printf("\n");
            i += size + 9;
        }
    }
}

void set(id3v2 id3, char *prop_name, char *prop_value) {
    int pos = find(prop_name, id3);
    if (pos == -1) 
        printf("There's no such frame");
    else {
        printf("set %s - %s", prop_name, prop_value);
        int newsize = 11 + strlen(prop_value);
        int size = (id3.str[pos + 4] << 21) + (id3.str[pos + 5] << 14) + (id3.str[pos + 6] << 7) + id3.str[pos + 7] + 10;
        char *tmp_s = malloc(id3.size - (pos + size));
        memcpy(tmp_s, id3.str + pos + size, id3.size - (pos + size));
        for(int i = 0; i < 4; ++i)
            id3.str[pos + i] = prop_name[i];
        id3.str[pos + 4] = ((newsize - 10) >> 21) % 128;
        id3.str[pos + 5] = ((newsize - 10) >> 14) % 128;
        id3.str[pos + 6] = ((newsize - 10) >> 7) % 128;
        id3.str[pos + 7] = (newsize - 10) % 128;
        id3.str[pos + 8] = 0;
        id3.str[pos + 9] = 0;
        id3.str[pos + 10] = 0;
        for (int i = pos + 11; i < pos + newsize; ++i)
            id3.str[i] = prop_value[i - pos - 11];
        for (int i = pos + newsize; i < id3.size; ++i)
            id3.str[i] = tmp_s[i - pos - newsize];
        free(tmp_s);
        fseek(id3.file, 0, SEEK_SET);
        for (int i = 0; i < id3.size; ++i)
            fputc(id3.str[i], id3.file);
        
        }  
}

void dest(id3v2 id3) {
    fclose(id3.file);
    free(id3.str);
}