#include <cstdlib>
#include <cmath>
#include <iostream>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>


#define PI 3.14159265

int frame = 0; // time from the beginning of animation, in frames
int isAnimating = 0;
int frameDuration = 50; // milliseconds

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  

  glutSwapBuffers();
}

void animate(int val)
{
  if (isAnimating)
    {
      frame++;
      glutPostRedisplay();
      glutTimerFunc(frameDuration, animate, 1);
    }
}

void setup()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
    {
    case ' ':
      isAnimating = !isAnimating;
      animate(0);
      break;
    case 'r':
    case 'R':
      isAnimating = 0;
      frame = 0;
      glutPostRedisplay();
      break;
    case 27:
      exit(0);
      break;
    }
}

void special(int key, int x, int y)
{
  switch (key)
    {
    case GLUT_KEY_DOWN:
      break;
    case GLUT_KEY_UP:
      break;
    }
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, 1, 5, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutCreateWindow("window");  

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  
  glewExperimental = GL_TRUE;
  glewInit();
  
  setup();
  
  glutMainLoop();
}
