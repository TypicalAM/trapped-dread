#include "Altar.h"
#include "Map.h"
#include "MapLoader.h"
#include <algorithm>
#include <glm/fwd.hpp>
#include <iterator>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"

#include "Floor.h"
#include "LightSource.h"
#include "Player.h"
#include "Skull.h"
#include "common.h"

float speed_forward = 0;
float speed_right = 0;
float speed_up = 0;

// encode movement using bit mask
// 0b0000 0000
// 0b0000 0001 - forward
// 0b0000 0010 - backward
// 0b0000 0100 - left
// 0b0000 1000 - right
// 0b0001 0000 - up
// 0b0010 0000 - down
// 0b0100 0000 - fly

int movement_mask = 0;

// added
float walk_speed = 2.f;

float aspectRatio = 1;

float INITIAL_HEIGHT = 1000;
float INITAL_WIDTH = 1000;

ShaderProgram *sp;
Player *camera_ptr; // singleton troche - mozna to lepiej napisac
std::vector<std::unique_ptr<CollidableObj>> GameObjects;
std::vector<std::unique_ptr<CollidableObj>> heldGameObjects;

std::vector<std::unique_ptr<PointLightSource>>
    StatonaryPointLightSources; // swiatło za graczem sie nie liczy
std::vector<std::unique_ptr<ConeLightSource>> StatonaryConeLightSources;

// for now, later we will delete the nonimportant ones
GLuint wallTexture;
GLuint floorTexture;
GLuint ceilingTexture;
GLuint skullTexture;
GLuint altarTexture;

// Check if we can grab a skull and grab it
bool can_grab_skull() {
  std::cout << "we can grab an object" << std::endl;
  glm::vec3 player_pos = camera_ptr->get_position();
  for (int i = 0; i < GameObjects.size(); i++) {
    if (!GameObjects[i]->can_grab(player_pos)) {
      continue;
    }

    // We get the color from old skull, create a new one for the hand
    // and delete the old one, the pointer to which will be freed
    // because of going out of scope
    SkullColor color = dynamic_cast<Skull *>(GameObjects[i].get())->get_color();
    heldGameObjects.push_back(std::make_unique<Skull>(0, 0, color));
    GameObjects.erase(GameObjects.begin() + i);
    return true;
  }

  return false;
}

// Check if we can place a skull on a matching end altar
bool can_place_skull() {
  std::cout << "we have an object in our hand" << std::endl;
  glm::vec3 player_pos = camera_ptr->get_position();
  for (int i = 0; i < GameObjects.size(); i++) {
    if (!GameObjects[i]->can_place_skull(player_pos)) {
      continue;
    }

    std::cout << "we can place an object" << std::endl;
    auto altar = dynamic_cast<Altar *>(GameObjects[i].get());
    if (altar->get_type() != END)
      continue;

    auto altar_pos = altar->get_pos();
    auto skull = dynamic_cast<Skull *>(heldGameObjects[0].get());

    if ((altar->get_color() == RED_ALTAR && skull->get_color() == RED_SKULL) ||
        (altar->get_color() == BLUE_ALTAR &&
         skull->get_color() == BLUE_SKULL)) {
      skull->get_color() == BLUE_SKULL ? camera_ptr->place_blue_skull()
                                       : camera_ptr->place_red_skull();
      GameObjects.push_back(std::make_unique<Skull>(
          altar_pos.first, altar_pos.second, skull->get_color()));
      heldGameObjects.erase(heldGameObjects.begin());
      return true;
    }

    if (heldGameObjects.size() != 2)
      return false;

    auto skull2 = dynamic_cast<Skull *>(heldGameObjects[1].get());
    if ((altar->get_color() == RED_ALTAR && skull2->get_color() == RED_SKULL) ||
        (altar->get_color() == BLUE_ALTAR &&
         skull2->get_color() == BLUE_SKULL)) {
      skull2->get_color() == BLUE_SKULL ? camera_ptr->place_blue_skull()
                                        : camera_ptr->place_red_skull();
      GameObjects.push_back(std::make_unique<Skull>(
          altar_pos.first, altar_pos.second, skull2->get_color()));
      heldGameObjects.erase(heldGameObjects.begin() + 1);
      return true;
    }
  }

  return false;
}

