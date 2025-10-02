#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "colors.h"



void PrintStackElements ( Stack_t* stack ) {

    for ( size_t ind = 0; ind < stack->capacity; ind++) {

        if ( ind < stack->size ) {

            printf ( "* [%d] = %s%d%s %s%s%s\n", 
                     ind,
                     BLUE, stack->data[ind], RES_COL,
                     RED, DataSpecialParamHandler ( stack->data[ind] ), RES_COL );

        } else {

            printf ( "  [%d] = %s%d%s %s%s%s\n",
                     ind,
                     BLUE, stack->data[ind], RES_COL,
                     RED, DataSpecialParamHandler ( stack->data[ind] ), RES_COL );

        }
            
    }

}

/*-----------------------------------------------------------------------------------------------*/

const char* DataSpecialParamHandler ( STK_ELM_TYPE param ) {

    switch ( param ) {

        case CANARY_NUM:
            return "(CANARY)";

        case POISON_NUM:
            return "(POISON)";

        default:
            return " ";

    }

}