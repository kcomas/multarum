
#include "token.h"

token_state *tokenize_file(const char *filename, char **const err) {
    struct stat s;
    if (stat(filename, &s) == -1) {
        *err = "File Not Found";
        return NULL;
    }
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) {
        *err = "Cannot Open File";
        return NULL;
    }
}
