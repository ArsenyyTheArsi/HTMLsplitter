#include "message_split.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HTMLPart* create_part(const char *content) {
    HTMLPart *part = (HTMLPart *)malloc(sizeof(HTMLPart));
    part->content = strdup(content);
    part->next = NULL;
    return part;
}

void free_parts(HTMLPart *head) {
    HTMLPart *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->content);
        free(tmp);
    }
}

char* append_content(char *fragment, const char *content, int length) {
    int frag_len = fragment ? strlen(fragment) : 0;
    char *new_fragment = (char *)malloc(frag_len + length + 1);
    if (fragment) {
        strcpy(new_fragment, fragment);
        free(fragment);
    } else {
        new_fragment[0] = '\0';
    }
    strncat(new_fragment, content, length);
    new_fragment[frag_len + length] = '\0';
    return new_fragment;
}

int split_message(const char *html, int max_len, char **error_message) {
    HTMLPart *fragments = NULL;
    HTMLPart *tail = NULL;

    const char *pos = html;
    char *current_fragment = NULL;
    int total_len = 0;
    int fragment_number = 1;

    while (*pos) {
        const char *start = pos;
        int part_len = 0;

        if (*pos == '<') {
            while (*pos && *pos != '>') pos++;
            if (*pos == '>') pos++;
            part_len = pos - start;
        } else {
            while (*pos && *pos != '<') pos++;
            part_len = pos - start;
        }

        if (total_len + part_len > max_len) {
            if (part_len > max_len) {
                *error_message = (char *)malloc(256);
                snprintf(*error_message, 256, "Один тег или текстовый блок превышает максимальную длину в %d символов.", max_len);
                free(current_fragment);
                free_parts(fragments);
                return SPLIT_ERROR_EXCEEDS_MAX_LEN;
            }

            HTMLPart *fragment = create_part(current_fragment);
            if (fragments == NULL) {
                fragments = tail = fragment;
            } else {
                tail->next = fragment;
                tail = fragment;
            }
            fragment_number++;
            current_fragment = NULL;
            total_len = 0;
        }

        current_fragment = append_content(current_fragment, start, part_len);
        total_len += part_len;
    }

    if (current_fragment) {
        HTMLPart *fragment = create_part(current_fragment);
        if (fragments == NULL) {
            fragments = tail = fragment;
        } else {
            tail->next = fragment;
            tail = fragment;
        }
    }

    HTMLPart *current = fragments;
    int seq = 1;
    while (current != NULL) {
        printf("Фрагмент №%d:\n%s\n\n", seq++, current->content);
        current = current->next;
    }

    free_parts(fragments);
    return SPLIT_SUCCESS;
}
