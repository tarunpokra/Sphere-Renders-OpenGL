// Tarun Pokra
// 3381418
// August 18th, 2025
//This file contains the set materials, setting the camera, drawing the objects, etc.
//It also contains the global variables as well for the camera and the cubemap, and floor,etc.
//ROUTINES:
//- setcamera calc and set the camera position using glulookat.
// - degred, convert deg to rad, for camera rotation.
//- setmaterial - sets the open material prop, ambient, diffuse, specular, etc.
// -drawFloorSencilOnly is for making a flat grid floor for buffer, but no light, texture, etc.
// drawfloorColor - draw semi transparent checkerboard floor with alpha blending.

// enable/disable reflection mapping - enable reflection mapping using the cubemap texture, automatic
// ------- disable the reflection mapping, etc.
//draw objects - drawing 3 refelctive speheres with different colours, and positions,
//           -  enable/disables the cube map relfection stuff.
//light gizmo - light source - draws a small yellow sphere, at light src pos.
//buildproceduralcubemap - generate all6 faces of cubemap, sky gradint/checkerboard ground.
//loadCubemapfromfolder - loading 6 image files jpg using "stb_image"
//initcubMap - create the cubemap texture - trying to load from image, or falls back to procedural gen.
//THIS IS A SCENE THAT BUILDS A REFLECTIVE SCENE WITH DYNAMIC CAMERA POSITION, PROCEDURAL/IMAGE ENVRIONMENT MAPPING,
//  ---- REFLECTIVE SPHERES USING THE CUBE MAP FUNCTION
// --- A VISIBLE LIGHT SOURCE USING GIZMO. 


#include "scene.h"
#include "stb_image.h" //downloaded this library from the internet, useful for cubemap stuff.

#include <vector>
#include <functional>

// Camera state here, global variables.
float cameraAzimuth   =40.0f;
float cameraElevation =10.0f ;   // was 25.0f, lowered so you see the floor immediately
float cameraDistance  = 25.0f ;   // was 20.0f, zoomed out a bit so everything fits, and easy for the viewer.

// Floor transparency here, basically how much of mirrored scene shows through the floor
float floorReflectAlpha = 0.7f;

// Light position (positional light: w=1)
GLfloat lightPos[ 4 ] ={ 5.0f, 10.0f, 5.0f, 1.0f} ;

// Cubemap texture id
GLuint cubeMapTex =0 ;

// image loader, not needed but useful. 
// Attempt to use stb_image.h or otherwise use and fall back to procedural cubemap

#if defined(__has_include)
  #if __has_include("stb_image.h") // file in the directory of the .cpp, src, exe, etc.
    #define HAS_STB_IMAGE 1
    #define STB_IMAGE_IMPLEMENTATION

    #include "stb_image.h"
  #endif
#endif


// Materials & camera stuff here 
void setMaterial (const GLfloat*ambient , const GLfloat * diffuse ,
                 const GLfloat* specular,GLfloat shininess, GLfloat alpha )
{
    GLfloat a[4] ={ ambient[0],  ambient[1],  ambient[2],  alpha }; // a
    GLfloat d[4] ={ diffuse[0],  diffuse[1],  diffuse[2],  alpha }; // d
    GLfloat s[4] = { specular[0], specular[1], specular[2], alpha }; // s

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  a ) ;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  d) ;
    glMaterialfv (GL_FRONT_AND_BACK ,GL_SPECULAR, s) ;
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shininess ) ;
}

inline float degreeRad(float d) {
    return d * 3.1415926535f /180.0f ; //used pi
}

void setCamera() //setting up the camera
{
    float ex =cameraDistance * cos(degreeRad(cameraElevation)) * sin(degreeRad(cameraAzimuth) ) ;
    float ey =cameraDistance * sin(degreeRad(cameraElevation)) ;
    float ez = cameraDistance * cos(degreeRad(cameraElevation)) * cos ( degreeRad(cameraAzimuth) ) ;
    gluLookAt (ex, ey, ez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 ) ;
}

