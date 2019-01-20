#include <kos.h>
#include <GL\gl.h>
#include <GL\glext.h>
#include <GL\glu.h>
#include <GL\glkos.h>

KOS_INIT_FLAGS(INIT_DEFAULT);
bool run = true;

// Input variables
float xPos = 0.0f;
float yPos = 0.0f;
float zPos = 7.0f;
float sensitivity = 1000.0f;

// rot holds our rotation value for all of the objects being drawn.
// This gets updated every loop.
GLfloat rot;

// rotSpeed controls how fast the objects are rotating.
GLfloat rotSpeed = 1.0f;

// Simple struct to hold RGBA values
struct ColorRGBA {
  ColorRGBA(){}
  ColorRGBA(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) : r(_r), g(_g), b(_b), a(_a){}
  GLfloat r,g,b,a;
};

static GLfloat _pyramid_vertices[] = {
  0.0f,1.0f,0.0f,
  -1.0f,-1.0f,1.0f,
  1.0f,-1.0f,1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f
};

static GLubyte _pyramid_indices[] = {
  0,1,2,
  0,2,3,
  0,3,4,
  0,4,1,
  1,2,3,
  1,4,3
};

static GLfloat _rect_vertices[] = {
  -1.0f,0.0f,-1.0f,
  -1.0f,0.0f,1.0f,
  1.0f,0.0f,-1.0f,
  1.0f,0.0f,1.0f
};

static GLubyte _rect_indices[] = {
  0,1,2,
  2,1,3
};


// Our primary pyramid color array
static ColorRGBA _colors[] = {
  ColorRGBA(1.0f,0.0f,0.0f,1.0f),
  ColorRGBA(0.0f,1.0f,0.0f,1.0f),
  ColorRGBA(0.0f,0.0f,1.0f,1.0f),
  ColorRGBA(1.0f,1.0f,0.0f,1.0f),
  ColorRGBA(0.0f,1.0f,1.0f,1.0f)
};

// Our secondary pyramid color array
static ColorRGBA _colors_2[] = {
  ColorRGBA(0.2435f,0.1234f,0.9853f,1.0f),
  ColorRGBA(0.8201f,0.530f,0.1952f,1.0f),
  ColorRGBA(0.1022f,0.16543f,1.0f,1.0f),
  ColorRGBA(1.0f,0.234f,1.0f,1.0f),
  ColorRGBA(0.89f,1.0f,0.234f,1.0f)
};

// Our third pyramid color array
static ColorRGBA _colors_3[] = {
  ColorRGBA(0.9472f,0.5642f,0.3254f,1.0f),
  ColorRGBA(0.392f,0.9843f,0.473f,1.0f),
  ColorRGBA(0.4932f,0.832f,0.123f,1.0f),
  ColorRGBA(0.4f,1.0f,1.0f,1.0f),
  ColorRGBA(0.24f,0.0f,0.54f,1.0f)
};

// White color array for the floor rectangle
static ColorRGBA _colors_rect[] = {
  ColorRGBA(1.0f,1.0f,1.0f,1.0f),
  ColorRGBA(1.0f,1.0f,1.0f,1.0f),
  ColorRGBA(1.0f,1.0f,1.0f,1.0f),
  ColorRGBA(1.0f,1.0f,1.0f,1.0f)
};

void DrawObject(GLfloat* vertices, GLubyte* indices, int indexArraySize, ColorRGBA* colors);

int main(int argc, char** argv){
  // Initialize the input device(s)
  maple_device_t* controller = maple_enum_type(0,MAPLE_FUNC_CONTROLLER);
  cont_state_t* controllerState;

  // Initialize OpenGL

  vid_set_mode(DM_640x480, PM_RGB565);

  glKosInit();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,640.0f/480.0f, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Begin our main loop
  while(run){

    // Handle Input
    if (controller){
      controllerState = (cont_state_t*) maple_dev_status(controller);

      // Pressing A will end the simulation
      if (controllerState->buttons & CONT_A)
        run = false;

      // DPAD up increases rotation speed
      if (controllerState->buttons & CONT_DPAD_UP)
        rotSpeed += 1;

      // DPAD down decreases rotation speed
      if (controllerState->buttons & CONT_DPAD_DOWN)
        rotSpeed -= 1;

      xPos += (float)(controllerState->joyx / sensitivity);
      yPos += (float)(controllerState->joyy / sensitivity);

      zPos += (float)(controllerState->rtrig / sensitivity);
      zPos -= (float)(controllerState->ltrig / sensitivity);

    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(xPos,yPos,zPos,
              0,0,0,
              0,1,0);

    // Push our LookAt matrix to the stack, so we don't have to recalculate it.
    // Then we'll rotate our big pyramid
    glPushMatrix();

    glRotatef(rot*rotSpeed, 0.0f,1.0f,0.0f);

    DrawObject(_pyramid_vertices,_pyramid_indices,sizeof(_pyramid_indices)/sizeof(GLubyte),_colors);

    glPopMatrix();

    // Smaller back pyramid
    glPushMatrix();

    glTranslatef(2.5f,1.0,-2.0f);
    glRotatef((rot+25)*rotSpeed, 35.0f,45.0f,0.0f);
    glScalef(0.5f,0.5f,0.5f);

    DrawObject(_pyramid_vertices,_pyramid_indices,sizeof(_pyramid_indices)/sizeof(GLubyte),_colors_2);

    glPopMatrix();

    // Small forward pyramid
    glPushMatrix();

    glTranslatef(-2.0f,0.5f,1.0f);
    glScalef(0.35f,0.35f,0.35f);
    glRotatef((rot+10)*rotSpeed, 0.5f,0.35f,0.15f);

    DrawObject(_pyramid_vertices,_pyramid_indices,sizeof(_pyramid_indices)/sizeof(GLubyte),_colors_3);

    glPopMatrix();

    // Floor Rectangle
    glPushMatrix();

    glTranslatef(0.0f,-2.0f,0.0f);
    glScalef(2.0f,0.0f,2.0f);

    DrawObject(_rect_vertices,_rect_indices,sizeof(_rect_indices)/sizeof(GLubyte),_colors_rect);

    glPopMatrix();

    // End drawing

    glKosSwapBuffers();

    // Update rotation
    rot += 0.1f;
  }

  return 0;
}

// This function handles actually drawing the objects to the screen.
void DrawObject(GLfloat* vertices, GLubyte* indices, int indexArraySize, ColorRGBA* colors){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawElements(GL_TRIANGLES,indexArraySize,GL_UNSIGNED_BYTE,indices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}
