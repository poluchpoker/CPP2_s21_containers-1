CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c++17
TEST_FLAGS = -lgtest -pthread -lstdc++ -lsubunit -lm -lrt
TEST_TARGET = testing_exe

all: test

clean:
	@echo "Delete unnecessary files..."
	rm -rf *.o *.a *.out *.log $(TEST_TARGET)

test: clean
	@echo "(@_@) Testing code... (@_@)"
	@$(CC) $(CFLAGS) ./s21_containers/test_*.cpp ./s21_containersplus/test_*.cpp $(TEST_FLAGS) -o $(TEST_TARGET)
	./testing_exe

valgrind: clean test
	@echo "(0_0) Checking the code for leaks... (0_0)"
	@CK_FORK=no valgrind --vgdb=no --leak-check=full \
	--show-leak-kinds=all --track-origins=yes --log-file="valgrind.log" -v --verbose -q --quiet -s ./$(TEST_TARGET)

check_style:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Google Style    ┃"
	@echo "┗=========================================┛"
	@find . \( -name '*.h' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -n -style=Google

format_style:
	@echo "┏=========================================┓"
	@echo "┃  Format your code for Google Style      ┃"
	@echo "┗=========================================┛"
	@find . \( -name '*.h' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -i -style=Google
