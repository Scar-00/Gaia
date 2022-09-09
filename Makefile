CC = clang
AR = ar -rcs
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-newline-eof -Wno-deprecated-declarations -DDLL_BUILD
CFLAGS += -Iinclude
LDFLAGS =

SRC = $(wildcard src/**/**/*.c) $(wildcard src/**/*.c) $(wildcard src/*.c) #$(wildcard include/Gaia/**/**/*.h) $(wildcard include/Gaia/**/*.h) $(wildcard include/Gaia/*.h)
OBJ = $(SRC:.c=.o)

static: $(OBJ)
	$(AR) libgaia.lib $^ $(LDFLAGS)

dynamic: $(OBJ)
	$(CC) -shared -o libgaia.dll $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< -MJ $@.json $(CFLAGS)

clean:
	rm -rf $(OBJ) libgaia.a libgaia.dll libgaia.lib libgaia.exp
