Tarun Pokra
3381418
COMP 390
Option 2 for Course Project, 

This program draws and renders a scene on a black and white checker board, on top of board,
there are 3 shinny spheres made, each with a different colour. Red, Green, Blue. There is a gizmo
light source made as a sphere in the blue sky up as the light source. The spheres reflect into the 
checker board, and the checker board reflects on the 3 spheres as well.

Controls for Camera Movement:
W = up, birds eye view.
S = down
A = rotate left
D = rotate right
Z = zoom in
X = zoom out
----------------------------
esc = terminate program.

Main file has the routines labelled up top. Has void functions like for the keyboard control, reshape, int main, etc.
the scene.cpp file also has routines labelled up top, it has functions like drawing the floor, stencils, cubemap,
drawing the objects, setting the materials, and camera, etc. Also has enable/disable functions for reflection mapping, drawing the light source etc. And thirdly scene.h has the call functions/declarations, etc.

There is already a .exe file for you, just double-click. 
NOTE: this was made on VS CODE, using C++, and outdated legacy tools freeglut, OpenGL, and MSYS2 MINGW64 terminal to compile and run on Windows 11. Also, inside output folder, there are screenshots for the output scene. 

If you want a fresh file: 

Compile command:
g++ COMP390-PrjOp2_pokra_tarun3381418.cpp scene.cpp -o COMP390-PrjOp2_pokra_tarun3381418.exe -lfreeglut -lopengl32 -lglu32 -lglew32

Run command:
./COMP390-PrjOp2_pokra_tarun3381418.exe

Checklist for Project Option 2:
- 3 reflective 3d objects, which are the 3 spheres each different colours.
- a light source, kind of like a sun in the sky.
- a computed ground
- black and white check board that reflects also. 

//done.
Thank you.
