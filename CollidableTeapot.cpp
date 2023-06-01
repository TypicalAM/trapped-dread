#include "CollidableTeapot.h"

void CollidableTeapot::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(baseM));

  glEnableVertexAttribArray(
      sp->a("vertex")); // W��cz przesy�anie danych do atrybutu vertex
  glVertexAttribPointer(
      sp->a("vertex"), 4, GL_FLOAT, false, 0,
      myTeapotVertices); // Wska� tablic� z danymi dla atrybutu vertex

  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0,
                        myTeapotColors); // za�aduj kolory

  glDrawArrays(GL_TRIANGLES, 0, myTeapotVertexCount); // Narysuj obiekt

  glDisableVertexAttribArray(
      sp->a("vertex")); // Wy��cz przesy�anie danych do atrybutu vertex
  glDisableVertexAttribArray(
      sp->a("color")); // Wy��cz przesy�anie danych do atrybutu vertex
}
