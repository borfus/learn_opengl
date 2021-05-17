#include <stdlib.h>
#include <string.h>
#include "file_util.h"
#include "opengl_util.h"

char *read_file_string(FILE *file) {
    fseek(file, 0, SEEK_END);
    long long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *string = malloc(length + 1L);
    if (string) {
        memset(string, '\0', length + 1L);
        fread(string, sizeof(char), length, file);
    }
    fclose(file);
    return string;
}
