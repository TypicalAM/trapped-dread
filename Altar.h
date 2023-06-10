#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>

enum AltarType { START, END };

class Altar : public CollidableObj {
private:
  int x_pos;
  int y_pos;
  AltarType type;

public:
  Altar(int x_pos, int y_pos, AltarType type);
  virtual ~Altar() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3 &other_pos) override;
};
