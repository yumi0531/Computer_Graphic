/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
//動作
#define STOP 1
#define WALK 2
#define RUN 3
// line mode
#define FILL 1
#define LINE 2
#define PI 3.141592653
#define Step 0.2
#define ANGLE 1.0

// projection & view
#define Orthographical_x 0
#define Orthographical_y 1
#define Orthographical_z 2
#define Perspective 3
#define show_All 4

int curw;
int curh;
int swingLeft = 0; //擺動
int swingRight = 0;
int isSwingForward = 0;
int curTurn = 180; //正面角度
int maxAngel = 0;  //最大轉動角度
int stepDis = 0;   //大腿擺動速度
float speed = 0;   //前進速度
int isStand = 1;   //停止(站著)
float curDistanceX = 0, curDistanceZ = 0;
float legDis = 0; //跨步距離(run or walk)
int polygonMode = FILL;
int running = 0; //跑步
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
//逆時針(順序不可亂)
int face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
/* indices of the box faces */
int cube[6] = {0, 1, 2, 3, 4, 5};
float points_twelve[][3] = {{1, 0, 0}, {0.866, 0, 0.5}, {0.5, 0, 0.866}, {0, 0, 1}, {-0.5, 0, 0.866}, {-0.866, 0, 0.5}, {-1, 0, 0}, {-0.866, 0, -0.5}, {-0.5, 0, -0.866}, {0, 0, -1}, {0.5, 0, -0.866}, {0.866, 0, -0.5}, {1, 1, 0}, {0.866, 1, 0.5}, {0.5, 1, 0.866}, {0, 1, 1}, {-0.5, 1, 0.866}, {-0.866, 1, 0.5}, {-1, 1, 0}, {-0.866, 1, -0.5}, {-0.5, 1, -0.866}, {0, 1, -1}, {0.5, 1, -0.866}, {0.866, 1, -0.5}};
float mo_angle = 0.0;
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj *sphere = NULL, *cylind = NULL;

/*-Declare robot position, orientation--*/
float self_ang = -90.0, glob_ang = 0.0;
float position[3] = {8.0, 0.0, 0.0};

/*-----Define window size----*/
int width = 512, height = 512;

// Global variables recording rotation angles of H-arm and blades.
float arm_ang = 100.0, bld_ang = 10.0;

/*-----Translation and rotations of eye coordinate system---*/
float eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double Eye[3] = {0.0, 0.0, 30.0}, Focus[3] = {0.0, 0.0, 0.0},
       Vup[3] = {0.0, 1.0, 0.0};

