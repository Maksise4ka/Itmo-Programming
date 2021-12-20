typedef struct bmp {
    int size;
    int height;
    int width;
    int offset;
    int hSize; // header size
    int bCount; // bit count, must be = 1
    FILE *file;
} bmp;

bmp init(char *);

void generate(bmp, char**);

void startGame(bmp, int, int, char*);

void to(bmp, char**);

void from(bmp, char**);

void dest(bmp *);