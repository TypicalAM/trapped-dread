#include "MapLoader.h"
#include "Map.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Check the file is accessible and readable
bool MapLoader::file_accessible() {
  std::ifstream file(this->filename);
  return file.good();
}

// Check if the file really is a map
// We are checking the following things:
// first line is x,y,z
// second line is empty
// third line is first row of the map
// Start and end altars are only allowed to be on the first layer
bool MapLoader::file_correct() {
  std::ifstream file(this->filename);
  std::string line;
  std::getline(file, line);
  std::stringstream ss(line);
  std::vector<int> dimensions;
  while (ss.good()) {
    std::string substr;
    std::getline(ss, substr, ',');
    dimensions.push_back(std::stoi(substr));
  }

  if (dimensions.size() != 3) {
    std::cout << "Nieprawidlowa liczba wymiarow" << std::endl;
    return false;
  }

  bool first_floor_has_start = false;
  bool first_floor_has_end = false;

  for (int i = 0; i < dimensions[2]; i++) {
    // Get the fisrt empty line
    std::getline(file, line);
    if (line != "") {
      std::cout << "Pierwsza linia nie jest pusta" << std::endl;
      return false;
    }

    // Get the next y lines and ensure they are x length
    for (int j = 0; j < dimensions[1]; j++) {
      std::getline(file, line);
      if (line.length() != dimensions[0]) {
        std::cout << "Linia " << j << " nie ma wymaganej dlugosci" << std::endl;
        return false;
      }

      // Check if we are on the first line
      if (i == 0) {
        for (int k = 0; k < dimensions[0]; k++) {
          if (line[k] == 'S') {
            if (first_floor_has_start) {
              std::cout << "Pierwszy start" << std::endl;
              return false;
            }
            first_floor_has_start = true;
          } else if (line[k] == 'E') {
            if (first_floor_has_end) {
              std::cout << "Pierwszy end" << std::endl;
              return false;
            }
            first_floor_has_end = true;
          }
        }
      } else {
        for (int k = 0; k < dimensions[0]; k++) {
          if (line[k] == 'S' || line[k] == 'E') {
            std::cout << "Here" << std::endl;
            return false;
          }
        }
      }
    }
  }

  file.close();

  if (!first_floor_has_start || !first_floor_has_end) {
    std::cout << "Nie ma startu lub endu" << std::endl;
    return false;
  }

  return true;
}

// Parse the file and load the map
GameMap MapLoader::parse_file() {
  std::ifstream file(this->filename);
  std::string line;

  std::cout << "Sprawdzam plik mapy " << this->filename << std::endl;

  std::getline(file, line);
  std::stringstream ss(line);
  std::vector<int> dimensions;

  while (ss.good()) {
    std::string substr;
    std::getline(ss, substr, ',');
    dimensions.push_back(std::stoi(substr));
  }

  Map3D map;
  for (int i = 0; i < dimensions[2]; i++) {
    std::getline(file, line);
    std::vector<std::vector<MapObject>> layer;

    for (int j = 0; j < dimensions[1]; j++) {
      std::getline(file, line);
      std::vector<MapObject> row;

      for (int k = 0; k < dimensions[0]; k++) {
        switch (line[k]) {
        case 'S':
          row.push_back(MapObject::START_POS);
          break;
        case 'E':
          row.push_back(MapObject::END_POS);
          break;
        case 'B':
          row.push_back(MapObject::START_BLUE_ALTAR);
          break;
        case 'b':
          row.push_back(MapObject::END_BLUE_ALTAR);
          break;
        case 'R':
          row.push_back(MapObject::START_RED_ALTAR);
          break;
        case 'r':
          row.push_back(MapObject::END_RED_ALTAR);
          break;
        case 'W':
          row.push_back(MapObject::WALL);
          break;
        case '0':
          row.push_back(MapObject::EMPTY);
          break;
        }
      }

      layer.push_back(row);
    }

    map.push_back(layer);
  }

  file.close();
  return GameMap(map);
}

std::optional<GameMap> MapLoader::load_map() {
  std::cout << "Laduje mape" << std::endl;
  std::cout << file_accessible() << std::endl;
  std::cout << file_correct() << std::endl;
  if (file_accessible() && file_correct())
    return parse_file();

  return std::nullopt;
}

MapLoader::MapLoader(std::string filename) { this->filename = filename; }
