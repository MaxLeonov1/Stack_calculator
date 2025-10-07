#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "support_functions.h"
#include "cmd_processor.h"
#include "cmd_asemblr.h"



int main ( int argc, char const *argv[] )
{

    const char* input_filename = "examples/input_test.txt";
    const char* byte_filename  = "examples/perform_test.txt";

    Stack_Err_t stack_status = Stack_Err_t::STK_SUCCSESFUL;
    Proc_Err_t  proc_status  = Proc_Err_t::PRC_SUCCSESFUL;

    CmdAssmblr ( input_filename, byte_filename );
    ProcErrHandler ( proc_status );

    CmdProcessor ( byte_filename, stack_status );
    ProcErrHandler ( proc_status );

    return 0;
}
