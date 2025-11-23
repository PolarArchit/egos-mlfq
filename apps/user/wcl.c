#include "app.h"
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        INFO("usage: wcl [FILE1] [FILE2] . . .");
        return -1;
    }

    int total_lines = 0;

    for (int i = 1; i < argc; i++) {
        int file_ino = dir_lookup(workdir_ino, argv[i]);
        if (file_ino < 0) {
            INFO("wcl: file %s not found", argv[i]);
            continue;
        }

        char buf[BLOCK_SIZE + 1];

        for (int offset = 0; ; offset++) {

            if (file_read(file_ino, offset, buf) == -1) {
                break;
            }

            buf[BLOCK_SIZE] = '\0';

            int len = strlen(buf);
            for (int k = 0; k < len; k++) {
                if (buf[k] == '\n') {
                    total_lines++;
                }
            }
        }
    }

    printf("%d\n\r", total_lines);
    return 0;
}