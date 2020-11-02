CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: main.o spec_graph.o  text_parser.o  prog_exec.o spec_repr.o
	$(CC) -o $(EXE) main.o spec_graph.o text_parser.o prog_exec.o spec_repr.o

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

main.o: spec_graph.h 
	$(CC) $(WARNING_FLAGS) -c main.c

spec_graph.o: spec_graph.c spec_graph.h  
	$(CC) $(WARNING_FLAGS) -c spec_graph.c

text_parser.o: text_parser.h text_parser.c spec_graph.h
	$(CC) $(WARNING_FLAGS) -c text_parser.c

prog_exec.o: prog_exec.c prog_exec.h spec_graph.h
	$(CC) $(WARNING_FLAGS) -c prog_exec.c

spec_repr.o: spec_repr.c spec_repr.h
	$(CC) $(WARNING_FLAGS) -c spec_repr.c

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html