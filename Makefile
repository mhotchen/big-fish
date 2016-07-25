OBJS= src/*.cpp src/circle/*.h src/circle/*.cpp src/actor/*.h src/actor/*.cpp
CC= g++
CC_FLAGS= -std=c++14
CC_LINKER_FLAGS= -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I src/
OBJ_NAME= run

all: $(OBJS)
	$(CC) $(OBJS) $(CC_FLAGS) $(CC_LINKER_FLAGS) -o $(OBJ_NAME)
