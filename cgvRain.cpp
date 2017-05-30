
#include<iostream>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#define MAX_PARTICLES 100000
#define WCX		640
#define WCY		480
#define RAIN	0
using namespace std;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float slowdown = 2.0;
float velocity = 4.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.05;
int loop;
int fall;
float angle = 0.0f;
float lx=0.0f,lz=-1.0f;
int precipitation=0;
float x=0.0f, z=5.0f;
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
void initRendering() {
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_FOG);
}

typedef struct {
  // Life
  bool alive;	// checks the particle alive
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
  // Velocity
  float vel;
  // Gravity
  float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];
int rain_vel=3;
// Initialize/Reset Particles 
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 3;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 100) - 10;
    par_sys[i].ypos = 50;
    par_sys[i].zpos = (float) (rand() % 100) - 10;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = rain_vel;
    par_sys[i].gravity = -5;//-0.8;

}

particles par_sys3[MAX_PARTICLES];

void initParticlesMatNegative(int i,particles par_sys[MAX_PARTICLES]) {
    par_sys[i].alive = true;
    par_sys[i].life = 3;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

     par_sys[i].xpos = (float) (rand() % 100) - 10;
    par_sys[i].ypos = 50;
    par_sys[i].zpos = (float) (rand() % 100) - 10;
par_sys[i].xpos=par_sys[i].xpos*-1;
par_sys[i].zpos=-1*par_sys[i].zpos ;
    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel =rain_vel;
    par_sys[i].gravity = -3;//-0.8;

}
void changeSize(int w, int h) {
	if (h == 0)
		h = 1;
    float ratio =  w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void drawTree() {

    glPushMatrix();
	glColor3f(0.6,0.6,0); 
    glTranslatef(0.1f ,0.5f, 0.0f);
	glScalef(1,2.5,1);
	glutSolidCube(1);
	glPopMatrix();

    glPushMatrix();
	glColor3f(0,1,0.4); 
	glScalef(1.8,1.7,1.8);
	
   //glTranslatef(0.0f ,1.5f, 0.0f);
   //glutSolidIcosahedron();
	glTranslatef(0.0f ,2.4f, 0.0f);
    glutSolidDodecahedron();
	glPopMatrix();

    //glutSolidTetrahedron();
	//glutSolidIcosahedron();
    //glutSolidOctahedron();
  //glutSolidDodecahedron();
   //glutSolidTeapot(2);

}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
     cout<<x<<" "<<z<<endl;
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

      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
    
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

void drawRainMatNegative(particles par_sys2[]) {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys2[loop].alive == true) {
      x = par_sys2[loop].xpos;
      y = par_sys2[loop].ypos;
      z = -1*par_sys2[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
     glEnd();

    
      par_sys2[loop].ypos += par_sys2[loop].vel / (slowdown*1000);
      par_sys2[loop].vel += par_sys2[loop].gravity;
     
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
        initParticlesMatNegative(loop,par_sys3);
      }
    }
  }
}


int fogOn=0;//enable ,disable fog
int lightOn=0;//enable ,disable light



