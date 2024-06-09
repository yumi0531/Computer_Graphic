/*-----------------------------------------------------------
 * An example program to draw a car
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#define PI 3.141592653

#define Step 0.5
//動作
#define STOP 1
#define WALK 2
#define RUN 3
// line mode
#define FILL 1
#define LINE 2
#define PI 3.141592653
int curw;
int curh;
int swingLeft = 0;
int swingRight = 0;
int isSwingForward = 0;
int curTurn = 180;
int maxAngel = 0;
int stepDis = 0;
float speed = 0;
int isStand = 1;
float curDistanceX = 0, curDistanceZ = 0;
float legDis = 0;
int polygonMode = FILL;
int running = 0;
int jumpHeight = 0;
int isjump = 0;
int isdown = 0;
float currentHeight = -0.5;
float body_height = 0.5;
int iskneeldown = 0;
float Roatebody = 0.0;
float Roateleg = 0.0;
float RoateBigleg = 0.0;
int is_line = 0;
float wink = 0.5;
int head_swing = 0;
int head_nod = 0;
int head_forward = 0;
int head_back = 0;
int animate = 0;

GLUquadricObj *wheel_A_cycle_1 = NULL, *wheel_B_cycle_1 = NULL, *wheel_C_cycle_1 = NULL;
GLUquadricObj *wheel_A_cycle_2 = NULL, *wheel_B_cycle_2 = NULL, *wheel_C_cycle_2 = NULL;

GLUquadricObj *mo_arm1 = NULL, *mo_arm2 = NULL, *mo_arm3 = NULL, *mo_arm4 = NULL;
/*-----Define a unit box--------*/
/* Vertices of the box */
float points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
float colors[6][3] = {{0.5, 0.5, 0.5}, {0.7, 0.7, 0.7}, {0.7, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, {0.5, 0.5, 0.7}};
/* indices of the box faces */
int cube[6] = {0, 1, 2, 3, 4, 5};
float points_twelve[][3] = {{1, 0, 0}, {0.866, 0, 0.5}, {0.5, 0, 0.866}, {0, 0, 1}, {-0.5, 0, 0.866}, {-0.866, 0, 0.5}, {-1, 0, 0}, {-0.866, 0, -0.5}, {-0.5, 0, -0.866}, {0, 0, -1}, {0.5, 0, -0.866}, {0.866, 0, -0.5}, {1, 1, 0}, {0.866, 1, 0.5}, {0.5, 1, 0.866}, {0, 1, 1}, {-0.5, 1, 0.866}, {-0.866, 1, 0.5}, {-1, 1, 0}, {-0.866, 1, -0.5}, {-0.5, 1, -0.866}, {0, 1, -1}, {0.5, 1, -0.866}, {0.866, 1, -0.5}};
float mo_angle = 0.0;
/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj *sphere = NULL, *cylind = NULL, *disk;

/*-Declare car position, orientation--*/
float self_ang = -90.0, glob_ang = 0.0;
float position[3] = {8.0, 0.0, 0.0};

/*-----Define window size----*/
int width = 512, height = 512;
float arm_ang = 100.0, bld_ang = 10.0;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void draw_blade()
{
  glBegin(GL_POLYGON);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 4.0, 0.0);
  glVertex3f(1.0, 8.0, 0.0);
  glVertex3f(-1.0, 8.0, 0.0);
  glVertex3f(-1.0, 4.0, 0.0);
  glEnd();
}
void myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0); /*set the background color BLACK */
                                    /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  /*---Create quadratic objects---*/
  if (sphere == NULL)
  {
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  if (cylind == NULL)
  {
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }
  if (disk == NULL)
  {
    disk = gluNewQuadric();
    gluQuadricDrawStyle(disk, GLU_FILL);
    gluQuadricNormals(disk, GLU_SMOOTH);
  }
}

