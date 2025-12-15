#pragma once

#include <stdio.h>

#include "error_handler.h"
#include "general_instructions.h"


typedef struct {

    const char* filename;
    long long   byte_num;
    long        line_num;
    long        max_line_size;

} File_Info_t;

#define INIT_FILE_INFO( name, filename ) File_Info_t name = { filename, 0, 0, 0};



long long FileByteCount               ( const char* filename );
void      FileGetInfo                 ( FILE* stream, File_Info_t* file_info );
int       DoubleCompare               ( double double_1, double double_2 );

int djb2hash      ( const char *str );
int HashBinSearch ( Cmd_Instr* Instr, int instr_size, int hash );
