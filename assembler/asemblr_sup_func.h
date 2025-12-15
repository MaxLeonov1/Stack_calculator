#pragma once

#include "cmd_asemblr.h"
#include "../general/support_functions.h"



void      AsmCtor                     ( Cmd_Assemblr_t* assmblr );
void      AsmDtor                     ( Cmd_Assemblr_t* assmblr );
void      InsertSpecParamToBuffer     ( Cmd_Assemblr_t* assmblr );
void      SortByCodeInstructionsTable ( Cmd_Assemblr_t* assmblr );
void      SortByHashInstructionsTable ( Cmd_Assemblr_t* assmblr );
void      CopyAndHashCmdInstr         ( Cmd_Instr* Cmd_Instr_Def, Cmd_Assemblr_t* assmblr );
int compare ( const void *a, const void *b );