float u[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

int show_case = show_All;

float eye[3] = {10.0, 5.0, 15.0};
float delta[3] = {0.0, 0.0, 0.0};
float theta[3] = {0.0, 0.0, 0.0};
float zoom = 2.5;

float cv, sv; /* cos(5.0) and sin(5.0) */

/*-----Drawing stye
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int style = 0;

void draw_scene(void);
float norm2(float v[]);

void draw_view_volumn()
{
  float far_plane_length = (50.0 * zoom) / 2.5;
  float zoom_points[][3] = {{-zoom, -zoom, -2.5}, {zoom, -zoom, -2.5}, {zoom, zoom, -2.5}, {-zoom, zoom, -2.5}, {-far_plane_length, -far_plane_length, -50.0}, {far_plane_length, -far_plane_length, -50.0}, {far_plane_length, far_plane_length, -50.0}, {-far_plane_length, far_plane_length, -50.0}};
  int zoom_face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(1);
  glColor3f(0.2, 0.2, 0.2);
  glPushMatrix();

  glTranslated(eye[0], eye[1], eye[2]);
  glTranslated(-delta[0], -delta[1], -delta[2]);
  glRotatef(theta[0], -1.0, 0.0, 0.0);
  glRotatef(theta[1], 0.0, -1.0, 0.0);
  glRotatef(theta[2], 0.0, 0.0, 1.0);

  for (i = 0; i < 6; i++)
  {
    glBegin(GL_POLYGON); /* Draw the face */
    glVertex3fv(zoom_points[zoom_face[i][0]]);
    glVertex3fv(zoom_points[zoom_face[i][1]]);
    glVertex3fv(zoom_points[zoom_face[i][2]]);
    glVertex3fv(zoom_points[zoom_face[i][3]]);
    glEnd();
  }
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void my_view(int P_style)
{
  // glMatrixMode(GL_MODELVIEW);
  switch (P_style)
  {
  case Orthographical_x:
    gluLookAt(15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
    draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  case Orthographical_y:
    gluLookAt(0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0); // WHY?
    draw_view_volumn();                                      //轉到對的位子後要先畫出來
    break;
  case Orthographical_z:
    gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
    draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  case Perspective:
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, -1.0);
    glTranslated(delta[0], delta[1], delta[2]);
    gluLookAt(eye[0], eye[1], eye[2], 10.0, 5.0, -10.0, 0.0, 1.0, 0.0);
    ////我寫到這
    // draw_view_volumn(); //轉到對的位子後要先畫出來
    break;
  }
}
/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
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

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
  int i;

  // glColor3f(0.20, 0.75, 0.0); /* Set the current color */
  for (i = 0; i < 6; i++) //六個面
  {
    glBegin(GL_POLYGON); /* Draw the face */
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

/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */

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

void draw_wheel()
{

  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(-2.0, -2.0, 5.0);
  glRotatef(-40, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 1.0, 0.0);
  if (cylind == NULL)
  { /* allocate a quadric object, if necessary */
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }

  /*--- Draw a cylinder ---*/
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
              10,               /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);

  glPopMatrix();
  glPushMatrix();
  glColor3f(0.0, 0.0, 1.0);
  glTranslatef(-7.0, -2.0, 5.0);
  glRotatef(250, 1.0, 0.0, 0.0);
  glRotatef(15, 0.0, 1.0, 0.0);

  if (cylind == NULL)
  { /* allocate a quadric object, if necessary */
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }

  /*--- Draw a cylinder ---*/
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
              5,                /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
              3);
  glPopMatrix();
}

void make_view(int x)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (x)
  {
  case 4: /* Perspective */

    /* In this sample program, eye position and Xe, Ye, Ze are computed
       by ourselves. Therefore, use them directly; no trabsform is
       applied upon eye coordinate system
       */
    gluLookAt(eye[0], eye[1], eye[2],
              eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
              u[1][0], u[1][1], u[1][2]);
    break;

  case 1: /* X direction parallel viewing */
    gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    break;
  case 2: /* Y direction parallel viewing */
    gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    break;
  case 3:
    gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    break;
  }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (x == Perspective)
  {
    glFrustum(-zoom, zoom, -zoom, zoom, 2.5, 50.0); //(near左邊, near右邊, near下面, near上面, near距離, far距離)
    // gluPerspective(90.0, (double) width/ (double) height, 1.5, 50.0);
  }
  else
  {
    if (width > height)
      glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
              40.0 * (float)height / (float)width,
              -0.0, 100.0);
    else
      glOrtho(-40.0 * (float)width / (float)height,
              40.0 * (float)width / (float)height, -40.0, 40.0,
              -0.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
}

/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_view()
{
  int i;

  glMatrixMode(GL_MODELVIEW);

  /*----Draw Eye position-----*/
  glPushMatrix();
  glTranslatef(eye[0], eye[1], eye[2]);
  glColor3f(0.0, 1.0, 0.0);
  glutWireSphere(1.0, 10, 10);
  glPopMatrix();

  /*----Draw eye coord. axes -----*/
  glColor3f(1.0, 1.0, 0.0); /* Draw Xe */
  glBegin(GL_LINES);
  glVertex3f(eye[0], eye[1], eye[2]);
  glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
  glEnd();

  glColor3f(1.0, 0.0, 1.0); /* Draw Ye */
  glBegin(GL_LINES);
  glVertex3f(eye[0], eye[1], eye[2]);
  glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
  glEnd();

  glColor3f(0.0, 1.0, 1.0); /* Draw Ze */
  glBegin(GL_LINES);
  glVertex3f(eye[0], eye[1], eye[2]);
  glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
  glEnd();
}

