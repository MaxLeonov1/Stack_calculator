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



int HashBinSearch ( Cmd_Instr* Instr, int instr_size, int hash ) {

    int left = 0;
    int right = instr_size - 1;

    while ( left <= right ) {

        int mid = ( left + right ) / 2;

        if ( Instr[mid].cmd_hash == hash )
            return mid;

        else if ( Instr[mid].cmd_hash < hash )
            left = mid + 1;
        
        else
            right = mid - 1;

    }

    return -1;

}



int djb2hash ( const char *str ) {

    int hash = 5381;
    int c = 0;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}