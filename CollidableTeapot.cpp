//#include "CollidableTeapot.h"
//#include <iostream>
//
//void CollidableTeapot::draw(const glm::mat4& baseM, ShaderProgram* sp) {
//
//    glm::mat4 M = glm::translate(baseM, m_position);
//
//    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
//
//
//    glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
//    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myTeapotVertices); //Wska� tablic� z danymi dla atrybutu vertex
//
//    glEnableVertexAttribArray(sp->a("color"));  //W��cz przesy�anie danych do atrybutu color
//    glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, myTeapotColors); //Wska� tablic� z danymi dla atrybutu color
//
//    glEnableVertexAttribArray(sp->a("normal"));  //W��cz przesy�anie danych do atrybutu normal
//    glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, myTeapotVertexNormals); //Wska� tablic� z danymi dla atrybutu normal
//
//    glEnableVertexAttribArray(sp->a("texCoord0"));  //W��cz przesy�anie danych do atrybutu texCoord0
//    glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, myTeapotTexCoords); //Wska� tablic� z danymi dla atrybutu texCoord0
//
//    glActiveTexture(GL_TEXTURE0);
//    glTexParameteri(GL_TEXTURE_2D,
//        GL_TEXTURE_WRAP_S,
//        GL_MIRRORED_REPEAT);
//
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//
//    glUniform1i(sp->u("textureMap0"), 0);
//
//    glDrawArrays(GL_TRIANGLES, 0, myTeapotVertexCount); //Narysuj obiekt
//
//    glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
//    glDisableVertexAttribArray(sp->a("color"));  //Wy��cz przesy�anie danych do atrybutu color
//    glDisableVertexAttribArray(sp->a("normal"));  //Wy��cz przesy�anie danych do atrybutu normal
//    glDisableVertexAttribArray(sp->a("texCoord0"));  //Wy��cz przesy�anie danych do atrybutu texCoord0
//
//}
//