// Floor stencil-only
void drawFloorStencilOnly() // 
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glBegin(GL_QUADS);
    for (int z = -FLOOR_HALF_TILES; z < FLOOR_HALF_TILES; ++z)
    {
        for (int x = -FLOOR_HALF_TILES; x < FLOOR_HALF_TILES; ++x )
        {
            float x0 = x * TILE_SIZE ,x1 = (x + 1) * TILE_SIZE;
            float z0 = z * TILE_SIZE, z1 = (z + 1) * TILE_SIZE ;

            glVertex3f( x0, FLOOR_Y, z0) ;
            glVertex3f(x0, FLOOR_Y, z1 ) ;
            glVertex3f (x1, FLOOR_Y, z1);
            glVertex3f(x1, FLOOR_Y, z0);
        }
    }
    glEnd();

    glEnable(GL_LIGHTING);
}



void drawFloorColor() //this is for colour passing the floor.
{
    //this is a semi-transparent checkerboard so we can see the mirrored scene underneath
    glDisable(GL_LIGHTING ) ; // keepchecker colors flat true black/white, etc. .
    glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA) ;

    glNormal3f(0.0f, 1.0f, 0.0f);  // flip the floor normal -1, etc.
//this is
    glBegin(GL_QUADS); //using quadilaterials usual stuff. 
    for (int z = -FLOOR_HALF_TILES; z<FLOOR_HALF_TILES; ++z)
    {
        for (int x= -FLOOR_HALF_TILES; x < FLOOR_HALF_TILES; ++x )
        {
            bool isWhite =((x + z) & 1) ==0;
            if (isWhite) glColor4f(1.0f, 1.0f, 1.0f, floorReflectAlpha ) ;
            else glColor4f(0.05f , 0.05f,0.05f, floorReflectAlpha) ; // nearly black, not fully black

            float x0= x * TILE_SIZE, x1 =( x + 1) * TILE_SIZE;
            float z0 = z * TILE_SIZE,z1 = (z + 1) * TILE_SIZE ;

            glVertex3f(x0, FLOOR_Y, z0) ;
            glVertex3f(x0, FLOOR_Y, z1 ) ;
            glVertex3f(x1, FLOOR_Y, z1);
            glVertex3f(x1, FLOOR_Y, z0);

        }
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

//
// Objects cubemap reflective here. 
// 
static void enableReflectionMapping() //reflective mapping of cubemap text
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex ? cubeMapTex : 0 ) ;

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP ) ; // basically sets the texture coordinates to reflect mapping for the axis for each
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP ) ;
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

    glEnable(GL_TEXTURE_GEN_S ) ; // this here is for automatic texture coordinate gen, for all 3 of the axes.
    glEnable(GL_TEXTURE_GEN_T );
    glEnable(GL_TEXTURE_GEN_R);
}

static void disableReflectionMapping()
{
    glDisable(GL_TEXTURE_GEN_S ) ; // this to disble the automatic texture coordinates gen, etc.
    glDisable( GL_TEXTURE_GEN_T ) ;
    glDisable (GL_TEXTURE_GEN_R );
    glDisable(GL_TEXTURE_CUBE_MAP) ;
}

