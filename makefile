COMPILER = g++-9
LINKER = g++-9
EXT = cpp
CFLAGS = -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Weffc++ -Wzero-as-null-pointer-constant -Wmain -Wfatal-errors -Wextra -Wall -std=c++17 -Wno-switch -Wno-unused-parameter -Wno-reorder -Wno-float-equal

LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
ABS_INC = -Isrc/
LOCAL_INC = -Isrc/ $(patsubst %,-I%,$(filter %/,$(wildcard $(SRC_DIR)*/)))
INC =  $(ABS_INC) $(LOCAL_INC)

SRC_DIR = src/
OBJ_DIR = obj/

SOURCES = $(wildcard $(SRC_DIR)*.$(EXT)) $(wildcard $(SRC_DIR)**/*.$(EXT))
OBJ = $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .$(EXT),.o,$(SOURCES)))

EXE_DIR = bin/
EXE = $(EXE_DIR)main

OBJ_FOLDERS = $(EXE_DIR) $(OBJ_DIR) $(subst -I$(SRC_DIR),$(OBJ_DIR),$(LOCAL_INC))

#################################################

all: before out

before:
	$(foreach f,$(OBJ_FOLDERS),mkdir -p $(f))

out: $(OBJ)
	$(LINKER) -o $(EXE) $(OBJ) $(LDFLAGS)

$(OBJ): $(OBJ_DIR)%.o : $(SRC_DIR)%.$(EXT)
	$(COMPILER) $(CFLAGS) $(INC) -c $< -o $@

debug:
	echo $(OBJ_FOLDERS)

.PHONY: clean
.SILENT: *.o out before

clean:
	rm -f $(OBJ_DIR)*.o $(OBJ_DIR)**/*.o $(EXE)
