# OBJS specifies which files to compile as part of the project
OBJS = main.cpp utils.cpp Game.cpp Snake.cpp LTexture.cpp


# CC specifies which complier to use
CC = g++

# Complier Flags specifies the additional compilation options we're using
# -w supress all warnings
COMPLIER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# OBJ_NAME specifies the name of the executable file
OBJ_NAME = snake.exe

# Target that complies the executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPLIER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) && ./$(OBJ_NAME)
