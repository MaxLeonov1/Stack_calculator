#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "support_functions.h"
#include "stack_creation.h"
#include "colors.h"



void ProcErrHandler ( Proc_Err_t proc_status ) {

    switch ( proc_status ) {

        case Proc_Err_t::ERR_IN_STACK_TERMINATION:
        {
            printf ( "Error in stack\n" );
            //StackDump ( stack );
            break;
        }
        case Proc_Err_t::FILE_OPEN_ERR:
        {
            printf ( "Unable to open file\n" );
            break;
        }
        case Proc_Err_t::INCOR_ARG_NUM_ERR:
        {
            printf ( "Command have incorrect number of arguments\n" );
            break;
        }
        case Proc_Err_t::MEM_ALLOCATE_ERR:
        {
            printf ( "Unable to allocate memory\n" );
            break;
        }
        case Proc_Err_t::UNDEF_COMAND_ERR:
        {
            printf ( "Incorrect command found\n" );
            break;
        }
        case Proc_Err_t::UNDEF_REGISTR_NUM_ERR:
        {
            printf ( "Appeal to not existing register\n" );
            break;
        }
        case Proc_Err_t::INCOR_ARG_ERR:
        {
            printf ( "Provided argument to command - incorrect\n" );
        }
        case Proc_Err_t::PRC_SUCCSESFUL:
        {
            break;
        }
        default:
        {
            break;
        }

    } 

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackErrorHandler ( Stack_t* stack, bool is_dump ) {

    #ifdef DEBUG
        size_t min_size = 1;
    #else
        size_t min_size = 0;
    #endif //DEBUG

    if ( stack->data == nullptr ) {

        if ( is_dump ) StackDump ( stack );
        return Stack_Err_t::STACK_ALLOCATION_ERROR;

    }

    if ( stack->capacity <= 0 ) {

        if ( is_dump ) StackDump ( stack );
        return Stack_Err_t::ZERO_CAPACITY_ERROR;

    }

    if ( stack->size < min_size ) {

        if ( is_dump ) StackDump ( stack );
        return Stack_Err_t::ZERO_SIZE_ERROR;


    }

    #ifdef DEBUG

    if ( stack->data[0] != CANARY_NUM && stack->data[stack->capacity + 1] != CANARY_NUM ) {

        if ( is_dump ) StackDump ( stack );
        return Stack_Err_t::SIZE_OVERFLOW_ERROR;
    
    }

    #endif //DEBUG

    return Stack_Err_t::STK_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

void StackDump ( Stack_t* stack ) {

    printf ( "=========================\n"
             "stack [%s%p%s]\n"
             "-------------------------\n"
             "size = %s%d%s\n"
             "capacity = %s%d%s\n"
             "data [%s%p%s]\n"
             "-------------------------\n",
             BLUE, stack, RES_COL,
             BLUE, stack->size, RES_COL,
             BLUE, stack->capacity, RES_COL,
             BLUE, stack->data, RES_COL );

    if ( stack->data != nullptr ) {

        PrintStackElements ( stack );

    }

    printf ( "=========================\n" );

}

/*-----------------------------------------------------------------------------------------------*/

void PrintStackElements ( Stack_t* stack ) {

    for ( size_t ind = 0; ind < stack->capacity; ind++) {

        if ( ind < stack->size ) {

            printf ( "* [%d] = %s%lf%s %s%s%s\n", 
                     ind,
                     BLUE, stack->data[ind], RES_COL,
                     RED, DataSpecialParamHandler ( stack->data[ind] ), RES_COL );

        } else {

            printf ( "  [%d] = %s%lf%s %s%s%s\n",
                     ind,
                     BLUE, stack->data[ind], RES_COL,
                     RED, DataSpecialParamHandler ( stack->data[ind] ), RES_COL );

        }
            
    }

}

/*-----------------------------------------------------------------------------------------------*/

const char* DataSpecialParamHandler ( STK_ELM_TYPE param ) {

    if      ( DoubleCompare ( param, CANARY_NUM ) ) return "(CANARY)";
    else if ( DoubleCompare ( param, POISON_NUM ) ) return "(POISON)";
    else return " ";   

}