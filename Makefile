LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  lodepng.h  shaderprogram.h Player.h common.h CollidableTeapot.h CollidableObj.h Floor.h
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp Player.cpp CollidableTeapot.cpp CollidableObj.cpp Floor.cpp teapotAndCube.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
