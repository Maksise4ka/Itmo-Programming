#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int key;
    unsigned char c;
    struct node *left, *right;
} node;

// int file_size(FILE *file){
//     fseek(file, 0L, SEEK_END);
//     int size = ftell(file);
//     rewind(file);
//     return size;
// }

void printbyte(unsigned char c) {
    int str[8] = {0};
    int l = 0;
    while (c != 0) {
        str[l] = c % 2;
        c /= 2;
        l++;
    }
    for(int i = 7; i >= 0; --i) {printf("%d", str[i]);}
}

int codebyte(char *str) {
    int c = 0; int exp = 0; int cum = 1;
    int size = strlen(str);
    for(int i = size - 1; i >= 0; --i) {
        exp = str[i] == '1';
        //printf("%d  - ", exp);
        c += exp * cum;
        //printbyte(c);
        //printf(",");
        cum *= 2;
    }   
    return c;
}

void sort(node **root, int n) {
    int f = 1;
    while (f) {
        f = 0;
        for (int i = 0; i < n - 1; ++i) {
            if (root[i]->key < root[i + 1]->key) {
                node* c = (node* )malloc(sizeof(node));
                c = root[i];
                root[i] = root[i + 1];
                root[i + 1] = c;
                f = 1;
            }
        }
    }
}

node* join(node* left, node* right) {
    node* res = (node*) malloc(sizeof(node));
    res->right = right;
    res->left = left;
    res->key = right->key + left->key;
    res->c = -1;
    return res;
};

void next(node* vertex, char* string, int strSize, char **dict){
    if (vertex->right == NULL) {
        strcpy(dict[(int) vertex->c], string);
        return;
    }
    string[strSize + 1] = '\0';
    string[strSize] = '1';
    next(vertex->right, string, strSize + 1, dict);
    string[strSize] = '0';
    next(vertex->left, string, strSize + 1, dict);
}

void Compression(char* filename) {
    FILE* file = fopen(filename, "rb");
    char** dict = (char**) malloc(256 * sizeof(char*));
    for (int i = 0; i < 256; ++i)
        dict[i] = (char*) calloc(256, sizeof(char));
    int* frequency = (int*) calloc(256, sizeof(int));
    int i, k = 0;
    unsigned char c = 0;
    node** tree = (node**) malloc(256 * sizeof(node*));
    int size = file_size(file);
    for(int i = 0; i < size; ++i) {
		c = fgetc(file);
        frequency[(unsigned char) c]++;
    }

	for (i = 0; i < 256; ++i) {
        if (frequency[i] != 0) {
            node *p = (node*) malloc(sizeof(node));
            p->c = (unsigned char)i;
            p->key = frequency[i];
            p->right = NULL;
            tree[k] = p;
            k++;
        }
    }
    sort(tree, k);
    while (k != 1) {
        sort(tree, k);
        tree[k-2] = join(tree[k - 2], tree[k - 1]);
        k--;
    }
    char* string = (char*)calloc(256, sizeof(char));
    int strSize = 0;
    next(tree[0], string, strSize, dict); int codesize = 0;

    rewind(file);
    char *filedata = (char*) malloc(size);
    for(int i = 0; i < size; ++i)
        filedata[i] = fgetc(file);
    fclose(file);
    
    FILE* outputFile = fopen(filename, "wb");
    for(int i = 0; i < 256; ++i) {
        codesize = strlen(dict[i]);
        fputc((char) codesize, outputFile);
        if (codesize) {
            if (codesize > 8) {
                fputc(codebyte(dict[i]) / 256, outputFile);
            }
            fputc(codebyte(dict[i]) % 256, outputFile);
        }
    }
    int full = 0; c = 0; unsigned char c1 = 0; int expr;
    for(int i = 0; i < size; ++i) {
        strcpy(string, dict[(unsigned char) filedata[i]]);
        for(int j = 0; j < strlen(string); ++j) {
            expr = string[j] == '1';
            c1 = (unsigned char) ((c1 >> (7 - full)) | expr) << (7 - full);
            full++; 
            if (full == 8) {
                fputc(c1, outputFile);
                c1 = 0;
                full = 0;
            }
        }
    }
    if (full != 0)
        fputc(c1, outputFile);
    fclose(outputFile);
    free(dict);
    free(frequency);
    free(tree);
}

int checkin(char *str, char **dict) {
    for(int i = 0; i < 256; ++i) {
        if (!strcmp(str, dict[i]))
            return i;
    }
    return -1;
}

void Decompression(char *output) {
    FILE *file = fopen(output, "rb");
    int size = file_size(file);
    char** dict = (char**) malloc(256 * sizeof(char*));
    for (int i = 0; i < 256; ++i)
        dict[i] = (char*) calloc(256, sizeof(char));
    int csize = 0; char c = 0; int k = 0; int r = 0; int tablesize = 0;
    for(int i = 0; i < 256; ++i) {
        csize = fgetc(file);
        tablesize++;
        if (csize != 0) {
            r = csize % 8;
            if(r) {
                c = fgetc(file);
                tablesize++;
                for(int j = r - 1; j >= 0; --j) {
                    dict[i][k] = ((c >> j) & (1)) + 48;
                    //printf("%c", dict[i][k]);
                    k++;
                }
            }
            for(int j = 0; j < csize - r; ++j) {
                if (j % 8 == 0) {
                    c = fgetc(file);
                    tablesize++;
                }
                dict[i][k] = ((c >> (7 - j)) & (1)) + 48;
                //printf("%c", dict[i][k]);
                k++;
            }
            dict[i][k] = 0;
            //printf("\n");
        }
        else dict[i][0] = 0;
        k = 0;
    }
    // for(int i = 0; i < 256; ++i) {
    //     if (dict[i])
    //         printf("%c - %s\n", i, dict[i]);
    // }

    FILE *res = fopen("res.txt", "wb");
    size -= tablesize;
    c = 0; char *str = (char*) calloc(256, sizeof(char)); k = 0; int exp, pos;
    for(int i = 0; i < size; ++i) {
        c = fgetc(file);
        for(int j = 0; j < 8; ++j) {
            exp = (c >> (7 - j)) & 1;
            //printf("%d", exp);
            str[k] = exp + 48;                
            k++;
            pos = checkin(str, dict);
            if (pos != -1) {
                fputc(pos, res);
                memset(str, 0, 256);
                k = 0;
            }
        }
    }
    fclose(res);
    fclose(file);
}
