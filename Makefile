#CC specifies which compiler we're using
CC = g++

#SOURCE_PATH
SRC_PATH = src

#INCLDUE_PATH
INC_PATH = include

#BIN_PATH
BIN_PATH = bin

#INCLUDE_PATHS specifies the main include path and the SDL2 path
INCLUDE_PATHS = -I.\mingw_dev_lib\include\SDL2 -I.\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L.\mingw_dev_lib\lib

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = minesweeper.exe

#This is the target that compiles our executable
$(BIN_PATH)\$(OBJ_NAME) : $(BIN_PATH)\main.o $(BIN_PATH)\loadTexture.o $(BIN_PATH)\timer.o $(BIN_PATH)\difficultyButton.o $(BIN_PATH)\flagCounter.o $(BIN_PATH)\minesweeper.o $(BIN_PATH)\menu.o $(BIN_PATH)\HUD.o $(BIN_PATH)\board.o $(BIN_PATH)\cell.o $(BIN_PATH)\button.o $(BIN_PATH)\UI_element.o
	$(CC) $^ $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $@

$(BIN_PATH)\main.o : $(SRC_PATH)\main.cpp $(INC_PATH)\minesweeper.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\minesweeper.o : $(SRC_PATH)\minesweeper.cpp $(INC_PATH)\minesweeper.h $(INC_PATH)\loadTexture.h $(INC_PATH)\boardDetails.h $(INC_PATH)\cell.h $(INC_PATH)\menu.h $(INC_PATH)\board.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\menu.o : $(SRC_PATH)\menu.cpp $(INC_PATH)\menu.h $(INC_PATH)\boardDetails.h $(INC_PATH)\difficultyButton.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@	

$(BIN_PATH)\board.o : $(SRC_PATH)\board.cpp $(INC_PATH)\board.h $(INC_PATH)\cell.h $(INC_PATH)\boardDetails.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@	

$(BIN_PATH)\HUD.o : $(SRC_PATH)\HUD.cpp $(INC_PATH)\HUD.h $(INC_PATH)\loadTexture.h $(INC_PATH)\timer.h $(INC_PATH)\flagCounter.h $(INC_PATH)\UI_element.h $(INC_PATH)\button.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@	

$(BIN_PATH)\cell.o : $(SRC_PATH)\cell.cpp $(INC_PATH)\cell.h $(INC_PATH)\button.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\difficultyButton.o : $(SRC_PATH)\difficultyButton.cpp $(INC_PATH)\difficultyButton.h $(INC_PATH)\button.h $(INC_PATH)\loadTexture.h $(INC_PATH)\boardDetails.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\button.o : $(SRC_PATH)\button.cpp $(INC_PATH)\button.h $(INC_PATH)\UI_element.h 
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\flagCounter.o : $(SRC_PATH)\flagCounter.cpp $(INC_PATH)\flagCounter.h $(INC_PATH)\loadTexture.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\timer.o : $(SRC_PATH)\timer.cpp $(INC_PATH)\timer.h $(INC_PATH)\loadTexture.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\UI_element.o : $(SRC_PATH)\UI_element.cpp $(INC_PATH)\UI_element.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

$(BIN_PATH)\loadTexture.o : $(SRC_PATH)\loadTexture.cpp $(INC_PATH)\loadTexture.h
	$(CC) -c $< $(INCLUDE_PATHS) -o $@