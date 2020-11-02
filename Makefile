CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: main.o graph.o  reader.o  command_executor.o validator.o
	$(CC) -o $(EXE) main.o graph.o reader.o command_executor.o validator.o

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

main.o: graph.h 
	$(CC) $(WARNING_FLAGS) -c main.c

# struct_input.o:
# 	$(CC) $(WARNING_FLAGS) -c struct_input.c

# linked_list.o: linked_list.c linked_list.h
# 	$(CC) $(WARNING_FLAGS) -c linked_list.c

graph.o: graph.c graph.h  
	$(CC) $(WARNING_FLAGS) -c graph.c

# constants.o: constants.c constants.h
# 	$(CC) $(WARNING_FLAGS) -c constants.c

reader.o: reader.h reader.c graph.h
	$(CC) $(WARNING_FLAGS) -c reader.c

# option_parser.o:
# 	$(CC) $(WARNING_FLAGS) -c option_parser.c

# traversal.o: traversal.c traversal.h linked_list.h graph.h
# 	$(CC) $(WARNING_FLAGS) -c traversal.c

command_executor.o: command_executor.c command_executor.h graph.h
	$(CC) $(WARNING_FLAGS) -c command_executor.c

validator.o: validator.c validator.h
	$(CC) $(WARNING_FLAGS) -c validator.c

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