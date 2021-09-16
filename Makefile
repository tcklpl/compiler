CC = g++
CFLAGS	= -g -Wall
OFLAGS = -O2 -c -Wall
TARGET	= compilador.exe

SRC_DIR = src
OBJ_DIR = obj
SRC_FILE_TREE = $(subst .,,$(wildcard $(SRC_DIR)/*/.))
SRC_FILES = $(subst //,/,$(wildcard $(SRC_FILE_TREE)/*.cpp)) $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Tive que fazer isso daqui pra compilar no windows tbm
ifeq ($(OS),Windows_NT)
	MKDIR_ARGS = ""
    OFLAGS += -std=c++0x
    override RM = del
else
    MKDIR_ARGS = -p
endif

.PHONY: all $(OBJ_DIR) $(SRC_FILE_TREE)
.SILENT: $(OBJ_DIR) $(SRC_FILE_TREE)

all: $(SRC_FILE_TREE) $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(OFLAGS) -o $@ $<

$(OBJ_DIR):
	@mkdir $(MKDIR_ARGS) $@ ||:

$(SRC_FILE_TREE): $(OBJ_DIR)
	@mkdir $(MKDIR_ARGS) $(subst src/,obj/,$@) ||:

clean:
	@$(RM) $(TARGET)
	@$(RM) -r $(OBJ_DIR)