// Procedura obsługi błędów
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W)
      movement_mask |= MOVE_FORWARD;
    if (key == GLFW_KEY_S)
      movement_mask |= MOVE_BACKWARD;
    if (key == GLFW_KEY_A)
      movement_mask |= MOVE_LEFT;
    if (key == GLFW_KEY_D)
      movement_mask |= MOVE_RIGHT;
    if (key == GLFW_KEY_SPACE)
      movement_mask |= MOVE_UP;
    if (key == GLFW_KEY_LEFT_CONTROL)
      movement_mask |= MOVE_DOWN;
    if (key == GLFW_KEY_T)
      camera_ptr->toggle_fly();
    if (key == GLFW_KEY_EQUAL)
      camera_ptr->inc_pitch(); // toggle ala noclip
    if (key == GLFW_KEY_KP_SUBTRACT)
      camera_ptr->inc_yaw(); // toggle ala noclip
    if (key == GLFW_KEY_0)
      camera_ptr->reset_cam(); // toggle ala noclip
  }

  if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W)
      movement_mask &= ~MOVE_FORWARD;
    if (key == GLFW_KEY_S)
      movement_mask &= ~MOVE_BACKWARD;
    if (key == GLFW_KEY_A)
      movement_mask &= ~MOVE_LEFT;
    if (key == GLFW_KEY_D)
      movement_mask &= ~MOVE_RIGHT;
    if (key == GLFW_KEY_SPACE)
      movement_mask &= ~MOVE_UP;
    if (key == GLFW_KEY_LEFT_CONTROL)
      movement_mask &= ~MOVE_DOWN;
    if (key == GLFW_KEY_E) {
      if (heldGameObjects.size() >= 1 && can_place_skull())
        return;

      if (heldGameObjects.size() <= 1 && can_grab_skull())
        return;
    }
  }
}

GLuint readTexture(const char *filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);

  // Wczytanie do pamięci komputera
  std::vector<unsigned char> image; // Alokuj wektor do wczytania obrazka
  unsigned width, height; // Zmienne do których wczytamy wymiary obrazka
  // Wczytaj obrazek
  unsigned error = lodepng::decode(image, width, height, filename);

  // Import do pamięci karty graficznej
  glGenTextures(1, &tex);            // Zainicjuj jeden uchwyt
  glBindTexture(GL_TEXTURE_2D, tex); // Uaktywnij uchwyt
  // Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               (unsigned char *)image.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return tex;
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {

  INITAL_WIDTH = width;
  INITIAL_HEIGHT = height;

  camera_ptr->reset_after_resize(width, height);

  if (height == 0)
    return;
  aspectRatio = (float)width / (float)height;
  glViewport(0, 0, width, height);
}

void mouse_move_callback(GLFWwindow *window, double xpos, double ypos) {

  // reset cursor to screen center
  glfwSetCursorPos(window, static_cast<double>(INITAL_WIDTH / 2),
                   static_cast<double>(INITIAL_HEIGHT / 2));

  camera_ptr->mouse_move_callback(xpos, ypos);
}

