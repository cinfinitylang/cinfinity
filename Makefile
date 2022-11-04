MAKEFLAGS += -silent

# Multi-system command(s) and other(s)
ifeq ($(OS), Windows_NT)
Make_dir   := mkdir
Remove_dir := rmdir
Slash      := \\
Print      := echo
endif

# Command(s) and folder(s) for the compilation
COMPILE         := g++ -Wall -std=c++2b -fmodules-ts
TO_BIN          := -o
TO_OBJ          := -c
C_INFINITY_NAME := C∞

SRC   := source
BUILD := BUILD

# Path(s) - (directories)
SRC_MAIN := .$(Slash)$(SRC)$(Slash)main.c++

new:
	@ $(Make_dir) .$(Slash)$(BUILD)
	@ $(COMPILE) ($TO_BIN) .$(Slash)$(BUILD)$(Slash)$(C_INFINITY_NAME) $(SRC_MAIN)
	@ $(Print) $(C_INFINITY_NAME): new { $(C_INFINITY_NAME) }

.PHONY:
clean:
	@ $(Remove_dir) .$(Slash)$(BUILD)
