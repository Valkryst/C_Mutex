COMPILER = gcc
BASE_FLAGS = -std=c18 -Werror -Wall -Wshadow -c
DEBUG_FLAGS = -ggdb3
LIBRARY_FLAGS = -pthread

DIR_BIN = bin
DIR_BUILD = build
DIR_SRC = src

FILE_EXECUTABLE = mutex

FILES_C = $(wildcard $(DIR_SRC)/*.c)
FILES_H = $(wildcard $(DIR_SRC)/*.h)
FILES_O = $(subst src/,$(DIR_BUILD)/,$(subst .c,.o,$(FILES_C)))

release: COMPILER_FLAGS = $(BASE_FLAGS)
release: $(DIR_BIN)/$(FILE_EXECUTABLE)

debug: COMPILER_FLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS)
debug: $(DIR_BIN)/$(FILE_EXECUTABLE)

$(DIR_BIN)/$(FILE_EXECUTABLE): make_directories $(FILES_O)
	$(COMPILER) $(COMPILER_FLAGS) $(FILES_O) -o $@ $(LIBRARY_FLAGS)

$(DIR_BUILD)/logger.o: $(DIR_SRC)/logger.c $(FILES_H)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

$(DIR_BUILD)/mutex.o: $(DIR_SRC)/mutex.c $(FILES_H)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

make_directories:
	mkdir -p $(DIR_BIN)
	mkdir -p $(DIR_BUILD)

clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_BUILD)/*
	rm -f errors.txt
	rm -f valgrind.txt

run: $(DIR_BIN)/$(FILE_EXECUTABLE)
	$(DIR_BIN)/$(FILE_EXECUTABLE)

run_valgrind: $(DIR_BIN)/$(FILE_EXECUTABLE)
	rm -f valgrind.txt
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.txt $(DIR_BIN)/$(FILE_EXECUTABLE)