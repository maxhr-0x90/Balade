CC := gcc

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

MAIN := $(BIN_DIR)/fraude
PLAY := $(BIN_DIR)/playground
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall -pedantic -O2 -g
LDFLAGS  := -Llib
LDLIBS   := -lglut -lGLU -lGL -lm

.PHONY: all clean

all: $(MAIN) $(PLAY)

$(PLAY): $(filter-out %main.o, $(OBJ)) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(MAIN):  $(filter-out %playground.o, $(OBJ)) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)