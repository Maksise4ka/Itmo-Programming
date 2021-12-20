#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[]) {
    if (argc >= 5) {
        char *filename;
        char *output;
        int max_iter = -1;
        int dump_freq = 1;
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "--input"))
                filename = argv[i + 1];
            else if (!strcmp(argv[i], "--output"))
                output = argv[i + 1];
            else if (!strcmp(argv[i], "--max_iter"))
                max_iter = atoi(argv[i + 1]);
            else if (!strcmp(argv[i], "--dump_freq"))
                dump_freq = atoi(argv[i + 1]);
        }
        bmp img = init(filename);
        startGame(img, max_iter, dump_freq, output);
        //printf("%s\t%s\t%d\t%d", filename, output, max_iter, dump_freq);
    }
    else
        printf("Dolzhen bit minimum --input and --output");
    return 0;
}
//C:\Users\ASUS\Desktop\роги\Github\Itmo-Programming\lab5
//main.exe --input "beehive.bmp" --output "result2" --max_iter 30 --dump_freq 1