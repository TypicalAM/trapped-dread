#pragma once
#include "common.h"
#include "shaderprogram.h"
#include <array>
#include <iostream>

// pure virtual class for objects that support collisions
class CollidableObj {
protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation_per_axis;   // how to rotate the obj when drawing
  glm::vec3 m_bounding_box_radius; // width , height, depth - everything is a
                                   // `prostopad³oœcian`

  GLuint m_texture;

  // priatve helper functions for aabb collision algo
  std::pair<float, int> slab_intersetion(const glm::vec3 rayStart,
                                         const glm::vec3 &rayDir) const;
  virtual glm::vec3 aabbMin() const {
    return m_position - m_bounding_box_radius;
  }
  virtual glm::vec3 aabbMax() const {
    return m_position + m_bounding_box_radius;
  }

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

  // returns true if this object has colided with point other_pos
  virtual bool hasColided(const glm::vec3 &other_pos);

  // returns new camera position after collision
  virtual glm::vec3 modify_cam_pos(const glm::vec3 &old_cam_pos,
                                   const glm::vec3 &new_cam_pos);

  // retuns true if this object can be grabbed
  virtual bool can_grab(const glm::vec3 &player_pos) { return false; }

  // returns true if this object can be placed
  virtual bool can_place_skull(const glm::vec3 &player_pos) { return false; }

  void debug_pos_print() {
    std::cout << "pos: " << m_position.x << " " << m_position.y << " "
              << m_position.z << std::endl;
  }

  virtual void bindTexture(GLuint texture) { m_texture = texture; }


  glm::vec3 get_pos() { return m_position; }
};
