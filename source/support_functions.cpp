#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>

#include "support_functions.h"



long long FileByteCount ( const char* filename ) {

    struct stat file_info = {};

    stat ( filename, &file_info );

    return file_info.st_size;

}



void FileGetInfo ( FILE* stream, File_Info_t* file_info ) {

    int  symbol = 0;
    long line_size = 0;
    long max_line_size = 0;
    long line_num = 1;
    file_info->byte_num = FileByteCount ( file_info->filename );

    while ( ( symbol = getc(stream) ) != EOF ) {

        line_size++;

        if ( symbol == '\n' ) {

            line_num++;
            max_line_size = ( max_line_size > line_size ) ? max_line_size : line_size;
            line_size = 0;

        }

    }

    file_info->line_num = line_num;
    file_info->max_line_size = max_line_size;

    fseek( stream, 0, SEEK_SET );

}



int DoubleCompare (double double_1, double double_2) {

    const double error = 1e-6;

    return (fabs (double_1 - double_2) < error) ? 1 : 0;
}
