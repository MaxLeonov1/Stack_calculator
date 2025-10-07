#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "support_functions.h"



long long FileByteCount ( const char* filename ) {

    struct stat file_info = {};

    stat ( filename, &file_info );

    return file_info.st_size;

}



int FileLineCount ( const char* filename ) {

    FILE* stream = fopen ( filename, "r" );
    if ( stream == nullptr ) return 0;

    int symbol   = 0;
    int line_num = 1;

    while ( ( symbol = getc(stream) ) != EOF ) {

        if ( symbol == '\n' ) line_num++;

    }

    fclose(stream);

    return line_num;

}