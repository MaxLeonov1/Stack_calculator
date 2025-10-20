#ifndef __ERR_HANDL__
#define __ERR_HANDL__

typedef enum {

    PRC_SUCCSESFUL = 0,
    UNDEF_COMAND_ERR = 1,
    INCOR_ARG_NUM_ERR = 2,
    FILE_OPEN_ERR = 3,
    ERR_IN_STACK_TERMINATION = 4,
    UNDEF_REGISTR_NUM_ERR = 5,
    MEM_ALLOCATE_ERR = 6,
    UNDEF_ARG_ERR = 7,
    INCOR_ARG_ERR = 8,

} Proc_Err_t;

typedef enum {

    STK_SUCCSESFUL = 0,  
    ZERO_CAPACITY_ERROR = 1,
    SIZE_OVERFLOW_ERROR = 2,
    STACK_ALLOCATION_ERROR = 3,
    ZERO_SIZE_ERROR = 4,

} Stack_Err_t;

/*--------------------------------------------------------------------------------*/
/*================ASSEMBLER=AND=PROCESSOR=ELEMENTS=CHECKERS=======================*/

#define STK_STATUS_CHECK if ( status != Stack_Err_t::STK_SUCCSESFUL ) return status;
#define PROC_STATUS_CHECK if ( status != Proc_Err_t::PRC_SUCCSESFUL ) return status;

#define REG_EXISTANCE_CHECK                                   \
if ( *arg_code >= 0 && *arg_code < assmblr->proc_reg_num )    \
    return Proc_Err_t::PRC_SUCCSESFUL;                        \
else return Proc_Err_t::UNDEF_REGISTR_NUM_ERR;                \

/*---------------------------------------------------------------------------------*/
/*=====================STACK=RELATED=====================*/
//#define DEBUG

const double POISON_NUM = 111111111;
const double CANARY_NUM = 666666666;

typedef double STK_ELM_TYPE;
/*-------------------------------------------------------*/

typedef struct {

    #ifdef DEBUG

    int   line_num;
    char* file_name;
    char* stack_name;

    long sum_elm_check;

    #endif

    STK_ELM_TYPE* data;
    size_t        size;
    size_t        capacity;

} Stack_t;

/*-------------------------------------------------------*/

#ifdef DEBUG

#define STK_INIT( name ) Stack_t name = { \
.line_num = __LINE__, \
.file_name = __FILE__, \
.stack_name = #name, \
.sum_elm_check = 0, \
.data = nullptr, \
.size = 0, \
.capacity = 0 };               

#else

#define STK_INIT( name ) Stack_t name = { \
.data = nullptr, \
.size = 0, \
.capacity = 0 };                   


#endif //DEBUG

/*-------------------------------------------------------*/
/*===================MAIN=FUNCTIONS======================*/

void        ProcErrHandler          ( Proc_Err_t proc_status );
void        PrintStackElements      ( Stack_t* stack );
const char* DataSpecialParamHandler ( STK_ELM_TYPE param );
Stack_Err_t StackErrorHandler       ( Stack_t* stack, bool is_dump );
void        StackDump               ( Stack_t* stack );

/*-------------------------------------------------------*/

#endif