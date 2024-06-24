#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_TAG_LEN 100
#define MAX_FILE_NAME_LEN 256

// Структура для хранения информации о теге
typedef struct Tag {
    char name[MAX_TAG_LEN];
    int is_open; // 1 - открывающий тег, 0 - закрывающий
} Tag;

// Функция для добавления открытых тегов в начало следующего фрагмента
void next_fragment_tags(char output[], Tag writtenTags[], int countTags, int *currentString_len, int *currentChar_index) {
    for (int i = 0; i < countTags; i++) {
        if (writtenTags[i].is_open) {
            strcat(output, writtenTags[i].name);
            *currentString_len += strlen(writtenTags[i].name);
            *currentChar_index += strlen(writtenTags[i].name);
        }
    }
}

// Функция для создания нового фрагмента HTML
void make_html_fragment(char *output, int *fragment_number) { 
    char name[MAX_FILE_NAME_LEN];
    snprintf(name, MAX_FILE_NAME_LEN, "fragment%d.html", *fragment_number);
    FILE* outputFile = fopen(name, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error opening output file: %s\n", name);
        exit(EXIT_FAILURE);
    }
    fputs(output, outputFile);
    fclose(outputFile);
    (*fragment_number)++;
}

int msg_split(FILE* sourceHTML, int max_len) {
    fseek(sourceHTML, 0, SEEK_SET);

    Tag writtenTags[1024]; 
    for (int i = 0; i < 1024; i++) {
        memset(writtenTags[i].name, '\0', MAX_TAG_LEN);
        writtenTags[i].is_open = 0;
    }

    char currentChar; 
    char currentTag[MAX_TAG_LEN];
    char currentString[max_len + 1]; 
    memset(currentString, '\0', sizeof(currentString)); 
    int currentChar_index = 0;
    int currentString_len = 0;
    int stackTags_len = 0;
    int countTags = 0;
    int writtenTag_index = 0;

    int fragment_number = 1;

    while ((currentChar = fgetc(sourceHTML)) != EOF) {
        if (currentChar == '<') {
            // Начало тега
            currentTag[0] = currentChar;
            int index = 1;
            while ((currentChar = fgetc(sourceHTML)) != '>' && currentChar != EOF) {
                currentTag[index++] = currentChar;
            }
            currentTag[index] = '>';
            currentTag[index + 1] = '\0';

            // Проверка на закрывающий тег
            if (currentTag[1] == '/') {
                // Закрывающий тег
                strcpy(writtenTags[writtenTag_index].name, currentTag);
                writtenTags[writtenTag_index].is_open = 0;

                // Ищем соответствующий открывающий тег и удаляем его
                for (int j = writtenTag_index - 1; j >= 0; j--) {
                    if (strcmp(writtenTags[j].name, currentTag) == 0 && writtenTags[j].is_open) {
                        stackTags_len -= strlen(writtenTags[j].name);
                        writtenTags[j].is_open = 0;
                        break;
                    }
                }
            } else {
                // Открывающий тег
                strcpy(writtenTags[writtenTag_index].name, currentTag);
                writtenTags[writtenTag_index].is_open = 1;
                stackTags_len += strlen(currentTag);
            }

            writtenTag_index++;
            countTags++;

            strcat(currentString, currentTag);
            currentString_len += strlen(currentTag);
            currentChar_index += strlen(currentTag);

        } else {
            // Символ не является '<', добавляем в строку
            currentString[currentChar_index] = currentChar;
            currentChar_index++;
            currentString_len++;
        }

        // Проверка на превышение максимальной длины фрагмента
        if (currentString_len + stackTags_len > max_len) {
            // Создаем новый фрагмент
            make_html_fragment(currentString, &fragment_number);
            memset(currentString, '\0', sizeof(currentString));
            currentChar_index = currentString_len = 0;
            stackTags_len = 0; // обнуляем stackTags_len после каждого фрагмента
            countTags = writtenTag_index = 0;
            memset(writtenTags, 0, sizeof(writtenTags));
        }
    }

    // Создаем последний фрагмент
    make_html_fragment(currentString, &fragment_number);

    return fragment_number - 1;


int main() {
    FILE* sourceHTML = fopen("source.html", "r");
    if (sourceHTML == NULL) {
        fprintf(stderr, "Error opening source file: source.html");
        return EXIT_FAILURE;
    }

    int max_len = 500; // Максимальная длина фрагмента

    int num_fragments = msg_split(sourceHTML, max_len);
    printf("Number of fragments: 

    fclose(sourceHTML);
    return EXIT_SUCCESS;
