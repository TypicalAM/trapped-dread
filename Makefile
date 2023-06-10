LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  lodepng.h  shaderprogram.h Player.h common.h CollidableTeapot.h CollidableObj.h Floor.h MapLoader.h Map.h Wall.h load_obj.h Altar.h Skull.h load_obj.h
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp Player.cpp CollidableTeapot.cpp CollidableObj.cpp Floor.cpp teapotAndCube.cpp MapLoader.cpp Map.cpp Wall.cpp Altar.cpp Skull.cpp load_obj.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
