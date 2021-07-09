#CC specifies which compiler we're using
CC = g++

#SOURCE_PATH
SRC_PATH = src

#OBJECT_PATH
OBJ_PATH = obj

#BIN_PATH
BIN_PATH = bin

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I.\mingw_dev_lib\include\SDL2 -I.\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L.\mingw_dev_lib\lib

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main.exe

#This is the target that compiles our executable
$(BIN_PATH)\$(OBJ_NAME) : $(OBJ_PATH)\main.o $(OBJ_PATH)\UI_element.o
	$(CC) $^ $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $@

$(OBJ_PATH)\main.o : $(SRC_PATH)\main.cpp
	$(CC) -c $^ $(INCLUDE_PATHS) -o $@
	
$(OBJ_PATH)\UI_element.o : $(SRC_PATH)\UI_element.cpp
	$(CC) -c $^ $(INCLUDE_PATHS) -o $@