void draw_scene()
{
  draw_floor();
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

  /*-------------------------------------------------------*/
  //右大腿

  // glLoadIdentity(); /* Initialize modelview matrix */
  glTranslatef(5.0, 0.0, 5.0);

  glPushMatrix();
  //

  glTranslatef(curDistanceX, 0.0, curDistanceZ); //前後
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);    //轉向

  glTranslatef(0.375, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(RoateBigleg, 1.0, 0.0, 0.0); //蹲下時大腿後傾
    // glTranslatef(0.0, -0.2, 0.0);
  }
  if (animate)
  {
    glRotatef((GLfloat)swingRight, 0.0, 0.0, 1.0); //擺動
  }

  glRotatef((GLfloat)swingRight, 1.0, 0.0, 0.0); //擺動

  glTranslatef(0.0, -0.5, 0.0);
  glColor3f((GLfloat)0 / 255, (GLfloat)255 / 255, (GLfloat)255 / 255);

  glPushMatrix();
  glScalef(0.7, 1.0, 1.0);
  draw_cube();
  glPopMatrix();

  //右小腿
  glTranslatef(0.0, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roateleg, 1.0, 0.0, 0.0); //蹲下時小腿前傾
  }
  glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0); //跨步角度

  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)153 / 255, 1.0);

  // glColor3f(1.0, 0.0, 0.0);

  glPushMatrix();
  glScalef(0.5, 1.0, 0.7);
  draw_cube();

  //右腳掌
  glPopMatrix();

  glTranslatef(0.0, -0.5, -0.1);
  glColor3f(1.0, 1.0, 1.0);

  // glColor3f(1.0, 0.0, 0.0);

  glPushMatrix();
  glScalef(0.7, 0.1, 1.0);
  draw_cube();
  glPopMatrix(); // pop to 右小腿

  glPopMatrix(); // pop to 右大腿

  //左大腿
  glPushMatrix();

  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(-0.375, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(RoateBigleg, 1.0, 0.0, 0.0); //蹲下時大腿後傾
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

  //左小腿
  glTranslatef(0.0, 0.0, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roateleg, 1.0, 0.0, 0.0); //蹲下時小腿前傾
  }
  glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)153 / 255, 1.0);
  glPushMatrix();
  glScalef(0.5, 1.0, 0.7);

  draw_cube();
  glPopMatrix();

  //左腳
  glTranslatef(0.0, -0.5, -0.1);
  glColor3f(1.0, 1.0, 1.0);

  glPushMatrix();
  // glScalef(0.5, 0.1, 0.7);
  glScalef(0.7, 0.1, 1.0);
  draw_cube();

  glPopMatrix(); // pop to 左小腿
  glPopMatrix(); // pop to 左大腿

  // /*-------Draw the body of the robot which is a cube----*/
  //身體
  glPushMatrix();
  glTranslatef(curDistanceX, 0.9, curDistanceZ);

  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);
  if (running)
  {
    glRotatef(-30, 1.0, 0.0, 0.0); //跑步時身體前傾
  }
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0); //蹲下時身體前傾
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

  if (cylind == NULL) //背後電風扇
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

  //頭
  glTranslatef(0.0, 2.5, 0.0);
  glRotatef((GLfloat)head_swing, 0.0, 1.0, 0.0);
  glRotatef((GLfloat)head_nod, 1.0, 0.0, 0.0);
  glColor3f(0.9, 0.9, 0.9);
  glPushMatrix();
  glScalef(3.0, 2.0, 2.5);
  draw_cube();
  glPopMatrix();

  //左眼
  glTranslatef(0.5, 0.0, -1.2);
  glColor3f(0.2, 0.2, 0.2);
  // glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.5, 0.5, 0.5);
  draw_cube();
  glPopMatrix();

  //右眼
  glTranslatef(-1.0, 0.0, 0.0);
  glColor3f(0.2, 0.2, 0.2);
  // glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.5, wink, 0.5);
  draw_cube();
  glPopMatrix();

  /*-------Draw the ear of the robot which is a cone----*/
  //耳朵
  // glTranslatef(curDistanceX, 0.9, curDistanceZ);
  // glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);
  glTranslatef(-0.3, 1.0, 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glColor3f(0.0, (GLfloat)102 / 255, (GLfloat)204 / 255);
  glScalef(0.65, 0.65, 0.65);
  draw_cone();
  glTranslatef(2.35, 0.0, 0.0);
  draw_cone();

  /* Divide it into 3 sections */
  // pop to 身體
  // glRotatef(90.0, 1.0, 0.0, 0.0);

  glPopMatrix();

  //右手
  glPushMatrix();
  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(1.1, 1.75, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0); //蹲下時身體前傾
  }
  glRotatef((GLfloat)swingLeft, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -0.5, 0.0);
  glColor3f(0.0, (GLfloat)102 / 255, (GLfloat)153 / 255);
  glPushMatrix();
  glScalef(0.3, 1.6, 0.4);
  draw_cube();
  glPopMatrix();
  glPopMatrix();

  //右手
  glPushMatrix();
  glTranslatef(curDistanceX, 0.0, curDistanceZ);
  glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

  glTranslatef(-1.1, 1.75, 0.0);
  if (iskneeldown)
  {
    glRotatef(Roatebody, 1.0, 0.0, 0.0); //蹲下時身體前傾
  }
  if (animate)
  {
    glRotatef(200, 0.0, 0.0, 1.0);                 //擺動
    glRotatef((GLfloat)swingRight, 0.0, 0.0, 1.0); //擺動
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
  // glutSwapBuffers();
  // glFlush(); /*--Display the results----*/
}
void display()
{
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  switch (show_case)
  {
  case Orthographical_x:
    make_projection(Orthographical_x);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_x); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();
    break;
  case Orthographical_y:
    make_projection(Orthographical_y);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_y); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();
    break;
  case Orthographical_z:
    make_projection(Orthographical_z);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_z); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();
    break;
  case Perspective:
    make_projection(Perspective);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();     /* Initialize modelview matrix */
    my_view(Perspective); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();
    break;
  case show_All:
    make_projection(Perspective);
    glViewport(width / 2, 0, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();     /* Initialize modelview matrix */
    my_view(Perspective); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();

    make_projection(Orthographical_x);
    glViewport(0, height / 2, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_x); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();

    make_projection(Orthographical_y);
    glViewport(width / 2, height / 2, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_y); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();

    make_projection(Orthographical_z);
    glViewport(0, 0, width / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();          /* Initialize modelview matrix */
    my_view(Orthographical_z); //<<<<<<<<<<<<<<<<<<<<<<
    draw_scene();
    break;
  }
  glFlush(); /*--Display the results----*/

  // /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  // return;
}

