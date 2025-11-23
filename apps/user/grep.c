#include "app.h"
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        INFO("usage: grep [PATTERN] [FILE]");
        return -1;
    }
    
    int file_ino = dir_lookup(workdir_ino, argv[2]);
    if (file_ino < 0) {
        INFO("grep: file %s not found", argv[2]);
        return -1;
    }

    char buf[BLOCK_SIZE + 1];
    int offset = 0;
    while (1) {
        if (file_read(file_ino, offset, buf) == -1) { // if the offset is too large, it returns -1 and also prints !!TDERROR OFFSET TOO LARGE, which should be ignored
            break;
        }
        buf[BLOCK_SIZE] = '\0';
        char* token = strtok(buf, "\n");    // split on line, and check every line line is done newline character.
        while (token != NULL) {
            char* line = strstr(token, argv[1]);
            if (line != NULL) {
                printf("%s\n\r", token);
            }
            token = strtok(NULL, "\n");
        }
        offset += 1;
    }
    return 0;
}