void drawObjects() // giant important function, spheres, etc.
{
    enableReflectionMapping();

    // Sphere 1 (Red, left side.
    {
        GLfloat a[3] = { 0.2f, 0.0f, 0.0f };   // ambient red
        GLfloat d[3] = { 0.8f, 0.1f, 0.1f };   // diffuse red
        GLfloat s[3] = { 1.0f, 1.0f, 1.0f };   // shiny white specular
        setMaterial(a, d, s, 96.0f, 1.0f);
//set the red material together with the white specular highlight stuff.
        glPushMatrix () ;
            glTranslatef ( -3.5f, 1.0f, -2.5f ) ; // position of the sphere on the left
            glutSolidSphere ( 1.0,40,30) ; // drawing the sphere thats in the scene.
        glPopMatrix();
    }

    // Sphere 2 green, right side. 
    {
        GLfloat a[3] = { 0.0f, 0.2f, 0.0f}; //setting the green material with white specular highlights here.
        GLfloat d[ 3 ] = { 0.1f, 0.8f, 0.1f  } ;
        GLfloat s [3]  ={1.0f,1.0f, 1.0f } ;
        setMaterial(a, d, s, 96.0f,1.0f);

        glPushMatrix();
            glTranslatef( 2.0f, 1.0f ,-1.5f ) ; // this is the position sphere on the right side.
            glutSolidSphere(1.0, 40, 30 ) ;
        glPopMatrix();
    }

    // Sphere 3 Blue, front one. 
    {
        GLfloat a[3] = {0.0f ,0.0f,0.2f }; //setting the blue material with the white specular highlights.
        GLfloat d[3] ={0.1f, 0.1f ,0.8f };
        GLfloat s [ 3] ={1.0f , 1.0f , 1.0f };
        setMaterial(a, d, s, 96.0f, 1.0f) ;

        glPushMatrix();
            glTranslatef(0.5f, 1.0f, 3.0f); // position of the sphere in the front.
            glutSolidSphere(1.0, 40, 30);
        glPopMatrix();
    }

    disableReflectionMapping(); //this is for disabling the reflection mapping stuff after the drawing.
}

// Light gizmo stuff. 
void drawLightGizmo()
{
    glDisable(GL_LIGHTING ) ; //disable the lighting to basucally draw the emissive object.
    glPushMatrix() ;
        glTranslatef ( lightPos[0 ], lightPos[1],lightPos[2] ) ; // positioning the gizo to the source of the light.

        glColor3f(1.0f, 1.0f, 0.2f ) ; //colour of bright yellow.
        glutSolidSphere(0.15, 20,10 ) ; //small sphere is made showing the light. use keyboard key to move around and see.
    glPopMatrix();
    glEnable(GL_LIGHTING ) ; //and then turn back on the lighting.
}
// Cubemap creation here. really important stuff.
static void makeFace ( unsigned faceTarget ,int w, int h,const std::function <void ( int,int , unsigned char*)> & painter )
{ // nbasically made a helper  to create a face of the cubemap using a pixel painter function
    std::vector<unsigned char> data(w * h * 3);
    for (int y = 0; y < h; ++y) //rbg buffer for image.
    { // generate colour for every pixel using the painter function here.
        for (int x =0; x <w;++x )
        {
            painter(x, y,&data[ ( y * w +x) *3]) ;
        }
    }
    glTexImage2D(faceTarget, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
}//upload image data to cubemap face.

// Simple procedural cubemap (sky gradient + checkerboard ground)
void buildProceduralCubeMap () // procedural cubemap, the sky gradiant, checker = ground, etc.
{
    const int W = 256, H =256 ;
     if (cubeMapTex == 0) {
        glGenTextures(1, &cubeMapTex); //create the texture once
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex); //bind for the setyp

    auto skyPainter =[& ] ( int x,int y,unsigned char* rgb ) 
    { // a painter for function for sky top-bottom, etc.
        float t = (float)y / (H - 1); // vertical factor
        // light blue  to deep blue
        // interpolating between the light blue which is the top, and the deep blue, which is the bottom.
        float r = (1.0f - t)  * 0.6f + t * 0.1f;
        float g = (1.0f - t ) * 0.8f +t * 0.2f;
        float b =(1.0f -t) * 0.95f + t * 0.4f ;
        rgb[0 ] =(unsigned char)(255.0f * r);
        rgb[1 ] = (unsigned char)(255.0f * g) ;
        rgb [2] =( unsigned char)( 255.0f * b) ;
    };

    auto checkerPainter = [&](int x, int y, unsigned char* rgb )
    {//another painter function for the checkerboard pattern here,
        int checkerSize = 32 ;  // pixels per square
        int cx =x / checkerSize ; // checker x
        int cy =y/ checkerSize ; // checker y
        bool white =((cx + cy) & 1)== 0;

        if (white) {
            rgb[0] = rgb[1] = rgb[2] = 230;  // light square, gray-ish white 
        } else {
            rgb[0] = rgb[1] = rgb[2] = 25;   // dark square, almost black basically.
        }
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex); // assigning each face of cubemap.

    makeFace ( GL_TEXTURE_CUBE_MAP_POSITIVE_X,W, H, skyPainter ) ;
    makeFace (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, W, H, skyPainter ) ;
    makeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, W,H, skyPainter )  ;  //the up aka sky,.
    makeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, W,H,checkerPainter ) ; //checkerboard ground, the bottom.
    makeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, W, H,skyPainter ) ;
    makeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, W, H, skyPainter ) ;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // basically setting the texture
    //parameters for the filter, and also the behaviour for the edge, etc.
    glTexParameteri( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) ;
   // andd
    glTexParameteri( GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE ) ;
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R , GL_CLAMP_TO_EDGE ) ;
}


