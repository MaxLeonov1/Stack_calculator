#include "cmd_asemblr.h"



int main ( int argc, char const *argv[] ) {

    const char* input_filename = "examples/input_test.txt"; //TODO ввод названия из командной строки
    const char* byte_filename  = "examples/perform.bin";

    Proc_Err_t  proc_status  = Proc_Err_t::PRC_SUCCSESFUL;

    proc_status = RunAssmblr ( input_filename, byte_filename );
    ProcErrHandler ( proc_status );

    return 0;
}