void my_reshape(int w, int h)
{
  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w > h)
    glOrtho(-15.0, 15.0, -15.0 * (float)h / (float)w, 15.0 * (float)h / (float)w,
            -10.5, 30.0);
  else
    glOrtho(-15.0 * (float)w / (float)h, 15.0 * (float)w / (float)h, -15.0, 15.0,
            -10.5, 30.0);
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */

//停止
void stop()
{
  running = 0;
  swingLeft = 0;
  swingRight = 0;
  isSwingForward = 0;
  legDis = 0;
}
/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */

void timer(int value)
{
  mo_angle += 0.02;

  //扇葉轉動(加在timer可以走路時也轉動)
  bld_ang += 5.0;
  if (bld_ang > 360.0)
  {
    bld_ang -= 360.0;
    arm_ang += 3.0;
    if (arm_ang > 360.0)
      arm_ang -= 360.0;
  }
  //動作
  int tempTurn = curTurn;
  curDistanceX = curDistanceX - speed * sin((GLfloat)tempTurn / 360 * PI * 2); //多乘速度
  curDistanceZ = curDistanceZ - speed * cos((GLfloat)tempTurn / 360 * PI * 2);
  if (!isSwingForward) //往後擺動
  {
    swingLeft = (swingLeft + stepDis);   //右手往前(+步伐)
    swingRight = (swingRight - stepDis); //右手往後
    if (swingLeft > 0)                   //左右手一前一後
    {
      legDis = legDis - stepDis;
    }
    else
    {
      legDis = legDis + stepDis;
    }
  }
  else //往前擺動
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
void idle_func()
{
  // bld_ang += 5.0;
  mo_angle += 0.02;
  // if (bld_ang > 360.0)
  // {
  //   bld_ang -= 360.0;
  //   arm_ang += 3.0;
  //   if (arm_ang > 360.0)
  //     arm_ang -= 360.0;
  // }

  display(); /* show the scene again */
}

void my_special_func(GLint key, GLint x, GLint y)
{
  if (key == GLUT_KEY_DOWN)
  {
    isjump = 0;
  }
  else if (key == GLUT_KEY_UP)
  {
    printf("jump");
    isjump = 1;
  }
}

void my_keyboard(unsigned char key, int ix, int iy)
{
  int i;
  float x[3], y[3], z[3];
  if (key == 'Q' || key == 'q')
    exit(0);
  // if (key == 'r')
  // {
  //   arm_ang += 5.0;
  //   if (arm_ang > 360.0)
  //     arm_ang -= 360.0;
  // }
  if (key == 'a') //加速
  {
    bld_ang += 10.0;
    if (bld_ang > 360.0)
      bld_ang -= 360.0;
    speed = 0.07;
  }
  else if (key == 'z') // counter-clockwise
  {
    curTurn = (curTurn - 5) % 360;
    // glutPostRedisplay(); //在圖像繪製的所有操作之後，要加入 glutPostRedisplay() 來重繪圖像
  }
  else if (key == 'x') // clowise
  {
    curTurn = (curTurn + 5) % 360;
    // glutPostRedisplay();
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
  else if (key == '9')
  {
    stop();
    if (wink >= 0.1)
    {
      wink -= 0.01;
    }
  }
  else if (key == '8')
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
  else if (key == '7')
  {
    head_swing = (head_swing - 5) % 360;
  }
  else if (key == '6')
  {
    // head_forward = 1;
    animate = 1;
    //點頭
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

    //揮手
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
  switch (key)
  {
  case 'u':
    delta[0] += Step;
    break;
  case 'j':
    delta[0] -= Step;
    break;
  case 'i':
    delta[1] -= Step;
    break;
  case 'k':
    delta[1] += Step;
    break;
  case 'o':
    delta[2] -= Step;
    break;
  case 'l':
    delta[2] += Step;
    break;
  case 'U':
    theta[0] += ANGLE;
    break;
  case 'J':
    theta[0] -= ANGLE;
    break;
  case 'I':
    theta[1] += ANGLE;
    break;
  case 'K':
    theta[1] -= ANGLE;
    break;
  case 'O':
    theta[2] -= ANGLE;
    break;
  case 'L':
    theta[2] += ANGLE;
    break;
  case '1':
    show_case = Orthographical_x;
    break;
  case '2':
    show_case = Orthographical_y;
    break;
  case '3':
    show_case = Orthographical_z;
    break;
  case '4':
    show_case = Perspective;
    break;
  case '5':
    show_case = show_All;
    break;
  }
  if (key == 'b')
  {
    zoom += 0.5;
  }
  else if (key == 'm')
  {
    zoom -= 0.5;
  }

  display();
}
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0); /*set the background color BLACK */
                                    /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

  // glViewport(0, 0, width, height);

  /*-----Set a parallel projection mode-----*/
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // glOrtho(-15.0, 15.0 , -15.0, 15.0, -10.5, 30.0);

  glEnable(GL_DEPTH_TEST); /*Enable depth buffer for shading computing */

  glFlush(); /*Enforce window system display the results*/
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
  glutSpecialFunc(my_special_func);
  glutIdleFunc(idle_func); /* Add idle event */
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_keyboard);

  glutMainLoop();
}
/*------------------------------------------
 * Procedure to compute norm of vector v[]
 */
float norm2(float v[])
{
  double sum;
  float temp;

  sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  temp = (float)sqrt(sum);
  return temp;
}
