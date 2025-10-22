#ifndef __INSTRUCTIONS__
#define __INSTRUCTIONS__

/*==============================================================*/

typedef enum { //just for understanding cmd in handler cases

    NULL_CMD = 0,
    PUSH  = 1,
    POP   = 2,
    SUM   = 3,
    SUB   = 4,
    DIV   = 5,
    MULT  = 6,
    SQRT  = 7,
    OUT   = 8,
    HLT   = 9,
    IN    = 10,
    PAUSE = 11,
    JMP   = 12,
    JB    = 13,
    JBE   = 14,
    JA    = 15,
    JAE   = 16,
    JE    = 17,
    JNE   = 18,
    PRTS  = 19,
    CALL  = 20,
    RET   = 21,
    DRAW  = 22,
    SIN   = 23,
    COS   = 24,
    PUSHR = 33,
    POPR  = 34,
    PRTR  = 35,
    PUSHM = 65,
    POPM  = 66,

} InterpretCmds;

typedef struct {

    InterpretCmds def;
    const char* name;
    int cmd_code;
    int is_arg;
    int cmd_hash;

} Cmd_Instr;

static Cmd_Instr Cmd_Interpret_Instr[] = {

    {InterpretCmds::PUSH,  "PUSH",  1,  1, 0 },
    {InterpretCmds::POP,   "POP",   2,  0, 0 },
    {InterpretCmds::SUM,   "SUM",   3,  0, 0 },
    {InterpretCmds::SUB,   "SUB",   4,  0, 0 },
    {InterpretCmds::DIV,   "DIV",   5,  0, 0 },
    {InterpretCmds::MULT,  "MULT",  6,  0, 0 },
    {InterpretCmds::SQRT,  "SQRT",  7,  0, 0 },
    {InterpretCmds::OUT,   "OUT",   8,  0, 0 },
    {InterpretCmds::HLT,   "HLT",   9,  0, 0 },
    {InterpretCmds::IN,    "IN",    10, 0, 0 },
    {InterpretCmds::PAUSE, "PAUSE", 11, 0, 0 },
    {InterpretCmds::JMP,   "JMP",   12, 1, 0 },
    {InterpretCmds::JB,    "JB",    13, 1, 0 },
    {InterpretCmds::JBE,   "JBE",   14, 1, 0 },
    {InterpretCmds::JA,    "JA",    15, 1, 0 },
    {InterpretCmds::JAE,   "JAE",   16, 1, 0 },
    {InterpretCmds::JE,    "JE" ,   17, 1, 0 },
    {InterpretCmds::JNE,   "JNE",   18, 1, 0 },
    {InterpretCmds::PRTS,  "PRTS",  19, 0, 0 },
    {InterpretCmds::CALL,  "CALL",  20, 1, 0 },
    {InterpretCmds::RET,   "RET",   21, 0, 0 },
    {InterpretCmds::DRAW,  "DRAW",  22, 0, 0 },
    {InterpretCmds::SIN,   "SIN",   23, 0, 0 },
    {InterpretCmds::COS,   "COS",   24, 0, 0 },

    {InterpretCmds::PUSHR, "PUSHR", 33, 1, 0 },
    {InterpretCmds::POPR,  "POPR",  34, 1, 0 },
    {InterpretCmds::PRTR,  "PRTR",  35, 1, 0 },

    {InterpretCmds::PUSHM, "PUSHM", 65, 1, 0 },
    {InterpretCmds::POPM,  "POPM",  66, 1, 0 },

};


void CreateInstructionsTable(Cmd_Instr* Cmd_Proc_Instr);


#endif //__INSTRUCTIONS__