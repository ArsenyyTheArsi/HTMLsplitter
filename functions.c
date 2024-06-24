#include <functions.h>

// Function to add open tags to the start of the next fragment
void next_fragment_tags(char output[], char writtenTags[1024][MAX_TAG_LEN], int countTags, int *currentString_len, int *currentChar_index) {
    for (int i = 0; i < countTags; i++) {
        if (writtenTags[i][1] != '/') {
            strcat(output, writtenTags[i]);
            *currentString_len += strlen(writtenTags[i]);
            *currentChar_index += strlen(writtenTags[i]);
        }
    }
}

// Function to create a new HTML fragment
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
