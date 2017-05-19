
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#define MAX_PARTICLES 100000
#define WCX		640
#define WCY		480
#define RAIN	0

float ground_points[21][21][3];
float ground_colors[21][21][4];
float slowdown = 2.0;
float velocity = 4.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.05;

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
}


int loop;
int fall;
//angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
int precipitation=0;
// XZ position of the camera
float x=0.0f, z=5.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
typedef struct {
  // Life
  bool alive;	// is the particle alive?
  float life;	// particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 3;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 200) - 10;
    par_sys[i].ypos = 50;
    par_sys[i].zpos = (float) (rand() % 200) - 10;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = 5;
    par_sys[i].gravity = -5;//-0.8;

}
particles par_sys2[MAX_PARTICLES];
void initParticles2(int i) {
    par_sys2[i].alive = true;
    par_sys2[i].life = 3;
    par_sys2[i].fade = float(rand()%100)/1000.0f+0.003f;

    par_sys2[i].xpos = (float) (rand() % 200) - 10;
    par_sys2[i].ypos = 50;
    par_sys2[i].zpos = (float) (rand() % 200) - 10;

    par_sys2[i].red = 0.5;
    par_sys2[i].green = 0.5;
    par_sys2[i].blue = 1.0;

    par_sys2[i].vel = 1;
    par_sys2[i].gravity = -3;//-0.8;

}
void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawbox() {

	glColor3f(0.4f, 0.4f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	//glutSolidSphere(0.75f,20,20);
	glutSolidCube(2);

}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}
// For Rain
void drawRain() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].life = -1.0;
      }
      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

