#include "Wall.h"
#include "myCube.h"

// TODO: CALCULATE COLLISION
std::array<std::pair<bool, bool>, 3>
Wall::calc_colision(const glm::vec3 &other_pos) {
  return std::array<std::pair<bool, bool>, 3>{
      std::pair<bool, bool>{false, false},
      std::pair<bool, bool>{other_pos.y <= m_position.y, false},
      std::pair<bool, bool>{false, false}};
}

void Wall::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2{baseM};

  M2 = glm::scale(M2, glm::vec3(0.5f, 0.5f, 0.5f));
  glm::mat4 M3 =
      glm::translate(M2, glm::vec3(2 * x_pos, 1.02f + 2 * layer, y_pos * 2));

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

Wall::Wall(int x_pos, int y_pos, int layer)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(2, 2, 2)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->layer = layer;
}
