typedef struct id3v2 {
    FILE *file;
    int size;
    char *str;
} id3v2;

id3v2 init(char *);

int isFrame(char *s);

int find(char*, id3v2);

void get(char*, id3v2);

void show(id3v2);

void set(id3v2, char*, char *);

void dest(id3v2);