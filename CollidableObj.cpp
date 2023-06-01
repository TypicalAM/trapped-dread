#include "CollidableObj.h"
#include <iostream>

glm::mat4 CollidableObj::calc_base_M(const glm::mat4 &baseM) {
  glm::mat4 newM = glm::translate(baseM, m_position);
  // rotate by all angles
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.x),
                     glm::vec3(1.f, 0.f, 0.f));
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.y),
                     glm::vec3(0.f, 1.f, 0.f));
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.z),
                     glm::vec3(0.f, 0.f, 1.f));

  return newM;
}

std::array<std::pair<bool, bool>, 3>
CollidableObj::calc_colision(const glm::vec3 &other_pos) {
  std::array<std::pair<bool, bool>, 3> collision_array{
      std::pair<bool, bool>{false, false}, std::pair<bool, bool>{false, false},
      std::pair<bool, bool>{false, false}};
  glm::vec3 diff = m_position - other_pos;

  // check we are colliding with the box
  bool colided =
      diff.x >= -m_bounding_box_radius.x && diff.x <= m_bounding_box_radius.x &&
      diff.y >= -m_bounding_box_radius.y && diff.y <= m_bounding_box_radius.y &&
      diff.z >= -m_bounding_box_radius.z && diff.z <= m_bounding_box_radius.z;

  if (!colided) {
    return collision_array;
  }

  if (abs(diff.x) <= m_bounding_box_radius.x) {
    collision_array[0].first = true;
    collision_array[0].second = true;
  }

  if (diff.y >= m_bounding_box_radius.y) {
    collision_array[1].first = true;
  }

  else if (diff.y <= -m_bounding_box_radius.y) {
    collision_array[1].second = true;
  }

  if (abs(diff.z) <= m_bounding_box_radius.z) {
    collision_array[2].first = true;
    collision_array[2].second = true;
  }

  std::cout << "colision detected with teapot" << std::endl;
  std::cout << "diff: " << diff.x << ", " << diff.y << ", " << diff.z
            << std::endl;
  std::cout << "\t\tcollision X:" << collision_array[0].first << ", "
            << collision_array[0].second << std::endl;
  std::cout << "\t\tcollision Y:" << collision_array[1].first << ", "
            << collision_array[1].second << std::endl;
  std::cout << "\t\tcollision Z:" << collision_array[2].first << ", "
            << collision_array[2].second << std::endl;

  return collision_array;
}
