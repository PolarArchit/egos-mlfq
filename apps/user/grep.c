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
        if (file_read(file_ino, offset, buf) == -1) {
            break;
        }
        buf[BLOCK_SIZE] = '\0';
        char* token = strtok(buf, "\n");    
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