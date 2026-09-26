// Tarun Pokra
// 3381418
// August 18, 2025
// Option 2 course project - COMP390
//This program basically renders a checkerboard, shows 3 shinny 3d objects which spheres each with a different colour
// red, green, and blue. 
//the checkerboard reflects itself into each of the 3 spheres
// and the 3 spheres reflect into the checkerboard
//there is also at least one light source in the scene as well
//note: most functionality are handled in scene.cpp/scene.h files. 
//FOR OPTION 2 HERE IS A CHECKLIST:
//- THIS PROGRAM HAS AT LEAST 3 REFLECTIVE 3D OBJECTS
//- THIS PROGRAM HAS A COMPUTED GROUND (BLACK AND WHITE CHECK BOARD)
//- THIS PROGRAM HAS AT LEAST ONE LIGHT SOURCE.
// THIS PROGRAM SHOWS 3 3D OBJECTS REFLECT ON THE COMPUTED GROUND, 
// AND THIS PROGRAM ALSO SHOWS THE COMPUTED GROUND REFLECT ON THE 3 3D OBJECTS AS WELL.
//GROUND REFLECTS THE 3D OBJECTS  - USING THE STENCIL AND MIRRORING
//3D OBJECTS REFLECT THE GROUND - USING THE CUBEMAP, WITH THE FLOOR FACE.

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "scene.h"

using namespace std;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
        | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //reset.
    setCamera();

    // Position light for specular highlights and gizmo
    glLightfv(GL_LIGHT0, GL_POSITION,
        lightPos ) ;

    // Write floor into stencil onlyno color, no depth updates
    // Write floor into stencil only
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // no color
    glDepthMask(GL_FALSE);                               // no depth
    glStencilFunc(GL_ALWAYS, 1, 0xFF);                   // always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);           // replace with 1
    drawFloorStencilOnly();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);     // re-enable color
    glDepthMask(GL_TRUE);                                // re-enable depth

    //draw mirrored scene (reflections) only where stencil == 1 (on floor)
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glPushMatrix();
        // Mirror across the floor Y=FLOOR_y. We mirror around world Y=0 so then translate if FLOOR_Y != 0
        if (FLOOR_Y != 0.0f) glTranslatef( 0.0f , 2.0f *FLOOR_Y, 0.0f )  ;

        glScalef(1.0f, -1.0f ,1.0f ) ; //scale

        // Mirror the light vertically to match mirrored scene
        GLfloat lightRef[ 4 ] = { lightPos[0], -lightPos[1], lightPos[2], lightPos[3] };
        glLightfv ( GL_LIGHT0, GL_POSITION, lightRef ) ;

        glFrontFace ( GL_CW ) ; // fix winding due to mirroring
        drawObjects () ;      // reflective objects aka cubemap
        glFrontFace ( GL_CCW) ;
    glPopMatrix();

    // Restore original light position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos) ;

    //disable the stencil and draw the actual floor semi-transparent to reveal reflection. 
    glDisable(GL_STENCIL_TEST ) ;
    drawFloorColor() ;

    //Draw the actual objects not mirrored
    drawObjects();

    // Now Draw light gizmo last, unlit
    drawLightGizmo();

    glutSwapBuffers();
}

void reshape(int w, int h) //simplified the code.
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h); //width and height.

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(50.0, (double)w / (double)h, 0.5, 150.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard ( unsigned char key, int, int )
{
    switch (key)
    {
        case 27: exit(0); break; // ESC
        case 'a': cameraAzimuth   -= 3; break;
        case 'd': cameraAzimuth   += 3; break;
        case 'w': cameraElevation += 2; break;
        case 's': cameraElevation -= 2; break;
        case 'z': cameraDistance  -= 0.5f;break;
        case 'x': cameraDistance  += 0.5f;break;

        // Optional for adjusting floor transparency
        case '[':floorReflectAlpha =std::max(0.0f, floorReflectAlpha - 0.05f);break ;
        case ']': floorReflectAlpha =std::min(1.0f, floorReflectAlpha + 0.05f);break;
    }
    glutPostRedisplay();
}

void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.9f, 1.0f); // skyish background  here.

    glClearStencil(0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Texture env. modulate cubemap with material/specular, etc.
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE ) ;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH | GLUT_STENCIL ) ;
    glutInitWindowSize(900, 650 ) ;
    glutCreateWindow("Option 2: reflective scene Planar and Cubemap!" ) ;

    initGL();
    buildProceduralCubeMap(); // build the checkerboard + sky cubemap


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0; //terminate

}// done,
