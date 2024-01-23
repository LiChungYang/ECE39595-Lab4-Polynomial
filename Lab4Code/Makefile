CC = g++
FLAGS = -g -Wall -pthread
MAIN_NAME = poly
EXECUTABLE = test_$(MAIN_NAME)
SOURCE_FILE = $(MAIN_NAME).cpp parser.cpp
TEST_FILE = $(EXECUTABLE).cpp


#rules:
#if want to compile own test_poly file, run make
$(EXECUTABLE): $(SOURCE_FILE) $(TEST_FILE)
	$(CC) $(SOURCE_FILE) $(TEST_FILE) $(FLAGS) -o $(EXECUTABLE)

#if want to compile main file, run make main 
main: $(SOURCE_FILE) main.cpp
	$(CC) $(SOURCE_FILE) main.cpp $(FLAGS) -o main

# if want to execute main, run make_main
time: main
	./main

#if want to execute test_poly, run make test
test: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

valgrind:
	valgrind ./$(EXECUTABLE)
.PHONY:test clean valgrind time
