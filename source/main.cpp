#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "support_functions.h"
#include "cmd_processor.h"
#include "cmd_asemblr.h"



int main ( int argc, char const *argv[] )
{

    const char* input_filename = "examples/circle.txt";
    const char* byte_filename  = "examples/perform_circle.bin";

    Stack_Err_t stack_status = Stack_Err_t::STK_SUCCSESFUL;
    Proc_Err_t  proc_status  = Proc_Err_t::PRC_SUCCSESFUL;

    ASSMBLR( ass_1 )

    proc_status = CmdAssmblr ( input_filename, byte_filename, &ass_1 );
    ProcErrHandler ( proc_status );

    proc_status = CmdAssmblr ( input_filename, byte_filename, &ass_1 );
    ProcErrHandler ( proc_status );

    proc_status = CmdProcessor ( byte_filename, stack_status );
    ProcErrHandler ( proc_status );

    return 0;
}
