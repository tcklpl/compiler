CXX := g++
MINGW_CROSS_CXX := x86_64-w64-mingw32-g++
CXXFLAGS := -g
OFLAGS := -O2 -c -Wall -std=c++17
TARGET := compilador.exe
DIST_TARGET_LINUX := compilador_linux.exe
DIST_TARGET_WIN64 := compilador_win64.exe

SRC_DIR := src
OBJ_DIR := obj
DST_DIR := dst
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
W64_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/win64/%.o,$(SRC_FILES))

.PHONY: all windows dist

dev: $(OBJ_DIR) $(TARGET)
windows: $(OBJ_DIR) $(DIST_TARGET_WIN64)

dist: $(DST_DIR) $(DIST_TARGET_LINUX) $(DIST_TARGET_WIN64)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DIST_TARGET_LINUX): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(DST_DIR)/$@ $^

$(DIST_TARGET_WIN64): $(W64_OBJ_FILES)
	$(MINGW_CROSS_CXX) $(CXXFLAGS) -static -o $(DST_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(OFLAGS) -o $@ $<

$(OBJ_DIR)/win64/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(MINGW_CROSS_CXX) $(OFLAGS) -o $@ $<

$(DST_DIR):
	@mkdir -p $@

clean:
	@$(RM) $(TARGET_LINUX)
	@$(RM) $(TARGET_WIN64)
	@$(RM) -r $(OBJ_DIR)
	@$(RM) -r $(DST_DIR)
