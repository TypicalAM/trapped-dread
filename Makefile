LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  lodepng.h  shaderprogram.h Player.h common.h CollidableObj.h Floor.h MapLoader.h Map.h Wall.h load_obj.h Altar.h Skull.h load_obj.h Exit.h
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp Player.cpp CollidableObj.cpp Floor.cpp teapotAndCube.cpp MapLoader.cpp Map.cpp Wall.cpp Altar.cpp Skull.cpp load_obj.cpp Exit.cpp

main_file: $(FILES) $(HEADERS)
	g++ -g -o main_file $(FILES)  $(LIBS) -I.
