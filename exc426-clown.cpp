//
// Clown's head with moving hat, ears and nose
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//


#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float angle = 0.0; // Rotation.angle of hat.
static float yRotation = 30;
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.

// Drawing routine.
void drawScene(void)
{
   float t; // Parameter along helix.
   float s = sin((PI/180) * angle);

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glLineWidth(1);
   // Place scene in frustum.
   glTranslatef(0.0, 0.0, -9.0);
   glRotatef(yRotation, 0, 1, 0);
   
   // Head.
   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 20, 20);

  
   glPushMatrix();   
   // Transformations of the hat and brim.
   glRotatef(angle, 0.0, 1.0, 0.0);
   glRotatef(30.0, 0.0, 0.0, 1.0);
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 0.0, 2.0);

   // Hat.
   glColor3f(0.0, 1.0, 0.0);
   glutWireCone(2.0, 4.0, 20, 20);   

   // Brim.
   glutWireTorus(0.2, 2.2, 10, 25);
   glPopMatrix();

   glPushMatrix();
   // Transformations of the left ear.
   glTranslatef(s, 0.0, 0.0);
   glTranslatef(3.5, 0.0, 0.0);

   // Left ear.
   glColor3f(1.0, 0.0, 0.0);
   glutWireSphere(0.5, 10, 10);
   glPopMatrix();

   glPushMatrix();   
   // Transformations of the right ear.
   glTranslatef(-s, 0.0, 0.0);
   glTranslatef(-3.5, 0.0, 0.0);

   // Right ear.
   glColor3f(1.0, 0.0, 0.0);
   glutWireSphere(0.5, 10, 10);
   glPopMatrix();

   // left eye
   glPushMatrix();
   glTranslatef(0.5 + s / 5, 1 + cos(angle*(PI/180)) / 5, sqrt(4 - 1 - 0.25)); // i hope this is calculated in compile-time
   glRotatef(angle * 3, 0, 0, 1);

   glColor3f(0.5, 1.0, 1.0);
   glutWireSphere(0.4, 10, 10);
   glPopMatrix();
   
   // right eye
   glPushMatrix();
   glTranslatef(-0.5 - s / 5, 1 + cos(angle*(PI/180)) / 5, sqrt(4 - 1 - 0.25)); // i hope this is calculated in compile-time
   glRotatef(-angle * 3, 0, 0, 1);

   glColor3f(0.5, 1.0, 1.0);
   glutWireSphere(0.4, 10, 10);
   glPopMatrix();
   

   glPushMatrix();
   // transformations for the nose
   glTranslatef(0, 0, 3.5 + s);

   // nose
   glColor3f(1.0, s * s * 0.6, 0);
   glutWireCone(0.5, 1.0 + s / 2, 10, 10);
   glPopMatrix();
   
   glPushMatrix();
   // Transformations of the spring to the left ear.
   glTranslatef(-2.0, 0.0, 0.0);
   glScalef(-1 - s, 1.0, 1.0);

   // Spring to left ear.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   for(t = 0.0; t <= 1.0; t += 0.05) 
      glVertex3f(t, 0.25 * cos(10.0 * PI * t), 0.25 * sin(10.0 * PI * t));
   glEnd();
   glPopMatrix();
   
   glPushMatrix();
   // Transformations of the spring to the right ear.
   glTranslatef(2.0, 0.0, 0.0);
   glScalef(1 + s, 1.0, 1.0);
   
   // Spring to right ear.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   for(t = 0.0; t <= 1.0; t += 0.05) 
      glVertex3f(t, 0.25 * cos(10.0 * PI * t), 0.25 * sin(10.0 * PI * t));
   glEnd();
   glPopMatrix();

   glPushMatrix();
   // transformations for spring to nose
   glTranslatef(0, 0, 2);
   glRotatef(-90, 0, 1, 0);
   glScalef(1.5 + s, 1, 1);

   // spring to nose
   glColor3f(0.8, 0.0, 0.1);
   glLineWidth(3);
   glBegin(GL_LINE_STRIP);
   for(t = 0.0; t <= 1.0; t += 0.05) 
     glVertex3f(t, 0.25 * cos(10.0 * PI * t), 0.25 * sin(10.0 * PI * t));
   glEnd();
   glPopMatrix();
   

   glutSwapBuffers();
}

// Routine to increase the rotation angle.
void increaseAngle(void)
{
   angle += 5.0;
   if (angle > 360.0) angle -= 360.0;
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      increaseAngle();

      glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
   case 'y':
     yRotation += 5;
     if (yRotation > 360) yRotation -= 360;
     break;
   case 'Y':
     yRotation -= 5;
     if (yRotation < 360) yRotation += 360;
     break;
   case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("clown3.cpp"); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}


