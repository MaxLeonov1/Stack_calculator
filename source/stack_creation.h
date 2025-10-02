#ifndef __STK_CRT__
#define __STK_CRT__

/*-------------------------------------------------------*/
#include <stdlib.h>

#include "error_handler.h"
/*-------------------------------------------------------*/



Stack_Err_t StackCtor ( Stack_t* stack, size_t capacity );
Stack_Err_t StackDtor ( Stack_t* stack );


#endif