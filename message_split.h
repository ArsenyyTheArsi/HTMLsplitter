#ifndef MESSAGE_SPLIT_H
#define MESSAGE_SPLIT_H

#define MAX_LEN 4096

// Коды ошибок
#define SPLIT_SUCCESS 0
#define SPLIT_ERROR_EXCEEDS_MAX_LEN 1

// Фрагменты HTML
typedef struct HTMLPart {
    char *content;
    struct HTMLPart *next;
} HTMLPart;

// Функция для создания нового HTML фрагмента
HTMLPart* create_part(const char *content);

// Функция для освобождения памяти, выделенной под HTML фрагменты
void free_parts(HTMLPart *head);

// Функция для добавления контента в текущий фрагмент
char* append_content(char *fragment, const char *content, int length);

// Функция для разделения HTML сообщения на фрагменты
int split_message(const char *html, int max_len, char **error_message);

#endif // MESSAGE_SPLIT_H