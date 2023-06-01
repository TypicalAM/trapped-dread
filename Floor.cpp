#include "Floor.h"
#include "myCube.h"

std::array<std::pair<bool, bool>, 3>
Floor::calc_colision(const glm::vec3 &other_pos) {
  return std::array<std::pair<bool, bool>, 3>{
      std::pair<bool, bool>{false, false},
      std::pair<bool, bool>{other_pos.y <= m_position.y, false},
      std::pair<bool, bool>{false, false}};
}

void Floor::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2{baseM};

  M2 = glm::scale(M2, glm::vec3(0.5f, 0.01f, 0.5f));

  constexpr int range = 20;
  for (int i = -range; i < range; i++) {
    for (int j = -range; j < range; j++) {
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
