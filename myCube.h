#pragma once

#ifndef MYCUBE_H_INCLUDED
#define MYCUBE_H_INCLUDED

// myCubeVertices - homogeniczne współrzędne wierzchołków w przestrzeni modelu
// myCubeNormals - homogeniczne wektory normalne ścian (per wierzchołek) w
// przestrzeni modelu myCubeVertexNormals - homogeniczne wektory normalne
// wierzchołków w przestrzeni modelu myCubeTexCoords - współrzędne teksturowania
// myCubeColors - kolory wierzchołków
// myCubeC1 - kolumna macierzy TBN^-1
// myCubeC2 - kolumna macierzy TBN^-1
// myCubeC3 - kolumna macierzy TBN^-1

extern int myCubeVertexCount;
extern float myCubeVertices[];
extern float myCubeColors[];
extern float myCubeNormals[];
extern float myCubeVertexNormals[];
extern float myCubeTexCoords[];
extern float myCubeC1[];
extern float myCubeC2[];
extern float myCubeC3[];

#endif // MYCUBE_H_INCLUDED
