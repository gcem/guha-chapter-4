//
// Draws a ball sliding on an inclined surface. World units are in meter, g = 9.8.
//
// Notes: surface is drawn using a vbo and display list. I could have created the
// vbo for only one line and then translated it in display list to draw other lines.
//

#include <cstdlib>
#include <cmath>
#include <iostream>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>


#define PI 3.14159265
#define YSLICES 20
#define XSLICES 20


int frame = 0; // time from the beginning of animation, in frames
float seconds = 0; // real time
int isAnimating = 0;
int frameDuration = 50; // milliseconds
float vertices[YSLICES * 2 * (XSLICES + 1) * 3];
unsigned int vao;
unsigned int buf;
unsigned int plane;
float inclineAngle = 30; // degrees
float inclineSecondPlane = 20; // relative to first plane
float g = 9.8; // gravitational acceleration
int onFirstPlane = 1;
float timeDiff; // time of changing to second plane, in seconds

void fillVertices()
{
  int index = 0;
  for (int i = 0; i < YSLICES; i++)
    {
      float y1 = -30 + (60.0 * i / YSLICES);
      float y2 = -30 + (60.0 * (i+1) / YSLICES);
      for (int j = 0; j <= XSLICES; j++)
	{
	  float x = -30 + (60.0 * j / XSLICES);
	  vertices[index++] = x;         // 6 * i * (XSLICES + 1) + 6 * j
	  vertices[index++] = y1;    // 6 * i * (XSLICES + 1) + 6 * j + 1
	  vertices[index++] = 0;     // 6 * i * (XSLICES + 1) + 6 * j + 2
	  
	  vertices[index++] = x;
	  vertices[index++] = y2;
	  vertices[index++] = 0;
	}
    }
}

void createDispList()
{
  plane = glGenLists(1);
  glNewList(plane, GL_COMPILE);
  glBindVertexArray(vao);
  for (int i = 0; i < YSLICES; i++)
    glDrawArrays(GL_TRIANGLE_STRIP, i * (XSLICES + 1) * 2, (XSLICES + 1) * 2);
  glEndList();
}

void display()
{
  float ballDistance, ballDistanceSecond;
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  
  glTranslatef(30, 0, -70);
  glRotatef(60, 0, -1, 0);

  // draw first plane
  glColor3f(0, 0, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glRotatef(90 - inclineAngle, -1, 0, 0);
  glCallList(plane);

  // second plane
  glPushMatrix();
  glTranslatef(0, -30, 0);
  glRotatef(inclineSecondPlane, -1, 0, 0);
  glTranslatef(0, -30, 0);
  glCallList(plane);
  glPopMatrix();
  
  if (onFirstPlane)
    {
      ballDistance =  (g * seconds * seconds / 2.0) * sin(inclineAngle * PI / 180);
      if (ballDistance > 60) {onFirstPlane = 0; timeDiff = seconds;}
      else
	{
	  glTranslatef(0, 30 - ballDistance, 5); // slide
	  glRotatef(ballDistance / 3 * 180/PI, 1, 0, 0); // roll
	        // (arclength/radius gives angle in radians)
	  glutWireSphere(5, 10, 10);
	}
    }
  if (!onFirstPlane)
    {
      glTranslatef(0, -30, 0); // move to the bottom of the first plane
      ballDistanceSecond = g * timeDiff * sin(inclineAngle * PI / 180) * (seconds - timeDiff) +
	g * (seconds-timeDiff) * (seconds-timeDiff) / 2 * sin((inclineAngle-inclineSecondPlane) * PI/180);
      glRotatef(inclineSecondPlane, -1, 0, 0);
      glTranslatef(0, -ballDistanceSecond, 3);
      glRotatef(ballDistance / 3 * 180/PI - inclineSecondPlane, 1, 0, 0); // rotation after first plane
      glRotatef(ballDistanceSecond / 3 * 180/PI, 1, 0, 0); // roll on second plane
    }
  glColor3f(1, 0, 0);
  glutWireSphere(5, 10, 10);
  glutSwapBuffers();
}

void animate(int val)
{
  if (isAnimating)
    {
      frame++;
      seconds = frame * frameDuration / 1000.0;
      glutPostRedisplay();
      glutTimerFunc(frameDuration, animate, 1);
    }
}

void setup()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &buf);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, buf);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  fillVertices();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  createDispList();
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
      seconds = 0;
      onFirstPlane = 1;
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
    case GLUT_KEY_UP:
      inclineAngle += 5;
      break;
    case GLUT_KEY_DOWN:
      inclineAngle -= 5;
      break;
    }
  glutPostRedisplay();
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
