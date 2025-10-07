#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_processor.h"
#include "support_functions.h"
#include "stack_creation.h"
#include "error_handler.h"
#include "calc_comands.h"
#include "colors.h"


const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;



Proc_Err_t CmdProcessor ( const char* input_file_name,  Stack_Err_t stk_status ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    Cmd_Proc processor = {};

    StackCtor ( &processor.proc_stk, processor.stk_def_size );

    FILE* input_file = fopen ( input_file_name, "r" );
    if ( input_file == nullptr ) return Proc_Err_t::FILE_OPEN_ERR;

    status = ScanCmdToBuffer ( input_file, &processor );
    PROC_STATUS_CHECK

    status = ProcessCmds ( &processor, &stk_status );
    PROC_STATUS_CHECK

    free(processor.cmd_buffer);

    return status;
    
}



Proc_Err_t ProcessCmds ( Cmd_Proc* processor, Stack_Err_t* stk_status ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    while ( processor->cur_com_ind < processor->cmd_num ) {

        int cmd_code = (int)(processor->cmd_buffer[2*processor->cur_com_ind + 1]);
        STK_ELM_TYPE argument = processor->cmd_buffer[2*processor->cur_com_ind + 2];

        //printf ( "%d %ld\n", cmd_code, argument );

        *stk_status = CmdHandler ( processor, cmd_code, argument );
        if ( *stk_status != Stack_Err_t::STK_SUCCSESFUL ) return Proc_Err_t::ERR_IN_STACK_TERMINATION;

        processor->cur_com_ind++;

    }

    return status;

}



Stack_Err_t CmdHandler ( Cmd_Proc* processor , int cmd_code, STK_ELM_TYPE argument ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;
    //printf("%d\n", cmd_code);
    switch ( cmd_code ) {

        case InterpretCmds::PUSH:
        {
            status = StackPush ( &processor->proc_stk, argument );
            return status;
        }
        case InterpretCmds::POP: 
        {
            STK_ELM_TYPE value_pop = 0;
            status = StackPop ( &processor->proc_stk, &value_pop );
            return status;
        }
        case InterpretCmds::SUM:
        {
            status = StackSum ( &processor->proc_stk );
            return status;
        } 
        case InterpretCmds::SUB:
        {
            status = StackSub ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::DIV:
        {
            status = StackDiv ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::MULT:
        {
            status = StackMult ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::SQRT:
        {
            status = StackSqrt ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::POPR:
        {
            status = RegistrPop ( processor, argument );
            return status;
        }
        case InterpretCmds::PUSHR:
        {
            status = RegistrPush ( processor, argument );
            return status;
        }
        case InterpretCmds::IN:
        {
            status = StackIn ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::OUT:
        {
            status = StackOut ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::HLT:
        {
            printf( "[END OF PROGRAM]\n" );
            return status;
        }
        default: 
        {
            printf( "[UNKNOWN COMAND]\n" );
            return status;
        }

    }

    return status;

}



Proc_Err_t ScanCmdToBuffer ( FILE* input_file, Cmd_Proc* processor ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    long cmd_num  = 0;

    fscanf ( input_file, "%ld", &cmd_num );

    processor->cmd_num = cmd_num;

    processor->cmd_buffer = (STK_ELM_TYPE*) calloc ( cmd_num * 2 + 1, sizeof(STK_ELM_TYPE) );
    if ( processor->cmd_buffer == nullptr ) return Proc_Err_t::MEM_ALLOCATE_ERR;

    for ( long scan_ind = 1; scan_ind < cmd_num * 2 + 1; scan_ind++ ) {

        STK_ELM_TYPE element = 0;

        fscanf ( input_file, "%ld", &element );

        processor->cmd_buffer[scan_ind] = element;

        //printf ( "%ld\n", processor->cmd_buffer[scan_ind] );

    }

    return status;

}