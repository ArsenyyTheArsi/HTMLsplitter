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

// if file without tags
void WithoutTag() {
    const char* HTMLcontent = "Text without tags";

    ftemp_create("testWT.html", HTMLcontent);

    int res = msg_split(fopen("testWT.html", "r"), 100); // if max_len < tag length

    if (res != 1) {
        printf("Separation without tags is successful\n");
        exit(0);
    }
}

// if file has long tag
void longTag() {
    const char* HTMLcontent = "<div>";
    const int TagLen = 10000; // tag lenght
    char* closeTag = "</div>";
    char* LargeHTML = malloc(strlen(HTMLcontent) + TagLen + strlen(closeTag) + 1);

    if (LargeHTML == NULL) {
        perror("Error, failed to allocate memory");
        exit(0);
    }

    strcpy(LargeHTML, HTMLcontent);

    for (int i = 0; i < TagLen; ++i) {
        strcat(LargeHTML, "a");
    }

    strcat(LargeHTML, closeTag);

    ftemp_create("testLST.html", LargeHTML); 

    int res = msg_split(fopen("testLST.html", "r"), 1000);// if max_len < tag length

    if (res != 1) {
        printf("Successfully separated by a large tag\n");
        exit(0);
    }
}


// test, if empty file
void EmptyFile() {

    ftemp_create("testEF.html", "");

    int res = msg_split(fopen("testEF.html", "r"), 100);// if max_len < tag length

    if (res != 1) {
        printf("Separation without tags is successful\n");
        exit(0);
    }
}

int main() {
    setlocale(LC_ALL, "rus");

    WithoutTag();
    longTag();
    EmptyFile();

    printf("success!");

    return 0;
}
