#pragma once
#include "common.h"
#include "shaderprogram.h"
#include <array>

// The results of a collision check, each record is a min/max pair in a given
// axis
// first pair is x collision values
// second pair is y collision values: (y closer to the top of the object, object colliding)
// third pair is z collision values
//
// About that second pair: let's say you are on top of an object
// then the first value will be true (we are closer to the top of the object)
// and the second value will be true (we are `in` the object)
using Collisions = std::array<std::pair<bool, bool>, 3>;

// pure virtual class for objects that support collisions
class CollidableObj {
protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation_per_axis;   // how to rotate the obj when drawing
  glm::vec3 m_bounding_box_radius; // width , height, depth - everything is a
                                   // `prostopad³oœcian`

public:
  CollidableObj(glm::vec3 init_pos, glm::vec3 init_rot,
                glm::vec3 bounding_box_radius)
      : m_position(init_pos), m_rotation_per_axis(init_rot),
        m_bounding_box_radius(bounding_box_radius) {}
  virtual ~CollidableObj() {}

  // most likely translae M by m_pos, rot by m_rot and then draw like normal -
  // possibly relode old M into shader
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) = 0;
  virtual glm::mat4 calc_base_M(const glm::mat4 &baseM);
  virtual Collisions calc_colision(const glm::vec3 &other_pos);
};
