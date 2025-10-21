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

    //printf("[%ld]\n", processor.ram_size);

    StackCtor ( &processor.proc_stk, processor.stk_def_size );
    StackCtor ( &processor.call_stk, processor.stk_def_size );

    FILE* input_file = fopen ( input_file_name, "rb" );
    if ( input_file == nullptr ) return Proc_Err_t::FILE_OPEN_ERR;

    status = ScanCmdToBuffer ( input_file, &processor );
    PROC_STATUS_CHECK

    status = ProcessCmds ( &processor, &stk_status );
    PROC_STATUS_CHECK

    free ( processor.ram );
    free ( processor.cmd_buffer );
    StackDtor ( &processor.proc_stk );
    StackDtor ( &processor.call_stk );

    return status;
    
}



Proc_Err_t ProcessCmds ( Cmd_Proc* processor, Stack_Err_t* stk_status ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    while ( processor->cur_com_ind < processor->cmd_num ) { //TODO сделать сдвиг зависящий от команды

        int cmd_code = (int)(processor->cmd_buffer[2*processor->cur_com_ind + 1]);
        STK_ELM_TYPE argument = processor->cmd_buffer[2*processor->cur_com_ind + 2];
        processor->cur_com_ind++; 

        //printf ( "%d\n", processor->cur_com_ind );
        //printf ( "%d %ld\n", cmd_code, argument );

        *stk_status = CmdHandler ( processor, cmd_code, argument );
        //printf ( "%s%d%s\n", BLUE, *stk_status, RES_COL );
        if ( *stk_status != Stack_Err_t::STK_SUCCSESFUL ) return Proc_Err_t::ERR_IN_STACK_TERMINATION;

        if ( cmd_code == InterpretCmds::HLT ) return status;

    }

    return status;

}



Stack_Err_t CmdHandler ( Cmd_Proc* processor , int cmd_code, STK_ELM_TYPE argument ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    switch ( cmd_code ) { //TODO структуры с указателями на функции

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
        case InterpretCmds::SIN:
        {
            status = StackSin ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::COS:
        {
            status = StackCos ( &processor->proc_stk );
            return status;
        }
        case InterpretCmds::POPR:
        {
            status = RegistrPop ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::PUSHR:
        {
            status = RegistrPush ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::PUSHM:
        {
            status = MemoryPush ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::POPM:
        {
            status = MemoryPop ( processor, (int)argument );
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
            //PrintStackElements(&processor->proc_stk);
            return status;
        }
        case InterpretCmds::HLT:
        {
            printf( "%s[END OF PROGRAM]%s\n", GREEN, RES_COL );
            return status;
        }
        case InterpretCmds::PAUSE:
        {
            //printf("aboba\n");
            PauseProc();
            return status;
        }
        case InterpretCmds::JMP:
        {
            status = JumpToCmd ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::JB:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::BELOW );
            return status;
        }
        case InterpretCmds::JBE:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::BELOW_AND_EQUAL );
            return status;
        }
        case InterpretCmds::JE:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::EQUAL );
            return status;
        }
        case InterpretCmds::JNE:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::NOT_EQUAL );
            return status;
        }
        case InterpretCmds::JA:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::GREATER );
            return status;
        }
        case InterpretCmds::JAE:
        {
            status = JumpIf ( processor, (int)argument, Cmd_Jump_t::GREATER_AND_EQUAL );
            return status;
        }
        case InterpretCmds::PRTR:
        {
            status = PrintRegValue ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::PRTS:
        {
            printf("==PROCESSOR_STACK==\n");
            PrintStackElements(&processor->proc_stk);
            printf("===================\n");
            return status;
        }
        case InterpretCmds::CALL:
        {
            status = CallCmd ( processor, (int)argument );
            return status;
        }
        case InterpretCmds::RET:
        {
            status = ReturnToCall ( processor );
            return status;
        }
        case InterpretCmds::DRAW:
        {
            status = DrawVideoMemory ( processor );
            return status;
        }
        case InterpretCmds::NULL_CMD:
        {
            printf ( "%s[NULL COMMAND]%s\n", YELLOW, RES_COL );
            return status;
        }
        default: 
        {
            printf( "%s[UNKNOWN COMMAND]%s\n", RED, RES_COL );
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

        fscanf ( input_file, "%lf", &element );

        processor->cmd_buffer[scan_ind] = element;

        //printf ( "%ld\n", processor->cmd_buffer[scan_ind] );

    }

    return status;

}