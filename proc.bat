@echo ==============================BEGIN=COMPL===============================================
g++ source/main.cpp source/cmd_processor.cpp source/calc_comands.cpp source/stack_creation.cpp ^
source/error_handler.cpp source/support_functions.cpp source/cmd_asemblr.cpp source/general_instructions.cpp ^
-c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal ^
-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs ^
-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -g -pipe -fexceptions ^
-Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security ^
-Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers ^
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 ^
-Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla
@echo ==============================BEGIN=LINK===============================================
g++ *.o -o proc.exe
@echo ===============================END=LINK================================================
@DEL *.o