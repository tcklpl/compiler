# Compilers
CXX := g++
MINGW_CROSS_CXX := x86_64-w64-mingw32-g++
# Flags for final .exe generation.
# -g	Keeps Symbol table and debug metadata.
CXXFLAGS := -g
# Flags for individual .o generation
# -O2			Optimization.
# -c			Will only compile but not link files.
# -Wall			Enable all compiles warnings.
# -std=c++17	Compile using c++ 17 as the standard.
# -MMD			Adds user headers files to the dependencies.
# -MP			Adds empty dummy rules, to avoid errors if header files are removed.
OFLAGS := -O2 -c -Wall -std=c++17 -MMD -MP
# Debug executable name, will be generated at ./
TARGET := compiler.exe
# Dist executables names, will be generated at dist/
DIST_TARGET_LINUX := compiler_linux.exe
DIST_TARGET_WIN64 := compiler_win64.exe

# Directories
SRC_DIR := src
OBJ_DIR := obj
DST_DIR := dst
# Finds recursevily all .cpp files inside src/, this will not run on windows.
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
# Transform all 'src/<PATH>/<FILE>.cpp' into 'obj/<PATH>/<FILE>.o'
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
# Transform all '<FILE>.o' into '<FILE>.d", used for the dependencies.
DPD_FILES = $(patsubst %.o, %.d, $(OBJ_FILES))
# Same as OBJ_FILES, but will put all files inside 'obj/win64' instead of 'obj/'
W64_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/win64/%.o, $(SRC_FILES))

# 'dev' and 'dist' are not compiler targets, but rather recipe names.
.PHONY: dev dist

# Dev rule, will compile a linux version ./compiler.exe
dev: $(OBJ_DIR) $(TARGET)

# Distribution rule, will compile windows and linux versions inside /dst.
dist: $(DST_DIR) $(DIST_TARGET_LINUX) $(DIST_TARGET_WIN64)

# Rule to combine the dev g++ .o files.
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to combine all dist g++ .o files.
$(DIST_TARGET_LINUX): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(DST_DIR)/$@ $^

# Rule to combine all dist mingw .o files into a final executable. Uses static linking of library because windows.
$(DIST_TARGET_WIN64): $(W64_OBJ_FILES)
	$(MINGW_CROSS_CXX) $(CXXFLAGS) -static -o $(DST_DIR)/$@ $^

# Rule to compile each .cpp file and recompile if any of the dependencies change (header files included)
-include $(DPD_FILES)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(OFLAGS) -o $@ $<

# Rule to compile each .h, but using mingw for windows. This will also recompile is the dependencies change.
$(OBJ_DIR)/win64/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(MINGW_CROSS_CXX) $(OFLAGS) -o $@ $<

# Rule to create the directories /dst and /obj
$(DST_DIR) $(OBJ_DIR):
	@mkdir -p $@

# Rule to clean everything, removes all compiled files and directories.
clean:
	@$(RM) $(TARGET)
	@$(RM) -r $(OBJ_DIR)
	@$(RM) -r $(DST_DIR)
