LIBS=-lGL -lglfw -lGLEW -lassimp
FILES=lodepng.cpp  main_file.cpp shaderprogram.cpp Player.cpp CollidableObj.cpp Floor.cpp MapLoader.cpp Map.cpp Wall.cpp Altar.cpp Skull.cpp Exit.cpp

main_file: $(FILES)
	g++ -g -o main_file $(FILES)  $(LIBS) -I.
