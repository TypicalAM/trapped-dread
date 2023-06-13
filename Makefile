LIBS=-lGL -lglfw -lGLEW -lassimp
FILES=src/Lodepng.cpp src/Main.cpp src/Shaderprogram.cpp src/Player.cpp src/CollidableObj.cpp src/Floor.cpp src/MapLoader.cpp src/Map.cpp src/Wall.cpp src/Altar.cpp src/Skull.cpp src/Exit.cpp

main_file: $(FILES)
	g++ -g -o game $(FILES)  $(LIBS) -I.
