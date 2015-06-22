#include "../lib/helpers.h"

#define BUF_SIZE 4096

int main(int argc, char **argv) {
    char buf[BUF_SIZE + 1]; // +1 for delimiter
    int count = 0;
    while ((count = read_until(STDIN_FILENO, buf, BUF_SIZE + 1, '\n')) > 0) {
        buf[count - 1] = '\0';
        int spaces = 0, i = 0;

        /* counting number of spaces */
        for(i = 0; i < count; i++) {
            spaces += (buf[i] == ' ');
        }

        /* split every input line by spaces */
        char* args[spaces + 2]; // +1 for last piece & +1 for NULL
        for(i = 0, spaces = 0; i < count; i++) {
            if (buf[i] == ' ' || buf[i] == '\0') {
                int ptr = i;
                while(ptr > 0 && buf[ptr - 1] != ' ') ptr--;
                args[spaces] = malloc(sizeof(char) * (i - ptr));
                int len = 0;
                while(ptr < i) {
                    args[spaces][len] = buf[ptr];
                    len++, ptr++;
                }
                spaces++;
            }
        }
        args[spaces] = NULL;

        int res = spawn(args[0], args);
        if (res != 0) {
            return 1;
        }
    }
    return 0;
}
