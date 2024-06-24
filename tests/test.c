#define _CRT_SECURE_NO_WARNINGS
#include "msg_split.h"
#include "functions.h"
#include <locale.h>

// create temp file 
void ftemp_create(const char* filename, const char* content) {

    FILE* Ftmp = fopen(filename, "w"); 

    if (Ftmp == NULL) {
        perror("File opening error");
        exit(0);
    }

    fputs(content, Ftmp);
    fclose(Ftmp);
}


...

int main() {
    setlocale(LC_ALL, "rus");

    WithoutTag();
    longTag();
    EmptyFile();

    printf("success!");

    return 0;
}
