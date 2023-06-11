#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>

enum SkullColor { BLUE_SKULL, RED_SKULL };

class Skull : public CollidableObj {
private:
  int x_pos;
  int y_pos;
  SkullColor color;

  float *verticesArray;
  float *normalsArray;
  float *texCoordsArray;
  float *colorsArray;
  size_t numVertices;

public:
  Skull(int x_pos, int y_pos, SkullColor color);
  ~Skull() {}
  void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3 &other_pos) override;
  bool can_grab(const glm::vec3 &player_pos) override;
  SkullColor get_color() { return color; }
};
