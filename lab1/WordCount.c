#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

char* sum(const char *s1, const char *s2){
    const int len1 = strlen(s1);
    const int len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); 
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); 
    return result;
}

int file_size(FILE *file){
    //int prev = ftell(file);
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    //fseek(file, prev, SEEK_SET);
    rewind(file);
    return size;
}

int lines_counter(FILE *file){
    if (file_size(file) == 0)
      return 0;
    int lines = 0;
    while (!feof(file))
        if (fgetc(file) == '\n')
            lines++;
    
    lines++;

    rewind(file);
    return lines;
}

int words_counter(FILE *file){
    int words = 0;
    char c, prev = ' ';
    while ((c = fgetc(file)) != EOF){
        if(c ==' ' || c == '\n' || c == '\t')
            if(isspace(prev) == 0)
                    words++;
        prev = c;
    }

    if (isspace(prev) == 0)
        words++;
    
    rewind(file);
    return words;

}
void solve(const char *filename, const char* request){
    FILE *fin;
    fin = fopen(filename, "r");

        if (!fin)
            printf("A file with this name was not found\n");

        else
            if (!strcmp(request, "-l") || !strcmp(request, "--lines")){
                printf("lines - %d\n", lines_counter(fin));
            }
            else if (!strcmp(request, "-c") || !strcmp(request, "--bytes")){
                printf("bytes - %d\n", file_size(fin));
            }
            else if (!strcmp(request, "-w") || !strcmp(request, "--words")){
                printf("words - %d\n", words_counter(fin));
            }
            else
                printf("Wrong command, use -h or --help to see all command\n");

        fclose(fin);
}

int main(int argc, char* argv[])
{
    FILE *fin;
    if (argc == 2)
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
            printf(" -l, --lines output only the number of lines\n -c, --bytes output the file size in bytes\n -w, --words output the number of words");
        else
            printf("Where is a filename?");
        
    else if(argc == 3){
        solve(argv[2], argv[1]);
    }

    else if (argc >= 4){
        /*char* filename = argv[2];
        char* space = " ";
        for(int i = 3; i < argc; ++i){
            filename = sum(filename, space);
            filename = sum(filename, argv[i]);
        }*/
        char* filename = argv[argc - 1];
        for(int i = 1; i < argc - 1; ++i){
          solve(filename, argv[i]);
        }
        //solve(filename, argv[1]);
    }
    return 0;
}