/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_cube()
{
  int i;

  for (i = 0; i < 6; i++)
  {
    glColor3fv(colors[i]); /* Set color */
    glBegin(GL_POLYGON);   /* Draw the face */
    glVertex3fv(points[face[i][0]]);
    glVertex3fv(points[face[i][1]]);
    glVertex3fv(points[face[i][2]]);
    glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

void draw_circle() //平面圓形
{
  glBegin(GL_TRIANGLE_FAN); //扇形填充
  glVertex3f(0.0f, 0.0f, 0.0f);
  int i = 0;
  for (i = 0; i <= 390; i += 15)
  {
    float p = i * 3.14 / 180;
    glVertex3f(sin(p), cos(p), 0.0f);
  }
  glEnd();
}

void draw_cone()
{
  glBegin(GL_QUAD_STRIP);
  int i = 0;
  for (i = 0; i <= 390; i += 15)
  {
    float p = i * 3.14 / 180;
    glVertex3f(0, 0, 1.0f);
    glVertex3f(sin(p), cos(p), 0.0f);
  }
  glEnd();
  draw_circle();
}
/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{
  int i, j;

  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++)
    {
      if ((i + j) % 2 == 0)
        glColor3f(1.0, 0.8, 0.8);
      else
        glColor3f(0.1, 0.1, 0.7);
      glBegin(GL_POLYGON);
      glVertex3f((i - 5.0) * 10.0, -2.5, (j - 5.0) * 10.0);
      glVertex3f((i - 5.0) * 10.0, -2.5, (j - 4.0) * 10.0);
      glVertex3f((i - 4.0) * 10.0, -2.5, (j - 4.0) * 10.0);
      glVertex3f((i - 4.0) * 10.0, -2.5, (j - 5.0) * 10.0);
      glEnd();
    }
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{

  /*----Draw a white sphere to represent the original-----*/
  glColor3f(0.9, 0.9, 0.9);

  gluSphere(sphere, 2.0, /* radius=2.0 */
            12,          /* composing of 12 slices*/
            12);         /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
  glColor3f(0.0, 0.95, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
              10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);               /* Divide it into 3 sections */

  /* Draw Y axis */
  glPushMatrix();
  glRotatef(-90.0, 1.0, 0.0, 0.0); /*Rotate about x by -90', z becomes y */
  glColor3f(0.95, 0.0, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
              10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);               /* Divide it into 3 sections */
  glPopMatrix();

  /* Draw X axis */
  glColor3f(0.95, 0.95, 0.0);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0); /*Rotate about y  by 90', x becomes z */
  gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
              10.0,               /* height of the cylinder */
              12,                 /* use 12-side polygon approximating circle*/
              3);                 /* Divide it into 3 sections */
  glPopMatrix();
  /*-- Restore the original modelview matrix --*/
  glPopMatrix();
}

