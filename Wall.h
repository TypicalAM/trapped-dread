#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>

class Wall : public CollidableObj {
private:
  int x_pos;
  int y_pos;
  int layer;

public:
  Wall(int x_pos, int y_pos, int layer);
  virtual ~Wall() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3& other_pos) override;

};
