#include <functions.h>

int msg_split(FILE* sourceHTML, int max_len) {
    fseek(sourceHTML, 0, SEEK_SET);
   
    char writtenTags[1024][MAX_TAG_LEN];//All the block tags written and readwill be stored there
    for (int i = 0; i < 1024; i++) {
        memset(writtenTags[i], '\0', MAX_TAG_LEN);
    }

    const char blockTags[BLOCK_TAGS_NUM][MAX_TAG_LEN] = {"<p>", "<b>", "<strong>", "<i>", "<ul>", "<ol>", "<div>", "<span>", 
                                                            "</p>", "</b>", "</strong>", "</i>", "</ul>", "</ol>", "</div>", "</span>"};//Block tags

    int stackTags_len, index, writtenTag_index, countTags;
    int open_yorn/*is tag open one yes or no(y or n)*/ = stackTags_len = countTags = writtenTag_index = 0;
    
    char currentChar;//The current read symbol
    char currentTag[MAX_TAG_LEN];
    char currentString[max_len + 1];//The current text read and stored
    memset(currentString, '\0', sizeof(currentString));//Fill the string with emptiness
    int currentChar_index, currentString_len;
    currentChar_index = currentString_len = 0;
    
    int fragment_number = 1;

    while ((currentChar = fgetc(sourceHTML)) != EOF) {
        /* If we reach the end of the file, write the last data to the html file */
        if (currentChar == '<') {
            open_yorn = 1;
            currentTag[index++] = currentChar;
        }
        /* If we see '>', we check whether the tag is a block tag and, if necessary, push it onto the stack */
        else if (currentChar == '>') {
            open_yorn = 0;
            currentTag[index] = currentChar;
            currentTag[index + 1] = '\0';

            int block_yorn = 0;//Is tag a block one yes or no(y or n)

            for (int i = 0; i < BLOCK_TAGS_NUM/2; i++) {//Run for opening tags index
                if (strcmp(blockTags[i], currentTag) == 0) {
                    strcpy(writtenTags[writtenTag_index], currentTag);
                    stackTags_len += strlen(currentTag);
                    countTags++;
                    writtenTag_index++;
                    block_yorn = 1;
                    break;
                }
            }
            
            for (int i = 8; i < BLOCK_TAGS_NUM; i++) {//Run for closing block tags index
                if (strcmp(blockTags[i], currentTag) == 0) {
                    block_yorn = 1;
                    if (writtenTag_index > 0 && strcmp(writtenTags[writtenTag_index - 1], blockTags[i-8]) == 0) {
                        writtenTag_index--;
                        countTags--;
                        stackTags_len -= strlen(blockTags[i-8]);
                        writtenTags[writtenTag_index][0] = '\0';
                    } else {
                        strcpy(writtenTags[writtenTag_index], currentTag);
                        stackTags_len += strlen(currentTag);
                        countTags++;
                        writtenTag_index++;
                    }
                    break;
                }
            }

            if ((max_len - currentString_len) > (strlen(currentTag) + stackTags_len)) {//If there is enough space for the tag, write it to the rest of the written tags
                strcat(currentString, currentTag);
                currentString_len += strlen(currentTag);
                currentChar_index += strlen(currentTag);
                memset(currentTag, '\0', MAX_TAG_LEN);
                index = 0;
            } else {
                for (int i = countTags - 1; i >= 0; i--) {//else close all the tags to finish writing to the file, there should have been a function that would close all the tags, but it somehow breaks the code
                    char temp[MAX_TAG_LEN*100] = "";
                    strcpy(temp, writtenTags[i]);
                    if (temp[1] != '/') {
                        for (int j = strlen(temp); j > 0; j--) {
                            temp[j] = temp[j - 1];
                        }
                        temp[1] = '/';
                    }
                    strcat(currentString, temp);
                    currentString_len += strlen(temp);
                }
                if (currentString_len > max_len) {
                    fprintf(stderr, "Error: Output exceeds maximum length.\n");
                    return -1;
                }
                make_html_fragment(currentString, &fragment_number);
                memset(currentString, '\0', sizeof(currentString));
                currentChar_index = currentString_len = 0;

                next_fragment_tags(currentString, writtenTags, countTags, &currentString_len, &currentChar_index);//Start the new file with the tags of the unfinished block

                if (currentTag[0] != '\0') {
                    strcat(currentString, currentTag);
                    currentString_len += strlen(currentTag);
                    currentChar_index += strlen(currentTag);
                    memset(currentTag, '\0', MAX_TAG_LEN);
                    index = 0;
                }
            }
        } else if (open_yorn == 1) {//Currently writing tag
            currentTag[index] = currentChar;
            index++;
        } else if (open_yorn == 0) {//Else writing text
            currentString[currentChar_index] = currentChar;
            currentChar_index++;
            currentString_len++;

            if (max_len - currentString_len <= stackTags_len) {
                for (int i = countTags - 1; i >= 0; i--) {//There should have been a function that would close all the tags, but it somehow breaks the code
                    char temp[MAX_TAG_LEN*100] = "";
                    strcpy(temp, writtenTags[i]);
                    if (temp[1] != '/') {
                        for (int j = strlen(temp); j > 0; j--) {
                            temp[j] = temp[j - 1];
                        }
                        temp[1] = '/';
                    }
                    strcat(currentString, temp);
                    currentString_len += strlen(temp);
                }
                if (currentString_len > max_len) {
                    fprintf(stderr, "Error: Output exceeds maximum length.\n");
                    return -1;
                }
                make_html_fragment(currentString, &fragment_number);
                memset(currentString, '\0', sizeof(currentString));
                currentChar_index = 0;
                currentString_len = 0;

                next_fragment_tags(currentString, writtenTags, countTags, &currentString_len, &currentChar_index);//Start the new file with the tags of the unfinished block
            }
        }
    }
    for (int i = countTags - 1; i >= 0; i--) {//There should have been a function that would close all the tags, but it somehow breaks the code
        char temp[MAX_TAG_LEN*100] = "";
        strcpy(temp, writtenTags[i]);
        if (temp[1] != '/') {
            for (int j = strlen(temp); j > 0; j--) {
                temp[j] = temp[j - 1];
            }
            temp[1] = '/';
        }
        strcat(currentString, temp);
        currentString_len += strlen(temp);
    }   
    if (currentString_len > max_len) {
                fprintf(stderr, "Error: Output exceeds maximum length.\n");
                return -1;
            }
    make_html_fragment(currentString, &fragment_number);//When EOF reached create fragments
    return 1;
}