void drawRain2() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys2[loop].alive == true) {
      x = par_sys2[loop].xpos;
      y = par_sys2[loop].ypos;
      z = par_sys2[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      par_sys2[loop].ypos += par_sys2[loop].vel / (slowdown*1000);
      par_sys2[loop].vel += par_sys2[loop].gravity;
      // Decay
      par_sys2[loop].life -= par_sys2[loop].fade;

      if (par_sys2[loop].ypos <= -10) {
        par_sys2[loop].life = -1.0;
      }
      //Revive
       if (par_sys[loop].ypos <= -10) {
        int zi = z - zoom + 10;
        int xi = x + 10;
        ground_colors[zi][xi][0] = 1.0;
        ground_colors[zi][xi][2] = 1.0;
        ground_colors[zi][xi][3] += 1.0;
        if (ground_colors[zi][xi][3] > 1.0) {
          ground_points[xi][zi][1] += 0.1;
        }
        par_sys[loop].life = -1.0;
      }

      if (par_sys2[loop].life < 0.0) {
        initParticles2(loop);
      }
    }
  }
}
void drawHail() {
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys2[loop].alive == true) {
      x = par_sys2[loop].xpos;
      y = par_sys2[loop].ypos;
      z = par_sys2[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (par_sys2[loop].ypos <= -10) {
        par_sys2[loop].vel = par_sys2[loop].vel * -1;
      }
      par_sys2[loop].ypos += par_sys2[loop].vel / (slowdown*1000); // * 1000
      par_sys2[loop].vel += par_sys2[loop].gravity;

      // Decay
      par_sys2[loop].life -= par_sys2[loop].fade;

      //Revive
      if (par_sys2[loop].life < 0.0) {
        initParticles2(loop);
      }
    }
  }
}
void renderScene(void) {

	//Add ambient light
GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
	
		glEnable(GL_TEXTURE_2D);

	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

// Draw ground

	
	glBegin(GL_QUADS);
	glColor3f(0.1f, 1, .7f);
//		glVertex3f(-100.0f, 0.0f, -100.0f);
//		glVertex3f(-100.0f, 0.0f,  100.0f);
//		glVertex3f( 100.0f, 0.0f,  100.0f);
//		glVertex3f( 100.0f, 0.0f, -100.0f);

	
		
//	  glColor3f(0.0f,100.0f,0.0f);    // Color Blue
//    glVertex3f( 1000000.0f, 10000.0f,-10.0f);    // Top Right Of The Quad (Top)
//    glVertex3f(-1000000.0f, 10000.0f,-10.0f);    // Top Left Of The Quad (Top)
//    glVertex3f(-1000000.0f, 10000.0f, 10.0f);    // Bottom Left Of The Quad (Top)
//    glVertex3f( 10000.0f, 10000.0f, 10.0f);    // Bottom Right Of The Quad (Top)
   // glColor3f(1.0f,0.5f,0.0f);    // Color Orange
   	glColor3f(0.1f, 1, .7f);
    glVertex3f( 100.0f,0, 100.0f);    // Top Right Of The Quad (Bottom)
    glVertex3f(-100.0f,0, 100.0f);    // Top Left Of The Quad (Bottom)
    glVertex3f(-100.0f,0,-100.0f);    // Bottom Left Of The Quad (Bottom)
    glVertex3f( 100.0f,0,-100.0f);    // Bottom Right Of The Quad (Bottom)
    glColor3f(0.0f,15.0f,100.0f); 
	//glColor3f(1.0f,0.0f,0.0f);    // Color Red    
    glVertex3f( 100000.0f, 10000.0f, 50.0f);    // Top Right Of The Quad (Front)
    glVertex3f(-100000.0f, 10000.0f, 50.0f);    // Top Left Of The Quad (Front)
    glVertex3f(-100000.0f,-10000.0f, 50.0f);    // Bottom Left Of The Quad (Front)
    glVertex3f( 100000.0f,-10000.0f,50.0f);    // Bottom Right Of The Quad (Front)
    //glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
    glColor4f(0.0f,15.0f,100.0f,0); 
	glVertex3f( 10000.0f,-10000.0f,-50.0f);    // Top Right Of The Quad (Back)
    glVertex3f(-10000.0f,-10000.000f,-50.0f);    // Top Left Of The Quad (Back)
    glVertex3f(-10000.0f, 10000.000f,-50.0f);    // Bottom Left Of The Quad (Back)
    glVertex3f( 10000.0f, 10000.000f,-50.0f);    // Bottom Right Of The Quad (Back)
    glColor4f(0.0f,15.0f,100.0f,0);  // Color Blue
    glVertex3f(-50.0f, 10000.0f, 10000.0f);    // Top Right Of The Quad (Left)
    glVertex3f(-50.0f, 10000.0f,-10000.0f);    // Top Left Of The Quad (Left)
    glVertex3f(-50.0f,-10000.0f,-10000.0f);    // Bottom Left Of The Quad (Left)
    glVertex3f(-50.0f,-10000.0f, 10000.0f);    // Bottom Right Of The Quad (Left)
    //glColor3f(100.0f,0.0f,100.0f);    // Color Violet
    glColor4f(0.0f,15.0f,100.0f,0); 
	glVertex3f( 50.0f, 10000.0f,-10000.0f);    // Top Right Of The Quad (Right)
    glVertex3f( 50.0f, 10000.0f, 10000.0f);    // Top Left Of The Quad (Right)
    glVertex3f( 50.0f,-10000.0f, 10000.0f);    // Bottom Left Of The Quad (Right)
    glVertex3f( 50.0f,-10000.0f,-10000.0f);
	glEnd();



	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
                glPushMatrix();
                
                    glTranslatef(i*10.0,0,j * 10.0);
                     drawbox();
                    glPopMatrix();
               }
               
               switch(precipitation)
               {
                case 0:
                break;
                case 1:
                drawRain();
                break;
                case 2:
                drawHail();
                break;
               }
//              drawRain();
//              //drawRain2();
//                drawHail();
        glutSwapBuffers();
} 

void processNormalKeys(unsigned char key, int xx, int yy) { 	

        if (key == 27)
              exit(0);
              
          if(key=='s')
		   precipitation=0;
		   
		      if(key=='r')
		   precipitation=1;
		   
		      if(key=='h')
		   precipitation=2;
		       
              
} 

void pressKey(int key, int xx, int yy) {

       switch (key) {
             case GLUT_KEY_UP : deltaMove = 0.5f; break;
             case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
       }
} 

void releaseKey(int key, int x, int y) { 	

        switch (key) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0;break;
        }
} 

void mouseMove(int x, int y) { 	

         // this will only be true when the left button is down
         if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	initRendering();
	glutCreateWindow("CGV-Rain Project");
   for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles2(loop);
        initParticles(loop);
    }
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
