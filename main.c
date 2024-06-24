#include <functions.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <html_file> <max_length>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int max_len;
    if ((sscanf(argv[2], "%d", &max_len) != 1) || ((max_len <= 0) && (max_len > 4096))) {
        fprintf(stderr, "Invalid maximum length.\n");
        return 1;
    }

    // Opening an HTML file
    FILE* sourceHTML = fopen(filename, "r");
    if (sourceHTML == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    // Splitting an HTML file
    if (msg_split(sourceHTML, max_len) == 1) {
        printf("HTML file split successfully.\n");
    } else {
        printf("Error: Split occurred within a tag or output exceeded max length.\n");
    }

    // Closing an HTML file
    fclose(sourceHTML);

    return 0;
}
