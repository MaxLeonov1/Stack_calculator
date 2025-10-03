#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_processor.h"
#include "support_functions.h"
#include "error_handler.h"
#include "calc_comands.h"
#include "colors.h"


const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;


Proc_Err_t CmdInterpreter ( const char* input_file_name,
                            Stack_t* stack, Stack_Err_t stack_status ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );

    if ( input_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    while ( fgets ( cmd_line, max_cmd_line_size, input_file ) ) {

        int cmd_code = 0;
        long argument = 0;

        int elments = sscanf ( cmd_line, "%d %ld", &cmd_code, &argument );

        if ( elments == 0 ) continue;

        stack_status = InterpretCmdHandler ( stack, cmd_code, argument );

        if ( cmd_code == InterpretCmds::HLT ) return status;

    }
        
        if ( stack_status != Stack_Err_t::STK_SUCCSESFUL ) return Proc_Err_t::ERR_IN_STACK_TERMINATION;

    free(cmd_line);

    return status;
    
}



Proc_Err_t CmdProc ( const char* input_file_name,
                                 Stack_t* stack, Stack_Err_t stack_status ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    long long file_symb_num = FileByteCount ( input_file_name ); 

    if ( input_file == nullptr )
        return Proc_Err_t::FILE_OPEN_ERR;

    char* cmd_line = (char*) calloc ( file_symb_num + 5, sizeof(char) );

    fread ( cmd_line, sizeof(char), file_symb_num + 5, input_file );

    //цикл с strtok

    free(cmd_line);

    return status;
    
}



Stack_Err_t InterpretCmdHandler ( Stack_t* stack, int cmd_code, long argument ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;
    //printf("%d\n", cmd_code);
    switch ( cmd_code ) {

        case InterpretCmds::PUSH:
        {
            status = StackPush ( stack, argument );
            return status;
        }
        case InterpretCmds::POP: 
        {
            STK_ELM_TYPE value_pop = 0;
            status = StackPop ( stack, &value_pop );
            return status;
        }
        case InterpretCmds::SUM:
        {
            status = StackSum ( stack );
            return status;
        } 
        case InterpretCmds::SUB:
        {
            status = StackSub ( stack );
            return status;
        }
        case InterpretCmds::DIV:
        {
            status = StackDiv ( stack );
            return status;
        }
        case InterpretCmds::MULT:
        {
            status = StackMult ( stack );
            return status;
        }
        case InterpretCmds::SQRT:
        {
            status = StackSqrt ( stack );
            return status;
        }
        case InterpretCmds::OUT:
        {
            status = StackOut ( stack );
            return status;
        }
        case InterpretCmds::HLT:
        {
            printf( "[END OF PROGRAM]\n" );
            return status;
        }

    }

    return status;

}