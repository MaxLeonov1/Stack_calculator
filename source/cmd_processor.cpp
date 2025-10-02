#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_processor.h"
#include "error_handler.h"
#include "calc_comands.h"
#include "colors.h"


const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;


Proc_Err_t CmdAssmblr ( const char* input_file_name, const char* output_file_name ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    FILE* output_file = fopen ( output_file_name, "wb" );

    if ( input_file == nullptr || output_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    while ( fgets ( cmd_line, max_cmd_line_size, input_file ) ) {

        char* comand = (char*) calloc ( max_cmd_size, sizeof(char) );
        int cmd_code = 0;
        long argument = 0;

        int elments = sscanf ( cmd_line, "%s %ld", comand, &argument );

        if ( elments == 0 ) continue;

        status = CmdConvToCode ( comand, elments, &cmd_code );

        if ( status != Proc_Err_t::PRC_SUCCSESFUL )
            return status;

        fprintf ( output_file, "%d %ld\n", cmd_code, argument );

        free(comand);

    }

    free(cmd_line);
    fclose(input_file);
    fclose(output_file);

    return status;

} 



Proc_Err_t CmdConvToCode ( char* command, int elements, int* cmd_code ) {

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( command, Asmblr_Cmd_Instr[ind].name) == 0) { 

            *cmd_code = ind + 1;
            
            if ( (  Asmblr_Cmd_Instr[ind].is_arg && elements < 2 ) ||
                 ( !Asmblr_Cmd_Instr[ind].is_arg && elements > 1 ))
                return Proc_Err_t::INCOR_ARG_NUM_ERR;

            return Proc_Err_t::PRC_SUCCSESFUL;

        }

    }

    return Proc_Err_t::UNDEF_COMAND_ERR;

}



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



Proc_Err_t CmdInterpreter_TEST ( const char* input_file_name,
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
            // printf ( "[POP VALUE]: %s%ld%s", BLUE, value_pop, RES_COL );
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