#if defined(HAS_STB_IMAGE )
static bool loadFaceFromFile ( GLenum face ,const char* path ) // loads up a cubemap, face texture from the file, and then upload to my AMD gpu.
{
    int w, h,n; 
    // loading the image using the stb_image file, rgb channels. etc.
    stbi_uc* data =stbi_load(path, &w, &h,&n,3 ) ; // force 3 channels (RGB)
    if (!data)
    {
        std::cerr << "can't load cubemap face..hmmm. " << path << std::endl ; //display to user. 
        return false;
    }

    glTexImage2D(face,0  ,GL_RGB, w,h,0, GL_RGB, GL_UNSIGNED_BYTE, data ) ;
    stbi_image_free(data); // upload the image data stuff to a cubemap face here.
    return true;// then free the memory of the image afterwards. 
}

static bool loadCubeMapFromFolder(const std::string& folder)// loading up 6 faces of the cubemap from a specific folder,
// it is in the same directory as all the other files "cubemap" as the name.
{
    // Expected file names inside ./cubemap/
    // You can rename these to match your actual files.
    const std::string files[6] ={ // basically these are file names in the cubemap folder, they are all jpg.
        folder + "/right.jpg", 
        folder + "/left.jpg",

        folder + "/top.jpg",
        folder + "/bottom.jpg",
        folder + "/front.jpg",
        folder + "/back.jpg"
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex); // bind the cubemap texturee, etc.

    bool ok = true; // try to attempt the loading of all of the 6 faces, 
    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_X, files[0].c_str());
    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, files[1].c_str());

    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, files[2].c_str());
    
    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, files[3].c_str());
    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, files[4].c_str());
    ok &= loadFaceFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, files[5].c_str());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//setting up the cubemap texture, params. etc.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // thism
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)  ;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)  ;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE  );

    return ok; // success status, etc.
}
#endif
 // initlaize the cubemap texture, it either loads from the file, or creates it through a precedure, etc.
void initCubeMap()
{
    glGenTextures(1, &cubeMapTex); // this is simply for texture ID stuff.
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex); // bind config. stuff.

#if defined(HAS_STB_IMAGE)
    // try  load from ./cubemap/ userprovided JPGs
    bool loaded = loadCubeMapFromFolder("./cubemap"); // exact folder name,
    if (loaded)
    {
        std::cout << "Cubemap loaded from ./cubemap/*.jpg" << std::endl; //display
        return;
    }
    std::cerr << "Cubemap images not found or failed to load. Using procedural cubemap.\n"; // display
#endif

    // fallback build a procedural cubemap so reflections still work by making a simple sky/ground cubemap, kind
    //of like a failsafe just in case. so there is still something show.
    buildProceduralCubeMap();
}// end,
//done. 