void draw_twelve()
{
  int i;
  glBegin(GL_POLYGON); /* Draw the face */
  for (i = 0; i < 12; i++)
  {
    if (i % 2 == 0 && is_line == 1)
    {
      glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
    }
    else if (i % 2 == 1 && is_line == 1)
    {
      glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
    }
    glVertex3fv(points_twelve[i]);
  }
  glEnd();

  for (i = 0; i <= 10; i++)
  {
    if (i % 2 == 0 && is_line == 1)
    {
      glColor3f(1.0, 1.0, 1.0);
    }
    else if (i % 2 == 1 && is_line == 1)
    {
      glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
    }
    glBegin(GL_POLYGON); /* Draw the face */
    glVertex3fv(points_twelve[0 + i]);
    glVertex3fv(points_twelve[12 + i]);
    glVertex3fv(points_twelve[13 + i]);
    glVertex3fv(points_twelve[1 + i]);
    glEnd();
  }
  if (i % 2 == 0 && is_line == 1)
  {
    glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
  }
  else if (i % 2 == 1 && is_line == 1)
  {
    glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
  }
  glBegin(GL_POLYGON); /* Draw the face */
  glVertex3fv(points_twelve[11]);
  glVertex3fv(points_twelve[23]);
  glVertex3fv(points_twelve[12]);
  glVertex3fv(points_twelve[0]);
  glEnd();

  glBegin(GL_POLYGON); /* Draw the face */
  if (i % 2 == 0 && is_line == 1)
  {
    glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
  }
  else if (i % 2 == 1 && is_line == 1)
  {
    glColor3f((GLfloat)255 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);
  }
  for (i = 12; i < 23; i++)
  {
    glVertex3fv(points_twelve[i]);
  }
  glEnd();
}
/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  static float ang_self = 0.0; /*Define the angle of self-rotate */
  static float angle = 0.0;

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(8.0, 5.0, 15.0, 4.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  draw_floor();

  draw_axes();
  glTranslatef(-5.0, -1.5, 0.0);
  // draw_axes();

  glPopMatrix();

  // object
  glTranslatef(0.0, 5.0, 0.0);

  glRotatef(50.0, 0.0, 1.0, 0.0);

  glPushMatrix();
  glTranslatef(-1, 2, 0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  if (mo_arm1 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm1 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm1, GLU_FILL);
    gluQuadricNormals(mo_arm1, GLU_SMOOTH);
  }
  if (mo_arm2 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm2 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm2, GLU_FILL);
    gluQuadricNormals(mo_arm2, GLU_SMOOTH);
  }
  if (mo_arm3 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm3 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm3, GLU_FILL);
    gluQuadricNormals(mo_arm3, GLU_SMOOTH);
  }
  if (mo_arm4 == NULL)
  { /* allocate a quadric object, if necessary */
    mo_arm4 = gluNewQuadric();
    gluQuadricDrawStyle(mo_arm4, GLU_FILL);
    gluQuadricNormals(mo_arm4, GLU_SMOOTH);
  }
  glColor3f(0.5, 0.5, 0.5);
  glPushMatrix();
  glTranslatef(0.0, 3, 0);
  glRotatef(-20.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm1, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glRotatef(40.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm2, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glPopMatrix();
  glPushMatrix();
  // glTranslatef(0.0,3,0);
  glRotatef(-20.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm3, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glRotatef(40.0, 0.0, 1.0, 0.0);
  gluCylinder(mo_arm4, 0.2, 0.2, /* radius of top and bottom circle */
              9,                 /* height of the cylinder */
              12,                /* use 12-side polygon approximating circle*/
              3);                /* Divide it into 3 sections */
  glPopMatrix();
  glRotatef(mo_angle, 0.0, 1.0, 0.0);
  glScalef(5.0, 3.0, 5.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(5);
  is_line = 1;
  draw_twelve();
  is_line = 0;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f((GLfloat)255 / 255, (GLfloat)153 / 255, (GLfloat)255 / 255);
  draw_twelve();
  glPopMatrix();

  glLoadIdentity(); /* Initialize modelview matrix */
  glPushMatrix();
  //

  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(0.375, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(RoateBigleg, 1.0, 0.0, 0.0);
    // glTranslatef(0.0, -0.2, 0.0);
  }
  if (animate)
  {
    glRotatef((GLfloat)swingRight, 0.0, 0.0, 1.0);
  }

  glRotatef((GLfloat)swingRight, 1.0, 0.0, 0.0);

  glTranslatef(0.0, -0.5, 0.0);
  glColor3f((GLfloat)0 / 255, (GLfloat)255 / 255, (GLfloat)255 / 255);

  glPushMatrix();
  glScalef(0.7, 1.0, 1.0);
  draw_cube();
  glPopMatrix();

  glTranslatef(0.0, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roateleg, 1.0, 0.0, 0.0);
  }
  glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0);

  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)153 / 255, 1.0);

  // glColor3f(1.0, 0.0, 0.0);

  glPushMatrix();
  glScalef(0.5, 1.0, 0.7);
  draw_cube();
  // right feet
  glPopMatrix();

  glTranslatef(0.0, -0.5, -0.1);
  glColor3f(1.0, 1.0, 1.0);

  glPushMatrix();
  glScalef(0.7, 0.1, 1.0);
  draw_cube();
  glPopMatrix(); // pop to right

  glPopMatrix(); // pop to right

  // left leg
  glPushMatrix();

  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(-0.375, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(RoateBigleg, 1.0, 0.0, 0.0);
    // glTranslatef(0.0, -0.2, 0.0);
  }

  glRotatef((GLfloat)swingLeft, 1.0, 0.0, 0.0);
  glTranslatef(0.0, currentHeight, 0.0);
  glColor3f((GLfloat)0 / 255, (GLfloat)255 / 255, (GLfloat)255 / 255);

  // glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.7, 1.0, 1.0);
  draw_cube();
  glPopMatrix();

  // left little leg
  glTranslatef(0.0, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roateleg, 1.0, 0.0, 0.0);
  }
  glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)153 / 255, 1.0);
  glPushMatrix();
  glScalef(0.5, 1.0, 0.7);

  draw_cube();
  glPopMatrix();
  // left feet
  glTranslatef(0.0, -0.5, -0.1);
  glColor3f(1.0, 1.0, 1.0);

  glPushMatrix();
  // glScalef(0.5, 0.1, 0.7);
  glScalef(0.7, 0.1, 1.0);
  draw_cube();

  glPopMatrix(); // pop to
  glPopMatrix(); // pop to
                 // /*-------Draw the body of the robot which is a cube----*/
  // body
  glPushMatrix();
  glTranslatef(curDistanceX, 0.9, curDistanceZ);

  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);
  if (running)
  {
    glRotatef(-30, 1.0, 0.0, 0.0);
  }
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0);
  }

  glTranslatef(0.0, body_height, 0.0);
  glColor3f((GLfloat)153 / 255, (GLfloat)204 / 255, (GLfloat)255 / 255);

  glPushMatrix();
  glScalef(2.0, 3.0, 1.5);
  draw_cube();

  /*-------Draw the cylinder, arm of the windmill------*/

  glColor3f(0.68, 0.68, 0.68); /* Gray colored */
  glTranslatef(0.0, 0.0, 0.0);

  // glRotatef(-90.0, 1.0, 0.0, 0.0); /* Roate about x axis, z become y,
  //                                     and y become -z (變得)*/

  if (cylind == NULL) // back windmill
  {                   /* allocate a quadric object, if necessary */
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }

  /*--- Draw a cylinder ---*/
  gluCylinder(cylind, 0.1, 0.1, /* radius of top and bottom circle */
              1,                /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);
  /*----- Draw a unit sphere ---*/
  glTranslatef(0.0, 0.0, 1.1);
  if (sphere == NULL)
  {
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }

  glColor3f(0.95, 0.2, 0.2);
  gluSphere(sphere, 0.15, /* radius=2.0 */
            12,           /* composing of 12 slices*/
            12);          /* composing of 12 stacks */

  /*------Draw three blades ---*/
  // glPopMatrix();
  // glRotatef(60.0, 1.0, 0.0, 0.0);
  glScalef(0.1, 0.1, 0.1);
  glColor3f(1.0, 1.0, 1.0);

  glRotatef(bld_ang, 0.0, 0.0, 1.0); /* Rotate about x axis, M5 coord. sys. */
  draw_blade();                      /* draw the first blade */

  glRotatef(120.0, 0.0, 0.0, 1.0); /* rotate by 120 degree, M6 coord. sys */
  draw_blade();                    /* draw 2nd blade */
  glRotatef(120.0, 0.0, 0.0, 1.0); /* rotate by 120 degree, M6 coord. sys */
  draw_blade();                    /* draw 2nd blade */

  glPopMatrix();
  // head
  glTranslatef(0.0, 2.5, 0.0);
  glRotatef((GLfloat)head_swing, 0.0, 1.0, 0.0);
  glRotatef((GLfloat)head_nod, 1.0, 0.0, 0.0);
  glColor3f(0.9, 0.9, 0.9);
  glPushMatrix();
  glScalef(3.0, 2.0, 2.5);
  draw_cube();
  glPopMatrix();

  // left eyes
  glTranslatef(0.5, 0.0, -1.2);
  glColor3f(0.2, 0.2, 0.2);
  // glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.5, 0.5, 0.5);
  draw_cube();

  // right eyes
  glTranslatef(-1.0, 0.0, 0.0);
  glColor3f(0.2, 0.2, 0.2);
  // glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.5, wink, 0.5);
  draw_cube();
  glPopMatrix();
  glPopMatrix();
  /*-------Draw the ear of the robot which is a cone----*/
  // glTranslatef(curDistanceX, 0.9, curDistanceZ);
  // glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);
  glTranslatef(-0.3, 1.0, 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glColor3f(0.0, (GLfloat)102 / 255, (GLfloat)204 / 255);
  glScalef(0.65, 0.65, 0.65);
  draw_cone();
  glTranslatef(2.35, 0.0, 0.0);
  draw_cone();
  glPopMatrix();

  // left arm
  glPushMatrix();
  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(1.1, 1.75, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0);
  }
  glRotatef((GLfloat)swingLeft, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)102 / 255, (GLfloat)153 / 255);
  glPushMatrix();
  glScalef(0.3, 1.6, 0.4);
  draw_cube();
  glPopMatrix();
  glPopMatrix();
  // right arm
  glPushMatrix();
  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(-1.1, 1.75, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0);
  }
  if (animate)
  {
    glRotatef(200, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)swingRight, 0.0, 0.0, 1.0);
  }
  glRotatef((GLfloat)swingRight, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)102 / 255, (GLfloat)153 / 255);
  glPushMatrix();
  glScalef(0.3, 1.6, 0.4);
  draw_cube();
  glPopMatrix();
  glPopMatrix();
  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  return;
}

