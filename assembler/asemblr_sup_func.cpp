#include "asemblr_sup_func.h"


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


