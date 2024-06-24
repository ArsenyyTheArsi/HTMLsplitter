#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_TAG_LEN 100
#define MAX_FILE_NAME_LEN 256

// Structure to store information about a tag
typedef struct Tag {
    char name[MAX_TAG_LEN];
    int is_open; // 1 - opening tag, 0 - closing tag
} Tag;

// Function to add open tags to the beginning of the next fragment
void next_fragment_tags(char output[], Tag writtenTags[], int countTags, int *currentString_len, int *currentChar_index) {
    for (int i = 0; i < countTags; i++) {
        if (writtenTags[i].is_open) {
            strcat(output, writtenTags[i].name);
            *currentString_len += strlen(writtenTags[i].name);
            *currentChar_index += strlen(writtenTags[i].name);
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
            // Start of a tag
            currentTag[0] = currentChar;
            int index = 1;
            while ((currentChar = fgetc(sourceHTML)) != '>' && currentChar != EOF) {
                currentTag[index++] = currentChar;
            }
            currentTag[index] = '>';
            currentTag[index + 1] = '\0';

            // Check for closing tag
            if (currentTag[1] == '/') {
                // Closing tag
                strcpy(writtenTags[writtenTag_index].name, currentTag);
                writtenTags[writtenTag_index].is_open = 0;

                // Find the matching opening tag and remove it
                for (int j = writtenTag_index - 1; j >= 0; j--) {
                    if (strcmp(writtenTags[j].name, currentTag) == 0 && writtenTags[j].is_open) {
                        stackTags_len -= strlen(writtenTags[j].name);
                        writtenTags[j].is_open = 0;
                        break;
                    }
                }
            } else {
                // Opening tag
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
            // Character is not '<', add it to the string
            currentString[currentChar_index] = currentChar;
            currentChar_index++;
            currentString_len++;
        }

        // Check for exceeding the maximum fragment length
        if (currentString_len + stackTags_len > max_len) {
            // Create a new fragment
            make_html_fragment(currentString, &fragment_number);
            memset(currentString, '\0', sizeof(currentString));
            currentChar_index = currentString_len = 0;
            stackTags_len = 0; // Reset stackTags_len after creating a new fragment

            // Move open tags to the new fragment
            next_fragment_tags(currentString, writtenTags, countTags, &currentString_len, &currentChar_index);
        }
    }

    // If there is data left at the end, save it
    if (currentString_len > 0) {
        make_html_fragment(currentString, &fragment_number); 
    }
    return 1;
}

void test_msg_split_simple() {
    FILE *fp = fopen("test.html", "w");
    fprintf(fp, "<h1>Hello, world!</h1>");
    fclose(fp);

    fp = fopen("test.html", "r");
    assert(msg_split(fp, 10) == 1);
    fclose(fp);

    remove("test.html");
    remove("fragment1.html");
}

void test_msg_split_with_tags() {
    FILE *fp = fopen("test.html", "w");
    fprintf(fp, "<div><h1>Hello, world!</h1></div>");
    fclose(fp);

    fp = fopen("test.html", "r");
    assert(msg_split(fp, 15) == 1);
    fclose(fp);

    remove("test.html");
    remove("fragment1.html");
}

void test_msg_split_with_nested_tags() {
    FILE *fp = fopen("test.html", "w");
    fprintf(fp, "<div><p>Hello, world!</p></div>");
    fclose(fp);

    fp = fopen("test.html", "r");
    assert(msg_split(fp, 15) == 1);
    fclose(fp);

    remove("test.html");
    remove("fragment1.html");
}

void test_msg_split_with_long_text() {
    FILE *fp = fopen("test.html", "w");
    fprintf(fp, "<div>This is a very long text that should be split into multiple fragments. </div>");
    fclose(fp);

    fp = fopen("test.html", "r");
    assert(msg_split(fp, 20) == 1);
    fclose(fp);

    remove("test.html");
    remove("fragment1.html");
    remove("fragment2.html");
}

int main() {
    test_msg_split_simple();
    test_msg_split_with_tags();
    test_msg_split_with_nested_tags();
    test_msg_split_with_long_text();

    printf("All tests passed!\n");
    return 0;
}