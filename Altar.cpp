#include "Altar.h"
#include "CollidableObj.h"
#include "load_obj.h"
#include "myCube.h"
#include <iostream>

#define ALTAR_SCALAR glm::vec3(0.001f, 0.001f, 0.001f)
#define ALTAR_OFFSET_Y 0.4f

void Altar::draw(const glm::mat4& baseM, ShaderProgram* sp) {
    glm::mat4 M2 = glm::translate(baseM, m_position);
    glm::mat4 M3 = glm::scale(M2, ALTAR_SCALAR);
    std::cout << "Drawing object" << std::endl;
    return;

    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
    glEnableVertexAttribArray(sp->a("vertex"));
    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verticesArray);

    glEnableVertexAttribArray(sp->a("color"));
    glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsArray);

    glEnableVertexAttribArray(sp->a("normal"));
    glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normalsArray);

    glEnableVertexAttribArray(sp->a("texCoord0"));

    glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoordsArray);

    glActiveTexture(GL_TEXTURE0);
   

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glUniform1i(sp->u("textureMap0"), 0);

    std::cout << "Drawing object" << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, numVertices); // Narysuj obiekt

    glDisableVertexAttribArray(sp->a("vertex"));

    glDisableVertexAttribArray(sp->a("color"));
    glDisableVertexAttribArray(sp->a("normal"));
    glDisableVertexAttribArray(sp->a("texCoord0"));
}

bool Altar::hasColided(const glm::vec3& other_pos) {
    // TODO: the bounding box has to be smaller
    // than the placing box
    return false;
}

bool Altar::can_place_skull(const glm::vec3& player_pos) {
    glm::vec3 diff = m_position - player_pos;
    return diff.x >= -m_bounding_box_radius.x &&
        diff.x <= m_bounding_box_radius.x &&
        diff.y >= -m_bounding_box_radius.y &&
        diff.y <= m_bounding_box_radius.y &&
        diff.z >= -m_bounding_box_radius.z &&
        diff.z <= m_bounding_box_radius.z;

    std::cout << "can place skull" << std::endl;
    std::cout << hasColided(player_pos) << std::endl;
    return hasColided(player_pos);
}

Altar::Altar(int x_pos, int y_pos, AltarType type, AltarColor color)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
        glm::vec3(1, 1, 1)) {
    this->m_position = glm::vec3(x_pos, ALTAR_OFFSET_Y, y_pos);
    this->type = type;
    this->color = color;
    this->x_pos = x_pos;
    this->y_pos = y_pos;

    // Load the object from file, no error handling because we HATE IT
    loadOBJ("models/table.obj", verticesArray, normalsArray, texCoordsArray,
        numVertices);

    colorsArray = new float[numVertices * 4];
    for (int i = 0; i < numVertices; i++) {
        colorsArray[i * 4] = color == AltarColor::RED_ALTAR ? 1.0f : 0.0f;
        colorsArray[i * 4 + 1] = color == AltarColor::RED_ALTAR ? 0.0f : 0.0f;
        colorsArray[i * 4 + 2] = color == AltarColor::RED_ALTAR ? 0.0f : 1.0f;
        colorsArray[i * 4 + 3] = 1.0f;
    }

    std::cout << "Loaded object, size is " << numVertices << std::endl;
}
