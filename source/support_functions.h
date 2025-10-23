#ifndef __SUPP_FUNC__
#define __SUPP_FUNC__


#include "error_handler.h"
#include "cmd_asemblr.h"
#include "general_instructions.h"


typedef struct {

    const char* filename;
    long long   byte_num;
    long        line_num;
    long        max_line_size;

} File_Info_t;

#define INIT_FILE_INFO( name, filename ) File_Info_t name = { filename, 0, 0, 0};


void      AsmCtor                     ( Cmd_Assemblr_t* assmblr );
void      AsmDtor                     ( Cmd_Assemblr_t* assmblr );
void      InsertSpecParamToBuffer     ( Cmd_Assemblr_t* assmblr );
long long FileByteCount               ( const char* filename );
void      FileGetInfo                 ( FILE* stream, File_Info_t* file_info );
int       DoubleCompare               ( double double_1, double double_2 );
void      SortByCodeInstructionsTable ( Cmd_Assemblr_t* assmblr );
void      SortByHashInstructionsTable ( Cmd_Assemblr_t* assmblr );
void      CopyAndHashCmdInstr         ( Cmd_Instr* Cmd_Instr_Def, Cmd_Assemblr_t* assmblr );

int djb2hash      ( const char *str );
int HashBinSearch ( Cmd_Instr* Instr, int instr_size, int hash );
int compare       ( const void *a, const void *b );

#endif 