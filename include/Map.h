#pragma once

#include "../include/Altar.h"
#include "../include/CollidableObj.h"
#include "../include/Exit.h"
#include "../include/Floor.h"
#include "../include/Skull.h"
#include "../include/Wall.h"
#include <algorithm>
#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <vector>

// MapObject is a enum class that represents a type of object on the map
enum MapObject {
  EMPTY,
  WALL,
  START_POS,
  END_POS,
  START_BLUE_ALTAR,
  START_RED_ALTAR,
  END_BLUE_ALTAR,
  END_RED_ALTAR,
};

// Map3D is a type representing a 3d vector of map objects
using Map3D = std::vector<std::vector<std::vector<MapObject>>>;

// GameMap is a class that represents a map
class GameMap {
private:
  Map3D internal_map;                 // 3d vector of map objects
  std::pair<int, int> start_position; // start position
  std::pair<int, int> end_position;   // end position
  int layers; // how many layers the map has (how many floors there are)

public:
  GameMap(Map3D map);
  Map3D get_map();                          // get the 3d vector of map objects
  std::pair<int, int> get_start_position(); // get the start position
  std::pair<int, int> get_end_position();   // get the end position
  std::unique_ptr<Floor> gen_floor();       // generate a floor based on the map
  std::string pretty_print();               // pretty print the map
  std::vector<std::unique_ptr<Wall>>
  gen_walls(); // generate game objects based on the map
  std::vector<std::unique_ptr<Altar>>
  gen_altars(); // generate altars based on the map
  std::vector<std::unique_ptr<Skull>>
  gen_skulls();                      // generate skulls based on the map
  std::unique_ptr<Exit> gen_exit();  // generate an exit based on the map
  glm::vec3 calc_player_start_pos(); // calculate the player's start position
};
