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



void AsmCtor ( Cmd_Assemblr_t* assmblr ) {

    assmblr->cmd_instr_num = sizeof(Cmd_Interpret_Instr)/sizeof(Cmd_Interpret_Instr[0]);

    assmblr->instr_sort_code = (Cmd_Instr*) calloc ( assmblr->max_cmd_instr_num, sizeof(Cmd_Instr) );
    assmblr->instr_sort_hash = (Cmd_Instr*) calloc ( assmblr->cmd_instr_num, sizeof(Cmd_Instr) );
    assmblr->instr_general   = (Cmd_Instr*) calloc ( assmblr->cmd_instr_num, sizeof(Cmd_Instr) );

}



void AsmDtor ( Cmd_Assemblr_t* assmblr ) {

    free ( assmblr->instr_general );
    free ( assmblr->instr_sort_code );
    free ( assmblr->instr_sort_hash );

}



void InsertSpecParamToBuffer ( Cmd_Assemblr_t* assmblr ) {

    assmblr->cmd_buffer[0] = assmblr->cmd_num;

}



void SortByCodeInstructionsTable ( Cmd_Assemblr_t* assmblr ) {

    for ( size_t i = 0; i < assmblr->cmd_instr_num; i++ ) {

        size_t index = assmblr->instr_general[i].cmd_code;
        assmblr->instr_sort_code[index] = assmblr->instr_general[i];

    }

}



int compare ( const void* cmd_1, const void* cmd_2 ) {
    
    int hash_1 = ((Cmd_Instr*)cmd_1)->cmd_hash;
    int hash_2 = ((Cmd_Instr*)cmd_2)->cmd_hash;

    return hash_1 - hash_2;

}

void SortByHashInstructionsTable ( Cmd_Assemblr_t* assmblr ) {

    for ( size_t i = 0; i < assmblr->cmd_instr_num; i++ )
        assmblr->instr_sort_hash[i] = assmblr->instr_general[i];

    qsort ( assmblr->instr_sort_hash, assmblr->cmd_instr_num, sizeof(assmblr->instr_sort_hash[0]), compare );

}



void CopyAndHashCmdInstr ( Cmd_Instr* Cmd_Instr_Def, Cmd_Assemblr_t* assmblr ) {

    for ( size_t i = 0; i < assmblr->cmd_instr_num; i++ ) {
        
        assmblr->instr_general[i] = Cmd_Instr_Def[i];
        const char* name = assmblr->instr_general[i].name;
        assmblr->instr_general[i].cmd_hash = djb2hash(name);

    }

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