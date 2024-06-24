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
