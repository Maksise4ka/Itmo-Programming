#include "funcs.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

int file_size(FILE *file){
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);
    return size;
}

int upper(int n) {
    if (n == 0)
        return 1;
    int ans = 0;
    while (n != 0) {
        ans++;
        n /= 10;
    }
    return ans;
}

char* sum(const char *s1, const char *s2){
    const int len1 = strlen(s1);
    const int len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); 
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); 
    return result;
}

void create(char *arcname, char *files[], int start, int stop) {
    int size = 2;
    FILE *archive = fopen(arcname, "wb");
    for(int i = start; i < stop; ++i) {
        FILE *cur_file = fopen(files[i],"r");
        if (cur_file) {
            size += strlen(files[i]) + upper(file_size(cur_file)) + 4;
            printf("Open %s successful\n", files[i]);
        }
        else {
            printf("Can't open %s\n", files[i]);
        }
        fclose(cur_file);
    }
    
    rewind(archive);
    fprintf(archive, "%d||", size);

    for(int i = start; i < stop; ++i) {
        FILE *cur_file = fopen(files[i], "rb");
        if (cur_file)
            fprintf(archive, "%d||%s||", file_size(cur_file), files[i]);
        fclose(cur_file);
    }

    for (int i = start; i < stop; ++i) {
        FILE *cur_file = fopen(files[i], "rb");
        if (cur_file) {
            int cur_size = file_size(cur_file);
            char c = fgetc(cur_file);
            while (!feof(cur_file)) {
                fputc(c,archive);    
                c = fgetc(cur_file);
            }
        }
        fclose(cur_file);
    }
    printf("Zipping successful\n");
    fclose(archive);
}

void list(char *arcname) {
    FILE *archive = fopen(arcname, "rb");
    if (!archive) {
        printf("Can't open archive %s", arcname);
        return;
    }
    int size = 0; int pos = 0;
    char filename[128]; int file_s;
    fscanf(archive, "%d||", &size);
    while (pos < size) {
        fscanf(archive, "%d||", &file_s);
        char c = fgetc(archive); int filelen = 0;
        while (c != '|') {
            filename[filelen] = c;
            filelen++;
            c = fgetc(archive);
        }
        c = fgetc(archive);
        pos = ftell(archive);
        printf("%s\n", filename);
        memset(filename, 0, 128);
    }
    fclose(archive);
}

void extract(char *arcname) {
    FILE *archive = fopen(arcname, "rb");
    if (!archive)
        printf("Can't open %s", arcname);
    int pos = 0, size = 0;
    char filenames[128][128]; int sizes[128] = {0};
    int k_files = 0;
    fscanf(archive, "%d||", &size);
    while (pos < size) {
        fscanf(archive, "%d||", &sizes[k_files]);
        char c = fgetc(archive); int filelen = 0;
        while (c != '|') {
            filenames[k_files][filelen] = c;
            filelen++;
            c = fgetc(archive);
        }
        c = fgetc(archive);
        pos = ftell(archive);
        printf("%s -- %d\n", filenames[k_files], sizes[k_files]);
        k_files++;
    }
    for(int i = 0; i < k_files; ++i) {
        FILE *cur_file = fopen(sum("res\\", filenames[i]), "wb");
        char c;
        for(int j = 0; j < sizes[i]; ++j) {
            c = fgetc(archive);
            fputc(c, cur_file);
            //if (c == 10)
                //j++;
        }
        fclose(cur_file);
    }
    fclose(archive);
}

int main (int argc, char *argv[]) {
    setlocale(LC_ALL,"RU");
    if (argc <= 3) {
        printf("Few Argument");
        return 0;
    }
    char *arcname;

    for(int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--file")) {
            arcname = argv[i + 1];
        }
        if (!strcmp(argv[i], "--create")) {
            create(arcname, argv, i + 1, argc);
            Compression(arcname);
        }
        if (!strcmp(argv[i], "--list")) {
            Decompression(arcname);
            list("res.txt");
        }
        if (!strcmp(argv[i], "--extract")) {
            Decompression(arcname);
            extract("res.txt");
        }
    }
    return 0;
}
