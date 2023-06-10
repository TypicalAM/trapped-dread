#include "Map.h"
#include "Altar.h"
#include "Floor.h"
#include "Wall.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string GameMap::pretty_print() {
  std::stringstream ss;

  // Left to right, bottom to top
  for (int layer = 0; layer < internal_map.size(); layer++) {
    ss << "The layer is " << layer << "\n";
    for (int y = 0; y < internal_map[0].size(); y++) {
      for (int x = 0; x < internal_map[0][0].size(); x++) {
        switch (internal_map[layer][y][x]) {
        case MapObject::START_ALTAR:
          ss << "S";
          break;
        case MapObject::END_ALTAR:
          ss << "E";
          break;
        case MapObject::WALL:
          ss << "W";
          break;
        default:
          ss << "0";
          break;
        }
      }
      ss << "\n";
    }
  }

  return ss.str();
}

Map3D GameMap::get_map() { return internal_map; }

std::pair<int, int> GameMap::get_start_position() { return start_position; }

std::pair<int, int> GameMap::get_end_position() { return end_position; }

std::unique_ptr<Floor> GameMap::gen_floor() {
  int width = internal_map[0][0].size();
  int length = internal_map[0].size();
  return std::make_unique<Floor>(0.4f, width, length);
}

glm::vec3 GameMap::calc_player_start_pos() {
  return glm::vec3(start_position.first, 0.f, start_position.second);
}

std::vector<std::unique_ptr<Wall>> GameMap::gen_walls() {
  std::vector<std::unique_ptr<Wall>> walls;

  for (int layer = 0; layer < internal_map.size(); layer++) {
    for (int y = 0; y < internal_map[0].size(); y++) {
      for (int x = 0; x < internal_map[0][0].size(); x++) {
        if (internal_map[layer][y][x] == MapObject::WALL)
          walls.push_back(std::make_unique<Wall>(x, y, layer));
      }
    }
  }

  std::cout << "Walls size: " << walls.size() << std::endl;
  return walls;
}

std::vector<std::unique_ptr<Altar>> GameMap::gen_altars() {
  std::vector<std::unique_ptr<Altar>> altars;

  for (int y = 0; y < internal_map[0].size(); y++) {
    for (int x = 0; x < internal_map[0][0].size(); x++) {
      if (internal_map[0][y][x] == MapObject::START_ALTAR)
        altars.push_back(std::make_unique<Altar>(x, y, START));

      else if (internal_map[0][y][x] == MapObject::END_ALTAR)
        altars.push_back(std::make_unique<Altar>(x, y, END));
    }
  }

  std::cout << "Altars size: " << altars.size() << std::endl;
  return altars;
}

GameMap::GameMap(Map3D map) {
  internal_map = map;
  for (int layer = 0; layer < map.size(); layer++) {
    for (int y = 0; y < map[0].size(); y++) {
      for (int x = 0; x < map[0][0].size(); x++) {
        if (map[layer][y][x] == MapObject::START_ALTAR) {
          start_position = std::make_pair(x, y);
        } else if (map[layer][y][x] == MapObject::END_ALTAR) {
          end_position = std::make_pair(x, y);
        }
      }
    }
  }

  layers = map.size();
}