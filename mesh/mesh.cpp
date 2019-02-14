//
// Created by 田一菲 on 2019/2/11.
//

#include <stdlib.h>
//#include <GL/glut.h>
#include <GLUT/glut.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4

GLfloat mat[4];
GLfloat position[4];
GLfloat angle = 0;
bool isRun = false;
bool changeCol = false;
GLfloat change[3];
GLfloat tempCol[3];
bool first = true;
bool mouseLeftDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float zoom = 1;
/*
 * Initialize depth buffer, projection matrix, light source, and lighting
 * model.  Do not specify a material property here.
 */

struct Point
{
    double x;
    double y;
    double z;
};

struct Face
{
    int v[3];
};

vector<Point>points;
vector<Face>faces;
GLfloat preCol[3];
GLfloat nextCol[3];//要变的颜色
int id;

void myIdle();
void changeIdle();
void mouseCB(int button, int state, int x, int y);
void mouseMotionCB(int x, int y);

GLuint createList()//创建显示列表
{
    id = glGenLists(1);
    if (!id)
        return id;
    glNewList(id, GL_COMPILE);
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < faces.size(); i++)
    {
        //画出来太小了··乘了个10
        //glBegin(GL_TRIANGLES);
        glVertex3d(points[faces[i].v[0] - 1].x * 10, points[faces[i].v[0] - 1].y * 10, points[faces[i].v[0] - 1].z * 10);
        glVertex3d(points[faces[i].v[1] - 1].x * 10, points[faces[i].v[1] - 1].y * 10, points[faces[i].v[1] - 1].z * 10);
        glVertex3d(points[faces[i].v[2] - 1].x * 10, points[faces[i].v[2] - 1].y * 10, points[faces[i].v[2] - 1].z * 10);
        // glEnd();
    }

    glEnd();
    glEndList();
}

void readFile()
{
    ifstream fin("bunny_1k.obj");
    if (!fin.is_open()){
        return;
    }
    string s;
    Face *f;
    Point *p;
    while (getline(fin, s)){
        if (s.length() < 2)
            continue;
        if (s[0] == 'v'){
            istringstream iss(s);
            p = new Point();
            string head;
            iss >> head >> p->x >> p->y >> p->z;
            points.push_back(*p);
        }
        else if (s[0] == 'f'){
            istringstream iss(s);
            f = new Face();
            string head;
            iss >> head >> f->v[0] >> f->v[1] >> f->v[2];
            faces.push_back(*f);
        }
    }
}

