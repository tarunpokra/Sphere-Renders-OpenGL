// Tarun Pokra
// 3381418
// August 18, 2025
//this is a header file the scene.cpp file i made for global variables, declarations, etc.

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>

#include <GL/glew.h> //needed to add this.
#include <GL/freeglut.h>

using namespace std;

// floor params.
const float FLOOR_Y = 0.0f;   // just slightly below 0 so objects "sit" on it originally: -0.01


const float TILE_SIZE = 1.0f;
const int   FLOOR_HALF_TILES = 12; // floor spans [-12..11] in both X and Z

// camera controls - extern
extern float cameraAzimuth;
extern float cameraElevation;
extern float cameraDistance;

//Floor reflectivenesss - alpha of the checker tiles, etc.
extern float floorReflectAlpha; // 0..1

// Light position x, y, z, w, 
extern GLfloat lightPos[4];

//Cubemap texture id
extern GLuint cubeMapTex;

//utilities
void setMaterial(const GLfloat* ambient,
                 const GLfloat* diffuse,
                 const GLfloat* specular,
                 GLfloat shininess,
                 GLfloat alpha = 1.0f ) ;

void setCamera();

// floor rendering / stencil + color
void drawFloorStencilOnly();
void drawFloorColor();

//objects + light gizmo
void drawObjects();
void drawLightGizmo();

//Cubemap setup here, 
void initCubeMap();
void buildProceduralCubeMap();


#endif // SCENE_H
//done.