void renderScene(void) {

	/////FOG/////
	
	 GLfloat fogColor[] = {0.5f, 2.0f,8.0f, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 10.0f);
  glFogf(GL_FOG_END, 20.0f);
    
    
    //LIGHTING
	  GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f,0.5f};
	GLfloat directedLightPos[] = {1,60.0f,z, 30.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
	
	

	if (deltaMove)
		computePos(deltaMove);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

   
   
   // ground 
    glBegin(GL_QUADS);
	glColor3f(0.1f, 1, .7f);

   	glColor3f(0.1f, 1, .7f);
    glVertex3f( 100.0f,0, 100.0f);    // Top Right Of The Quad (Bottom)
    glVertex3f(-100.0f,0, 100.0f);    // Top Left Of The Quad (Bottom)
    glVertex3f(-100.0f,0,-100.0f);    // Bottom Left Of The Quad (Bottom)
    glVertex3f( 100.0f,0,-100.0f);    // Bottom Right Of The Quad (Bottom)
    
    //SKYBOX
    glColor3f(0.0f,15.0f,100.0f); //Sky color
   
    glVertex3f( 100000.0f, 10000.0f, 50.0f);    // Top Right Of The Quad (Front)
    glVertex3f(-100000.0f, 10000.0f, 50.0f);    // Top Left Of The Quad (Front)
    glVertex3f(-100000.0f,-10000.0f, 50.0f);    // Bottom Left Of The Quad (Front)
    glVertex3f( 100000.0f,-10000.0f,50.0f);    // Bottom Right Of The Quad (Front)
 
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
  
    glColor4f(0.0f,15.0f,100.0f,0); 
	glVertex3f( 50.0f, 10000.0f,-10000.0f);    // Top Right Of The Quad (Right)
    glVertex3f( 50.0f, 10000.0f, 10000.0f);    // Top Left Of The Quad (Right)
    glVertex3f( 50.0f,-10000.0f, 10000.0f);    // Bottom Left Of The Quad (Right)
    glVertex3f( 50.0f,-10000.0f,-10000.0f);
	glEnd();


/////Object(TREE) generation in Grid 9x9 
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
	
                glPushMatrix();
                
                    glTranslatef(i*10.0,0,j * 10.0);
                     drawTree();
                    glPopMatrix();
               }
               
               switch(precipitation)
               {
                case 0:
                //No Precipitation
                break;
                case 1:
                drawRainMatNegative(par_sys3);
                drawRain();
                break;
              
               }

        glutSwapBuffers();
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

     
         if (xOrigin >= 0) {

		
		deltaAngle = (x - xOrigin) * 0.001f;

		//Camera direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {
			xOrigin = x;
		}
	}
}

void menu(int id)
{
	if(id==0)
	{
		exit(0);
	}
	
	glutPostRedisplay();
}
void menu_effect(int id)
{
	if(id==1)
	{

if(fogOn==1)
{
	 glDisable(GL_FOG);
	 fogOn=0;
	 }
	 else
	 {
	 glEnable(GL_FOG);
	 fogOn=1;
	 }
	}
	else	if(id==2)
	{

			if(lightOn==1)
			{
			 glDisable(GL_LIGHTING);
			 glDisable(GL_LIGHT0);
	 		lightOn=0;
	 		}
	 	else
	 		{
	 		glEnable(GL_LIGHTING);
     		glEnable(GL_LIGHT0);
	 		lightOn=1;
	 	}
	}
	glutPostRedisplay();
}
void menu_rain(int id)
{
	if(id==0)
	{
		precipitation=1;
	}
	if(id==1)
	{
		precipitation=0;
	}if(id==2)
	{
		rain_vel+=1;
	}if(id==3)
	{
		rain_vel-=1;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(420,320);
	glutInitWindowSize(720,480);
	
	glutCreateWindow("CGV-Rain Project");
   for (loop = 0; loop < MAX_PARTICLES; loop++) {
   
        initParticles(loop);
        initParticlesMatNegative(loop,par_sys3);
    }
	initRendering();
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);


	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

     int rain_ch=glutCreateMenu(menu_rain);
    glutAddMenuEntry("start ",0);
    glutAddMenuEntry("stop ",1);
	glutAddMenuEntry("increase velocity",2);
	glutAddMenuEntry("decrease velocity",3);
    
    	int prec_ch=glutCreateMenu(menu);
    	 glutAddSubMenu("Rain",rain_ch);
    	glutCreateMenu(menu);
    	
		int effect_ch=glutCreateMenu(menu_effect);
        glutAddMenuEntry("Enable/Disable Fog",1);
    	glutAddMenuEntry("Enble/Disable Lighting",2);
    	
    glutCreateMenu(menu);
    glutAddSubMenu("Precipitation",prec_ch);
    glutAddSubMenu("Effects",effect_ch);
    glutAddMenuEntry("Exit",0);
	
      
	  
	  glutAttachMenu(GLUT_RIGHT_BUTTON);


	glEnable(GL_DEPTH_TEST);

	
	glutMainLoop();

	return 1;
}