/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w > h)
    glOrtho(-40.0, 40.0, -40.0 * (float)h / (float)w, 40.0 * (float)h / (float)w,
            -100.0, 100.0);
  else
    glOrtho(-40.0 * (float)w / (float)h, 40.0 * (float)w / (float)h, -40.0, 40.0,
            -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void stop()
{
  running = 0;
  swingLeft = 0;
  swingRight = 0;
  isSwingForward = 0;
  legDis = 0;
}

void timer(int value)
{
  mo_angle += 0.02;

  // windmill
  bld_ang += 5.0;
  if (bld_ang > 360.0)
  {
    bld_ang -= 360.0;
    arm_ang += 3.0;
    if (arm_ang > 360.0)
      arm_ang -= 360.0;
  }
  // action
  int tempTurn = curTurn;
  curDistanceX = curDistanceX - speed * sin((GLfloat)tempTurn / 360 * PI * 2);
  curDistanceZ = curDistanceZ - speed * cos((GLfloat)tempTurn / 360 * PI * 2);
  if (!isSwingForward)
  {
    swingLeft = (swingLeft + stepDis);
    swingRight = (swingRight - stepDis);
    if (swingLeft > 0)
    {
      legDis = legDis - stepDis;
    }
    else
    {
      legDis = legDis + stepDis;
    }
  }
  else
  {
    swingLeft = (swingLeft - stepDis);
    swingRight = (swingRight + stepDis);
    if (swingLeft < 0)
    {
      legDis = legDis - stepDis * 1.2;
    }
    else
    {
      legDis = legDis + stepDis * 1.2;
    }
  }
  if (swingLeft > maxAngel)
  {
    isSwingForward = 1;
  }
  if (swingLeft < maxAngel * -1)
  {
    isSwingForward = 0;
  }
  if (isjump)
  {
    currentHeight += 1.0;
    if (currentHeight > 5.0)
    {
      isdown = 1;
    }
    if (isdown)
    {
      currentHeight -= 1.0;
    }

    currentHeight = -0.5;
  }
  display();
  glutPostRedisplay();
  if (!isStand)
    glutTimerFunc(value, timer, value);
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_keyboard(unsigned char key, int x, int y)
{
  if (key == 'Q' || key == 'q')
    exit(0);
  // if (key == 'r')
  // {
  //   arm_ang += 5.0;
  //   if (arm_ang > 360.0)
  //     arm_ang -= 360.0;
  // }
  if (key == 'a')
  {
    bld_ang += 10.0;
    if (bld_ang > 360.0)
      bld_ang -= 360.0;
    speed = 0.07;
  }
  else if (key == 'z') // counter-clockwise
  {
    curTurn = (curTurn - 5) % 360;
  }
  else if (key == 'x') // clowise
  {
    curTurn = (curTurn + 5) % 360;
    // glutPostRedisplay();
  }
  else if (key == 'j')
  {
    // printf("jump");
    isjump = 1;
    glutPostRedisplay();
  }
  else if (key == 'w')
  {
    stop();
    maxAngel = 20;
    stepDis = 1;
    speed = 0.02;

    if (isStand)
      glutTimerFunc(20, timer, 20);
    isStand = 0;
  }
  else if (key == 'r')
  {
    stop();
    running = 1;
    maxAngel = 55;
    stepDis = 5;
    speed = 0.04;

    if (isStand)
      glutTimerFunc(10, timer, 10);
    isStand = 0;
  }
  else if (key == 's')
  {
    isStand = 1;
    iskneeldown = 0;
    body_height = 0.5;
    Roatebody = 0;
    Roateleg = 0;
    RoateBigleg = 0;
    animate = 0;
    head_nod = 0;
    stop();
  }
  else if (key == 'k')
  {
    stop();
    iskneeldown = 1;
    if (Roatebody >= -30)
    {
      body_height -= 0.02;
      Roatebody -= 1;
      Roateleg -= 1;
      RoateBigleg += 1;
    }
  }
  else if (key == 'p')
  {
    stop();
    if (wink >= 0.1)
    {
      wink -= 0.01;
    }
  }
  else if (key == 'P')
  {
    stop();
    if (wink <= 0.5)
    {
      wink += 0.01;
    }
  }
  else if (key == 'o')
  {
    swingLeft += 1;
  }
  else if (key == 'O')
  {
    swingLeft -= 1;
  }
  else if (key == 'h')
  {
    head_swing = (head_swing - 5) % 360;
  }
  else if (key == 'H')
  {
    // head_forward = 1;
    animate = 1;

    if (head_nod <= -20)
    {
      head_forward = 0;
    }
    else if (head_nod >= 20)
    {
      head_forward = 1;
    }
    if (head_forward)
    {

      head_nod -= 1;
    }
    else
    {
      head_nod += 1;
    }

    if (swingLeft <= -20)
    {
      isSwingForward = 0;
    }
    else if (swingLeft >= 20)
    {
      isSwingForward = 1;
    }

    if (isSwingForward)
    {
      swingLeft -= 1;
      swingRight += 1;
    }
    else
    {
      swingLeft += 1;
      swingRight -= 1;
    }
  }

  display();
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(width, height);
  glutCreateWindow("windmill");

  myinit(); /*---Initialize other state varibales----*/

  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  /*  glutIdleFunc(display); */
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_keyboard);

  glutMainLoop();
}
