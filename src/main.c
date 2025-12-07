#include "stdio.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* fp = fopen("test/example.bss", "rb");


    if (!fp) {
        perror("fopen");
        return 0;
    }

    // Seek to end to determine file size
    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return 0;
    }

    long filesize = ftell(fp);
    if (filesize < 0) {
        perror("ftell");
        fclose(fp);
        return 0;
    }

    rewind(fp); // go back to beginning

    // Allocate buffer (+1 for null terminator)
    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        perror("malloc");
        fclose(fp);
        return 0;
    }

    // Read the file into the buffer
    size_t read_bytes = fread(buffer, 1, filesize, fp);
    if (read_bytes != filesize) {
        perror("fread");
        free(buffer);
        fclose(fp);
        return 0;
    }

    buffer[filesize] = '\0'; // null-terminate the string

    fclose(fp);


    lexer *lex = init_lexer(buffer);
    return 0;
}



