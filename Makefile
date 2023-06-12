LIBS=-lGL -lglfw -lGLEW -lassimp
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp Player.cpp CollidableObj.cpp Floor.cpp teapotAndCube.cpp MapLoader.cpp Map.cpp Wall.cpp Altar.cpp Skull.cpp load_obj.cpp Exit.cpp

main_file: $(FILES)
	g++ -g -o main_file $(FILES)  $(LIBS) -I.
