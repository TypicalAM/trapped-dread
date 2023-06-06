#include "Floor.h"
#include "CollidableObj.h"
#include "myCube.h"
#include <array>
#include <iostream>
#include <vector>

Collisions Floor::calc_colision(const glm::vec3 &other_pos) {
  std::cout << "other_pos.y: " << other_pos.y << std::endl;
  std::cout << "m_position.y: " << m_position.y << std::endl;

  float inside_pos = other_pos.y - m_position.y;

  std::cout << "inside_pos" << inside_pos << std::endl;

  bool inside = other_pos.y < m_position.y + 0.02f;

  std::cout << "inside: " << inside << std::endl;

  return Collisions{std::pair<bool, bool>{true, true},
                    std::pair<bool, bool>{0.02f >= inside_pos, inside},
                    std::pair<bool, bool>{true, true}};
}

void Floor::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2{baseM};

  M2 = glm::scale(M2, glm::vec3(0.5f, 0.01f, 0.5f));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      glm::mat4 M3 = glm::translate(M2, glm::vec3(2 * i, 0, 2 * j));

      // rotate so it gets diffrent color
      if ((i + j) % 2) {
        M3 = glm::rotate(M3, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
      }

      glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));

      glEnableVertexAttribArray(
          sp->a("vertex")); // W³¹cz przesy³anie danych do atrybutu vertex
      glVertexAttribPointer(
          sp->a("vertex"), 4, GL_FLOAT, false, 0,
          myCubeVertices); // Wska¿ tablicê z danymi dla atrybutu vertex

      glEnableVertexAttribArray(sp->a("color"));
      glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0,
                            myCubeColors); // za³aduj kolory

      glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount); // Narysuj obiekt

      glDisableVertexAttribArray(
          sp->a("vertex")); // Wy³¹cz przesy³anie danych do atrybutu vertex
      glDisableVertexAttribArray(
          sp->a("color")); // Wy³¹cz przesy³anie danych do atrybutu vertex
    }
  }
}

Floor::Floor(float floor_level, int width, int length)
    : CollidableObj(glm::vec3(0, floor_level, 0), glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, 0)) {
  this->width = width;
  this->length = length;
}
