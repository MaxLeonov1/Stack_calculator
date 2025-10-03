CFLAGS=-c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal \
	   -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
	   -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -g -pipe -fexceptions \
	   -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
	   -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	   -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo \
	   -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits \
	   -Wwrite-strings -Werror=vla

SOURCES = main.cpp cmd_processor.cpp calc_comands.cpp stack_creation.cpp error_handler.cpp \
support_functions.cpp cmd_asemblr.cpp

OBJDIR = obj
CXX = g++
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE = calculator

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(OBJDIR)/%.o: source/%.cpp
	@ $(CXX) $(CFLAGS) -c $< -o $@

clean:
	@ rm -rf $(OBJDIR)