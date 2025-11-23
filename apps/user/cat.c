#include "app.h"
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        INFO("usage: cat [FILE]");
        return -1;
    }

    int file_ino = dir_lookup(workdir_ino, argv[1]);
    if (file_ino < 0) {
        INFO("cat: file %s not found", argv[1]);
        return -1;
    }
    char buf[BLOCK_SIZE + 1];

    for (int offset = 0; ; offset++) {
        
        if (file_read(file_ino, offset, buf) == -1) {
            break;
        }
        buf[BLOCK_SIZE] = '\0';
        printf("%s", buf);
    }
    
    printf("\n\r"); 
    return 0;
}