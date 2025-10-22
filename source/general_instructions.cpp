#include <stdlib.h>
#include <stdio.h>

#include "general_instructions.h"


void CreateInstructionsTable(Cmd_Instr* Cmd_Proc_Instr) {
    size_t total_commands = sizeof(Cmd_Interpret_Instr) / sizeof(Cmd_Interpret_Instr[0]);

    for (size_t i = 0; i < total_commands; i++) {
        int index = Cmd_Interpret_Instr[i].cmd_code;
        
        Cmd_Proc_Instr[index].def = Cmd_Interpret_Instr[i].def;
        Cmd_Proc_Instr[index].name = Cmd_Interpret_Instr[i].name;
        Cmd_Proc_Instr[index].cmd_code = Cmd_Interpret_Instr[i].cmd_code;
        Cmd_Proc_Instr[index].is_arg = Cmd_Interpret_Instr[i].is_arg;
    }
}