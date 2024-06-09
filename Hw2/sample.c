#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
//動作
#define STOP 1
#define WALK 2
#define RUN 3
// line mode
#define FILL 1
#define LINE 2
GLfloat specular[] = {1.0f, 0.8f, 0.4f, 0.5f};
GLfloat specref[] = {1.0f, 1.0f, 0.3f, 0.1f};
GLfloat diffuse[] = {0.6f, 0.8f, 0.2f, 0.5f};

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

/*------------------------------------------------------------
 * Procedure to draw a floor of black-and-white tiles.
 */
void draw_floor()
{
    int i, j;

    if (polygonMode == LINE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
        {
            if ((i + j) % 2 == 0)
                glColor3f(0.9, 0.9, 0.9);
            else
                glColor3f(0.2, 0.2, 0.2);
            // draw_circle();
            glBegin(GL_POLYGON);
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j);
            glEnd();
        }
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex3f(-8.0f, -3.0f, 4.0f);
    glVertex3f(8.0f, -3.0f, 4.0f);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(8.0f, -3.0f, -4.0f);
    glVertex3f(-8.0f, -3.0f, -4.0f);
    glEnd();

    //背景
    glBegin(GL_QUADS);
    glColor3ub(33, 33, 33);
    glVertex3f(-8.0f, -3.0f, -4.0f);
    glVertex3f(8.0f, -3.0f, -4.0f);
    glColor3ub(156, 66, 180);
    glVertex3f(8.0f, 8.0f, -4.0f);
    glVertex3f(-8.0f, 8.0f, -4.0f);
    glEnd();

    //左大腿
    glPushMatrix();

    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

    glTranslatef(0.375, 0.0, 0.0);

    glRotatef((GLfloat)swingRight, 1.0, 0.0, 0.0);

    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    // glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(0.4, 1.0, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();

    //左小腿
    glTranslatef(0.0, -0.5, 0.0);

    glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0);

    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.1, 0.5, 0.5);
    // glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(0.3, 1.0, 0.3);
    glutSolidCube(1.0);

    //左腳
    glPopMatrix();

    glTranslatef(0.0, -0.5, -0.1);
    glColor3f(0.8, 0.8, 0.2);
    // glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(0.5, 0.1, 0.7);
    glutSolidCube(1.0);
    glPopMatrix(); // pop to 左小腿

    glPopMatrix(); // pop to 左大腿

    //右大腿
    glPushMatrix();

    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

    glTranslatef(-0.375, 0.0, 0.0);
    glRotatef((GLfloat)swingLeft, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.4, 1.0, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();

    //右小腿
    glTranslatef(0.0, -0.5, 0.0);
    glRotatef((GLfloat)legDis, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.1, 0.5, 0.5);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 1.0, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    //右腳
    glTranslatef(0.0, -0.5, -0.1);
    glColor3f(0.8, 0.8, 0.2);
    // glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(0.5, 0.1, 0.7);
    glutSolidCube(1.0);

    glPopMatrix(); // pop to 右小腿
    glPopMatrix(); // pop to 右大腿

    //身體
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 1.0, 0.0);
    glColor3f(1.0, 0.7, 0.1);
    // glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(1.4, 2.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    //頭
    glTranslatef(0.0, 1.25, 0.0);
    glColor3f(0.9, 0.9, 0.9);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    //左眼
    glTranslatef(0.125, 0.1, -0.25);
    glColor3f(0.2, 0.2, 0.2);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //右眼
    glTranslatef(-0.25, 0.0, 0.0);
    glColor3f(0.2, 0.2, 0.2);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //嘴巴
    // glTranslatef(0.125, -0.25, 0.0);
    // // glColor3f(0.8, 0.2, 0.2);
    // glColor3f(1.0, 0.0, 0.0);
    // glPushMatrix();
    // glScalef(0.3, 0.1, 0.1);
    // glutSolidCube(1.0);
    // glPopMatrix();

    // pop to 身體
    glPopMatrix();

    //左手xx
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

    glTranslatef(0.85, 1.75, 0.0);
    glRotatef((GLfloat)swingLeft, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.8, 0.4, 0.5);
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 1.6, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    //右手
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)curTurn, 0.0, 1.0, 0.0);

    glTranslatef(-0.85, 1.75, 0.0);
    glRotatef((GLfloat)swingRight, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glColor3f(0.8, 0.4, 0.5);
    // glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 1.6, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    curw = w;
    curh = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -8.0);
}

void keyEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
        curTurn = (curTurn - 5) % 360;
        glutPostRedisplay();
        break;
    case 'z':
        curTurn = (curTurn + 5) % 360;
        glutPostRedisplay();
        break;
    case 'q':
        exit(0);
        break;
    case 1:
        polygonMode = FILL;
        break;
    case 2:
        polygonMode = LINE;
        break;
    default:
        break;
    }
}

void timer(int value)
{
    int tempTurn = curTurn;
    curDistanceX = curDistanceX - speed * sin((GLfloat)tempTurn / 360 * 3.14 * 2);
    curDistanceZ = curDistanceZ - speed * cos((GLfloat)tempTurn / 360 * 3.14 * 2);
    if (!isSwingForward)
    {
        swingLeft = (swingLeft + stepDis);
        swingRight = (swingRight - stepDis);
        if (swingLeft > 0)
        {
            legDis = legDis - stepDis * 1.2;
        }
        else
        {
            legDis = legDis + stepDis * 1.2;
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
    display();
    glutPostRedisplay();
    if (!isStand)
        glutTimerFunc(value, timer, value);
}

//停止
void stop()
{
    swingLeft = 0;
    swingRight = 0;
    isSwingForward = 0;
    legDis = 0;
}

void MenuFunc(int data)
{
    switch (data)
    {
    case 1:
        isStand = 1;
        stop();
        break;
    case 2:
        stop();
        maxAngel = 20;
        stepDis = 1;
        speed = 0.02;

        if (isStand)
            glutTimerFunc(20, timer, 20);
        isStand = 0;
        break;
    case 3:
        stop();
        maxAngel = 55;
        stepDis = 5;
        speed = 0.04;

        if (isStand)
            glutTimerFunc(10, timer, 10);
        isStand = 0;
        break;
    }
}

void init(void)
{

    glEnable(GL_CULL_FACE);
    // glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, diffuse);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0f);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 8);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutCreateMenu(MenuFunc);
    glutAddMenuEntry("stop", STOP);
    glutAddMenuEntry("walking", WALK);
    glutAddMenuEntry("running", RUN);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("robot");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyEvent);

    glutMainLoop();
    return 0;
}