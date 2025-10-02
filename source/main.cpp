#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "support_functions.h"
#include "cmd_processor.h"



int main ( int argc, char const *argv[] )
{

    //x^2 - 5x + 4

    STK_INIT ( stk_1 )
    Stack_Err_t stack_status = Stack_Err_t::STK_SUCCSESFUL;
    StackCtor ( &stk_1, 5 );
    //PrintStackElements ( &stk_1 );

    CmdAssmblr ( "examples/discr.txt", "examples/perform_discr.txt" );

    //printf ( "%lld\n", FileByteCount( "examples/perform_discr.txt" ) );

    CmdInterpreter ( "examples/perform_discr.txt", &stk_1, stack_status );

    //printf ( "%ld", stk_1.data[0] );
    StackDtor( &stk_1 );

    return 0;
}
