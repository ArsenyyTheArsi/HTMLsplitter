#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "message_split.h"

// Функция для чтения содержимого файла в строку
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(file_size + 1);
    if (!content) {
        perror("Не удалось выделить память");
        exit(EXIT_FAILURE);
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    fclose(file);
    return content;
}

int main(int argc, char *argv[]) {
    int max_len = MAX_LEN;
    char *filename = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                max_len = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Использование: %s -m max_len html_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Ожидался HTML файл после опций\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[optind];
    char *html = read_file(filename);

    char *error_message = NULL;
    int result = split_message(html, max_len, &error_message);

    if (result == SPLIT_ERROR_EXCEEDS_MAX_LEN) {
        fprintf(stderr, "Ошибка: %s\n", error_message);
        free(error_message);
        free(html);
        exit(EXIT_FAILURE);
    }

    free(html);
    return 0;
}