void init(void)
{
    GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    //光源位置
    //GLfloat position = {1.0, 1.0, 5.0, 0.0};
    position[0] = 1.0;
    position[1] = 1.0;
    position[2] = 5.0;
    position[3] = 0.0;

    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat local_view[] = { 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    mat[0] = 0.9; mat[1] = 0.0; mat[2] = 0.0;

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

}


void display(void)
{
    //GLfloat mat[4];
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(4, 8, 0.0);
    //teapot的颜色
    //   mat[0] = 0.9; mat[1] = 0.0; mat[2] = 0.0;
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    if (changeCol){
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    }

    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glutSolidTeapot(3.0);

    glPushMatrix();
    glTranslatef(8, 0, 0.0);

    GLfloat col[3] = { 1, 1, 1 };
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, col);

    //绘制模型
    glCallList(1);
    /*
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < faces.size(); i++)
    {
        //画出来太小了··乘了个10
        //glBegin(GL_TRIANGLES);
        glVertex3d(points[faces[i].v[0] - 1].x * 10, points[faces[i].v[0] - 1].y * 10, points[faces[i].v[0] - 1].z * 10);
        glVertex3d(points[faces[i].v[1] - 1].x * 10, points[faces[i].v[1] - 1].y * 10, points[faces[i].v[1] - 1].z * 10);
        glVertex3d(points[faces[i].v[2] - 1].x * 10, points[faces[i].v[2] - 1].y * 10, points[faces[i].v[2] - 1].z * 10);
        // glEnd();
    }
    glEnd();
    */

    glPopMatrix();

    glPopMatrix();

    if (isRun){
        glTranslatef(6, 8, 0);
        glRotated(angle, 0, 10, 0);
        glTranslated(-6, -8, 0);
    }

    glTranslatef(6, 8, 0);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glScalef(zoom, zoom, zoom);//缩放
    glTranslated(-6, -8, 0);

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(0.0, 16.0, 0.0, 16.0*(GLfloat)h / (GLfloat)w,
                -10.0, 10.0);
    else
        glOrtho(0.0, 16.0*(GLfloat)w / (GLfloat)h, 0.0, 16.0,
                -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawScene()
{
    if (first){
        for (int i = 0; i < 3; i++)
        {
            preCol[i] = mat[i];
            tempCol[i] = mat[i];
        }//记录当前颜色
        //0-1的随机浮点数改变颜色
        srand(unsigned(time(NULL)));
        nextCol[0] = rand() / double(RAND_MAX);
        nextCol[1] = rand() / double(RAND_MAX);
        nextCol[2] = rand() / double(RAND_MAX);
        first = false;
        for (int i = 0; i < 3; i++)
        {
            change[i] = (nextCol[i] - preCol[i]) / 1000;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        mat[i] = preCol[i] + change[i];
        preCol[i] = mat[i];
    }
    if (((mat[0] <= nextCol[0]) && (nextCol[0] < tempCol[0])) || ((mat[0] >= nextCol[0]) && (nextCol[0] > tempCol[0]))){
        first = true;
        changeCol = false;
    }
    /*
    mat[0] = rand() / double(RAND_MAX);
    mat[1] = rand() / double(RAND_MAX);
    mat[2] = rand() / double(RAND_MAX);
    */
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a':
            exit(0);
            cout << "Unhandled key press " << key << "." << endl;
            break;
        case 27:
            exit(0);
            break;
        case 'c'://按C变颜色
            if (!changeCol){
                changeCol = true;
                //drawScene();
                //glutTimerFunc(333, onTimer, 333);
                //glutTimerFunc(33, onTimer, 1);
                glutIdleFunc(changeIdle);
            }
            //else{
            //glutIdleFunc(NULL);
            //}
            break;
        case 'r':
            //按R旋转
            if (!isRun){
                isRun = true;
                glutIdleFunc(myIdle);
            }
            else{
                isRun = false;
                //glutIdleFunc(NULL);
            }
            break;
        default:
            cout << "Unhandled key press " << key << "." << endl;
            break;
    }
    glutPostRedisplay();
}

void specialKey(GLint key, GLint x, GLint y)
{
    //方向键控制光源
    switch (key)
    {
        case GLUT_KEY_UP:
            position[1] += 0.5;
            break;
        case GLUT_KEY_LEFT:
            position[0] += 0.5;
            break;
        case GLUT_KEY_DOWN:
            position[1] -= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            position[0] -= 0.5;
            break;
        default:
            break;
    }
    //cout << position[0] << "  " << position[1] << endl;
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y)//鼠标滚轮缩放 左键旋转
{
    mouseX = x;
    mouseY = y;
    zoom = 1;

    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){
            mouseLeftDown = true;
        }
        else{
            mouseLeftDown = false;
        }
    }

    else if (state == GLUT_UP && button == GLUT_WHEEL_UP){
        zoom = 0.008f + 1;
        glutPostRedisplay();
    }
    else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN){
        zoom = -0.008f + 1;
        glutPostRedisplay();
    }

}

void mouseMotionCB(int x, int y)
{
    cameraAngleX = cameraAngleY = 0;

    if (mouseLeftDown)
    {
        cameraAngleY += (x - mouseX) * 0.5f;
        cameraAngleX += (y - mouseY) * 0.5f;
        mouseX = x;
        mouseY = y;
    }

    glutPostRedisplay();
}

void myIdle()
{
    angle = 0.1;
    glutPostRedisplay();
}

void changeIdle()
{
    if (changeCol){
        drawScene();
        glutPostRedisplay();
        //glutTimerFunc(33, onTimer, 33);
    }
}

/*
 * Main Loop
 */
int main(int argc, char **argv)
{
    readFile();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("hello world");
    init();
    createList();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
    glutMainLoop();
    return 0;
}