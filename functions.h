#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAG_LEN 100
#define BLOCK_TAGS_NUM 16
#define MAX_FILE_NAME_LEN 256

void next_fragment_tags(char output[], char writtenTags[1024][MAX_TAG_LEN], int countTags, int *currentString_len, int *currentChar_index);// Function to add open tags to the start of the next fragment
void make_html_fragment(char *output, int *fragment_number);// Function to create a new HTML fragment
