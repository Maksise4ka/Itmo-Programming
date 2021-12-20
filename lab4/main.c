#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

char* sum(const char *s1, const char *s2){
    const int len1 = strlen(s1);
    const int len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); 
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); 
    return result;
}

const char *space = " ";

int main(int argc, char *argv[]) {
    if (argc < 3)
        printf("Error");
    else {
        int pathlen = strlen(argv[1]) - 11;
        char *filename = (char*) malloc(pathlen * sizeof(char));
        filename = argv[1] + 11;
        id3v2 id3 = init(filename);
        if (id3.size != -1) {
            if (!strcmp(argv[2], "--show"))
                show(id3); 
            else {
                char request[10]; //выделил 5 лишних байт
                for(int i = 0; i < 5; ++i)
                    request[i] = argv[2][i];
                if (!strcmp(request, "--get")) {
                    char *prop_name = malloc(10);
                    if (strlen(argv[2]) == 10) {
                        for(int i = 0; i < 4; ++i)
                            prop_name[i] = argv[2][i + 6];
                        get(prop_name, id3);
                        free(prop_name);
                    }
                    else
                        printf("Incorrect frame");
                }
                else if (!strcmp(request, "--set")) {
                    if (argc >= 4) {
                        char *prop_name;
                        prop_name = malloc(10);
                        for(int i = 0; i < 4; ++i)
                            prop_name[i] = argv[2][i + 6];
                        char *prop_value = argv[3] + 8;
                        for(int i = 4; i < argc; ++i) {
                            prop_value = sum(prop_value, space);
                            prop_value = sum(prop_value, argv[i]);
                        }
                        set(id3, prop_name, prop_value);
                    }
                    else
                        printf("A lot of or few argument");
                }
                else 
                    printf("Incorrect command");
            }
        
        dest(id3);
        }
    }
    return 0;
}
//cd "c:\Users\ASUS\Desktop\роги\Github\Itmo-Programming\lab4\" ; if ($?) { gcc main.c struct.c -o main } ; if ($?) { .\main }