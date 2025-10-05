#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "support_functions.h"
#include "cmd_processor.h"
#include "cmd_asemblr.h"



int main ( int argc, char const *argv[] )
{

    //x^2 - 5x + 4

    Stack_Err_t stack_status = Stack_Err_t::STK_SUCCSESFUL;

    //PrintStackElements ( &stk_1 );

    CmdAssmblr ( "examples/input_test.txt", "examples/perform_test.txt" );

    CmdProcessor ( "examples/perform_test.txt", stack_status );

    //CmdInterpreter ( "examples/perform_discr.txt", &stk_1, stack_status );

    //printf ( "%ld", stk_1.data[0] );

    return 0;
}