// Procedura inicjująca
void initOpenGLProgram(GLFWwindow *window) {
  //************Tutaj umieszczaj kod, który należy wykonać raz, na początku
  // programu************
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  glfwSetWindowSizeCallback(window, windowResizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetCursorPosCallback(window, mouse_move_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  floorTexture = readTexture("floor_diff.png");
  wallTexture = readTexture("wall_diffuse.png");

  sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
}

// Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow *window) {
  //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu
  // pętli
  // głównej************
  delete camera_ptr;
  delete sp;
}

// alters global `GameObjects`
void setupInitialPositionsOfObjects(GameMap &map) {
    auto floor = map.gen_floor();
    floor->bindTexture(floorTexture);
    GameObjects.push_back(std::move(floor));

  GameObjects.push_back(std::move(map.gen_exit()));
  // TODO FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!! 
  // analogicze dla exit textura
  
  for (auto& wall : map.gen_walls()) {
      wall->bindTexture(wallTexture);
      GameObjects.push_back(std::move(wall));
  }
  for (auto& altars : map.gen_altars()) {
      if (typeid(*altars) == typeid(Skull))
          altars->bindTexture(skullTexture);
      else
          altars->bindTexture(altarTexture);
      GameObjects.push_back(std::move(altars));
  }
}

// Procedura rysująca zawartość sceny
void drawScene(GLFWwindow *window) {
  //************Tutaj umieszczaj kod rysujący obraz******************l
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera_ptr->calcViewMatrix();

  glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f,
                                 50.0f); // Wylicz macierz rzutowania

  glm::mat4 M = glm::mat4(1.0f);

  sp->use(); // Aktywacja programu cieniującego

  glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
  glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
  glUniform1f(sp->u("divisor"), 3.2);

  // draw all the game objects
  // for now just floor
  for (auto &obj : GameObjects) {
    // print type
    // std::cout << typeid(*obj).name() << std::endl;

    obj->draw(M, sp);
  }

  // draw held objects - those moving with the cam
  int number_of_held = heldGameObjects.size();
  int i = 0;
  for (auto &obj : heldGameObjects) {
    // print type
    // std::cout << "held: " << typeid(*obj).name() << std::endl;

    glm::mat4 M2 = glm::translate(M, camera_ptr->getCamPos());
    float angle = glm::radians(-1 * camera_ptr->getAngles().y);

    M2 = glm::rotate(M2, angle + 1.4f + -0.6f * i, glm::vec3(0.f, 1.f, 0.f));
    M2 = glm::translate(M2, glm::vec3(0.2f, -0.4f, 0.4f));
    M2 = glm::scale(M2, glm::vec3(0.5));
    // rotate the sceond skull to face inwards
    M2 = glm::rotate(M2, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

    obj->draw(M2, sp);

    i++;
  }

  glfwSwapBuffers(window); // Przerzuć tylny bufor na przedni
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Nie podano nazwy pliku z mapą.\n");
    exit(EXIT_FAILURE);
  }

  std::string map_path = "maps/" + std::string(argv[1]) + ".map";
  auto map = (new MapLoader(map_path))->load_map();
  if (!map.has_value()) {
    fprintf(stderr, "Nie można załadować mapy.\n");
    exit(EXIT_FAILURE);
  }

  std::cout << "Mapa załadowana poprawnie." << std::endl;
  std::cout << map->pretty_print() << std::endl;

  // Try to load
  GLFWwindow *window; // Wskaźnik na obiekt reprezentujący okno

  glfwSetErrorCallback(error_callback); // Zarejestruj procedurę obsługi błędów

  if (!glfwInit()) { // Zainicjuj bibliotekę GLFW
    fprintf(stderr, "Nie można zainicjować GLFW.\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(
      INITAL_WIDTH, INITIAL_HEIGHT, "OpenGL", NULL,
      NULL); // Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

  if (!window) // Jeżeli okna nie udało się utworzyć, to zamknij program
  {
    fprintf(stderr, "Nie można utworzyć okna.\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  camera_ptr =
      new Player(INITAL_WIDTH, INITIAL_HEIGHT, map->calc_player_start_pos());
  camera_ptr->moveCam(glm::vec3(0.f, 0.4f, 0.f));
  camera_ptr->setFloorLevel(0.4f);

  glfwMakeContextCurrent(
      window); // Od tego momentu kontekst okna staje się aktywny i polecenia
               // OpenGL będą dotyczyć właśnie jego.
  glfwSwapInterval(
      1); // Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

  if (glewInit() != GLEW_OK) { // Zainicjuj bibliotekę GLEW
    fprintf(stderr, "Nie można zainicjować GLEW.\n");
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window); // Operacje inicjujące

  setupInitialPositionsOfObjects(map.value());

  // Główna pętla
  glfwSetTime(0); // Zeruj timer
  while (!glfwWindowShouldClose(
      window)) // Tak długo jak okno nie powinno zostać zamknięte
  {
    camera_ptr->update(glfwGetTime(), movement_mask, walk_speed, GameObjects);

    glfwSetTime(0);    // Zeruj timer
    drawScene(window); // Wykonaj procedurę rysującą

    glfwPollEvents(); // Wykonaj procedury callback w zalezności od zdarzeń
                      // jakie zaszły.
  }

  freeOpenGLProgram(window);

  glfwDestroyWindow(window); // Usuń kontekst OpenGL i okno
  glfwTerminate();           // Zwolnij zasoby zajęte przez GLFW
  exit(EXIT_SUCCESS);
}
