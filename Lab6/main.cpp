#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bits/stdc++.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// the size of the window measured in pixels
#define dim 300

using namespace std;

unsigned char prevKey;

enum EObiect {cubw, cubs, sferaw, sferas, tri} ob = cubw;

int once=0;

struct Point{
    double x;
    double y;
    double z;
} P1,P2,P3,P1p,P2p,P3p,P1pp,P2pp,P3pp,P1ppp,P2ppp,P3ppp,Q,P1p4,P2p4,P3p4;

struct Matrix{
  vector<double> firstrow;
  vector<double> secondrow;
  vector<double> thirdrow;
};

void DisplayTriunghi(Point po1, Point po2, Point po3){
    glBegin(GL_TRIANGLES);
    glVertex3f(po1.x,po1.y,po1.z);
    glVertex3f(po2.x,po2.y,po2.z);
    glVertex3f(po3.x,po3.y,po3.z);
    glEnd();
}

void printPoint(Point p)
{
  cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
}

void DisplayAxe() {
  int X, Y, Z;
  X = Y = 200;
  Z = 200;

  glLineWidth(2);

  // Ox axis - green
  glColor3f(0, 1, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(X,0,0);
  glEnd();

  // Oy axis - blue
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,Y,0);
  glEnd();

  // Oz axis - red
  glColor3f(1, 0, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,0,Z);
  glEnd();

  glLineWidth(1);
}

void DisplayWireframeCube() {
    // red
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINES);

    // jos
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);

    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);

    // sus
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, 1.0);

    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);

    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 1.0, 1.0);

    glVertex3f(0.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 1.0);

    // vertic
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);

    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 1.0);

    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0);

    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 1.0);

    glEnd();
}

void DisplayDiagonal()
{
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 1.0, 1.0);
  glEnd();
}

// wireframe cube
void Display1() {
   glColor3f(1,0,0);
   //glutWireCube(1);
   //input - n grade de rotit
   int n=15;
   glPushMatrix();
   double unghi=atan(1/sqrt(2));
   unghi=unghi*180.0/M_PI;
   cout<<unghi<<endl;
   /*
   glRotated(-45,0,1,0);
   glRotated(unghi,1,0,0);
   glRotated(n,0,0,1);
   glRotated(-unghi,1,0,0);
   glRotated(45,0,1,0);
   */
   glRotated(45,0,1,0);
   glRotated(-unghi,1,0,0);
   glRotated(n,0,0,1);
   glRotated(unghi,1,0,0);
   glRotated(-45,0,1,0);
   DisplayWireframeCube();
   DisplayDiagonal();
   glPopMatrix();
   DisplayAxe();
}

// solid cube
void Display2() {
   glColor3f(1,0,0);
   glutSolidCube(1);
}

// wireframe sphere
void Display3() {
   glColor3f(0,0,1);
   glutWireSphere(1, 10, 10);
}

// solid sphere
void Display4() {
   glColor3f(0,0,1);
   glutSolidSphere(1, 10, 10);
}

void translation(Point &p, double x, double y, double z)
{
  p.x+=x;
  p.y+=y;
  p.z+=z;
}

void assignp(Point &p, Point &p1)
{
  p.x=p1.x;
  p.y=p1.y;
  p.z=p1.z;
}

Point multbymatrix(Point &p, Matrix m)
{
  Point ans;
  ans.x=m.firstrow[0]*p.x+m.firstrow[1]*p.y+m.firstrow[2]*p.z;
  ans.y=m.secondrow[0]*p.x+m.secondrow[1]*p.y+m.secondrow[2]*p.z;
  ans.z=m.thirdrow[0]*p.x+m.thirdrow[1]*p.y+m.thirdrow[2]*p.z;
  //cout<<"ans"<<endl;
  //cout<<ans.x<<" "<<ans.y<<" "<<ans.z<<endl;
  return ans;
}

void calculatePutInyzPlane()
{
    if (once)
      return;
    once=1;
    //pas 1 - translatie cu (-P1)
    assignp(P1p,P1);
    assignp(P2p,P2);
    assignp(P3p,P3);
    translation(P1p,-P1.x,-P1.y,-P1.z);
    translation(P2p,-P1.x,-P1.y,-P1.z);
    translation(P3p,-P1.x,-P1.y,-P1.z);
    printPoint(P1p);
    printPoint(P2p);
    printPoint(P3p);
    //pas 2 - rotatie 
    //calcul Q
    Q.x=P2p.x;
    Q.y=0;
    Q.z=P2p.z;
    //cout<<"Q "<<Q.x<<" "<<Q.z<<endl;
    double d1=sqrt(Q.x*Q.x+Q.z*Q.z);
    //cout<<"d1 "<<d1<<endl;
    /*
    Point V1,V2;
    //Ox=(1,0,0);
    //calcul unghi
    V1.x=Q.x-P1p.x;
    V1.y=Q.y-P1p.y;
    V1.z=Q.z-P1p.z;
    V2.x=1-P1p.x;
    V2.y=0-P1p.y;
    V2.z=0-P1p.z;
    double magn1=0, magn2=0;
    magn1=V1.x*V1.x+V1.y*V1.y+V1.z*V1.z;
    magn2=V2.x*V2.x+V2.y*V2.y+V2.z*V2.z;
    magn1=sqrt(magn1);
    magn2=sqrt(magn2);
    double dot_pr=V1.x*V2.x+V1.y*V2.y+V1.z*V2.z;
    double radians=acos(dot_pr/(magn1*magn2));
    double angle_degrees=radians*180/M_PI;
    */
    //matrice de rotatie in jurul Oy cu (90-angle)
    // cos(a-90) 0 sin(a-90)
    // 0 1 0
    // - sin(a-90) 0 cos(a-90)
    //SAU
    double a=(P2.z-P1.z)/d1;
    double b=-(P2.x-P1.x)/d1;
    Matrix m;
    m.firstrow.assign({a,0,b});
    m.secondrow.assign({0,1,0});
    m.thirdrow.assign({-b,0,a});
    // a 0 b
    // 0 1 0
    // -b 0 a
    //std::cout<<m.firstrow[0]<<" "<<m.firstrow[1]<<" "<<m.firstrow[2]<<endl;
    //assignp(P1pp,P1p);
    //assignp(P2pp,P2p);
    //assignp(P3pp,P3p);
    P1pp=multbymatrix(P1p,m);
    P2pp=multbymatrix(P2p,m);
    P3pp=multbymatrix(P3p,m);
    //std::cout<<"d1 "<<d1<<endl;
    //std::cout<<"P2pp "<<0<<" "<<P2.y-P1.y<<" "<<d1<<endl;
    //printPoint(P2pp);
    printPoint(P1pp);
    printPoint(P2pp);
    printPoint(P3pp);
    /*
    P1pp.x=a*P1p.x+0*P1p.y+b*P1p.z;
    P1pp.y=0*P1p.x+P1p.y+0*P1p.z;
    P1pp.z=-b*P1p.x+0*P1p.y+a*P1p.z;
    P2pp.x=a*P2p.x+0*P2p.y+b*P2p.z;
    P2pp.y=0*P2p.x+P2p.y+0*P2p.z;
    P2pp.z=-b*P2p.x+0*P2p.y+a*P2p.z;
    P3pp.x=a*P3p.x+0*P3p.y+b*P3p.z;
    P3pp.y=0*P3p.x+P3p.y+0*P3p.z;
    P3pp.z=-b*P3p.x+0*P3p.y+a*P3p.z;
    */
    //partea 3
    double d2=sqrt(P2pp.y*P2pp.y+P2pp.z*P2pp.z);
    double na=-(P2.y-P1.y)/d2;
    //matrice
    // 1 0 0
    // 0 d1/d2 na
    // 0 -na d1/d2
    Matrix m2;
    m2.firstrow.assign({1,0,0});
    m2.secondrow.assign({0,d1/d2,na});
    m2.thirdrow.assign({0,-na,d1/d2});
    //assignp(P1ppp,P1pp);
    //assignp(P2ppp,P2pp);
    //assignp(P3ppp,P3pp);
    P1ppp=multbymatrix(P1pp,m2);
    P2ppp=multbymatrix(P2pp,m2);
    P3ppp=multbymatrix(P3pp,m2);
    printPoint(P1ppp);
    printPoint(P2ppp);
    printPoint(P3ppp);
    /*
    P1ppp.x = 1*P1pp.x+0*P1pp.y+0*P1pp.z;
    P1ppp.y = 0*P1pp.x+d1/d2*P1pp.y+na*P1pp.z;
    P1ppp.z=0*P1pp.x-na*P1pp.y+d1/d2*P1pp.z;
    P2ppp.x = 1*P2pp.x+0*P2pp.y+0*P2pp.z;
    P2ppp.y = 0*P2pp.x+d1/d2*P2pp.y+na*P2pp.z;
    P2ppp.z=0*P2pp.x-na*P2pp.y+d1/d2*P2pp.z;
    P3ppp.x = 1*P3pp.x+0*P3pp.y+0*P3pp.z;
    P3ppp.y = 0*P3pp.x+d1/d2*P3pp.y+na*P3pp.z;
    P3ppp.z=0*P3pp.x-na*P3pp.y+d1/d2*P3pp.z;
    */
    //partea 4
    double d3=sqrt(P3ppp.x*P3ppp.x+P3ppp.y*P3ppp.y);
    //Rz(a)=???
    double cosa=P3ppp.y/d3;
    double sina=P3ppp.x/d3;
    Matrix m3;
    m3.firstrow.assign({cosa, -sina,0});
    m3.secondrow.assign({sina,cosa,0});
    m3.thirdrow.assign({0,0,1});
    //Rz:
    // cosa -sina 0
    // sina cosa 0
    // 0 0 1
    P1p4=multbymatrix(P1ppp,m3);
    P2p4=multbymatrix(P2ppp,m3);
    P3p4=multbymatrix(P3ppp,m3);
    /*
    P1p4.x=cosa*P1ppp.x-sina*P1ppp.y+0*P1ppp.z;
    P1p4.y=sina*P1ppp.x+cosa*P1ppp.y+0*P1ppp.z;
    P1p4.z=0*P1ppp.x+0*P1ppp.y+1*P1ppp.z;
    P2p4.x=cosa*P2ppp.x-sina*P2ppp.y+0*P2ppp.z;
    P2p4.y=sina*P2ppp.x+cosa*P2ppp.y+0*P2ppp.z;
    P2p4.z=0*P2ppp.x+0*P2ppp.y+1*P2ppp.z;
    P3p4.x=cosa*P3ppp.x-sina*P3ppp.y+0*P3ppp.z;
    P3p4.y=sina*P3ppp.x+cosa*P3ppp.y+0*P3ppp.z;
    P3p4.z=0*P3ppp.x+0*P3ppp.y+1*P3ppp.z;
    */
   std::cout<<"Points"<<endl;
   printPoint(P1p4);
   printPoint(P2p4);
   printPoint(P3p4);
}

void Display5(){
    cout<<fixed<<setprecision(10);
    glColor3f(0,0,0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 20.0);
    double random_value = distrib(gen);
    //std::cout << "Random double value: " << random_value << std::endl;
    P1.x=distrib(gen);
    P1.y=distrib(gen);
    P1.z=distrib(gen);
    P2.x=distrib(gen);
    P2.y=distrib(gen);
    P2.z=distrib(gen);
    P3.x=distrib(gen);
    P3.y=distrib(gen);
    P3.z=distrib(gen);
    glPushMatrix();
    calculatePutInyzPlane();
    cout<<flush;
    DisplayTriunghi(P1p4,P2p4,P3p4);
    glPopMatrix();
    DisplayAxe();
    //glMatrixMode(GL_MODELVIEW);
}


void DisplayObiect()
{
  switch (ob)
  {
  case cubw:
    Display1();
    break;
  case cubs:
    Display2();
    break;
  case sferaw:
    Display3();
    break;
  case sferas:
    Display4();
    break;
  case tri:
    Display5();
    break;
  default:
    break;
  }
}

// the rotation with 10 degrees about the Ox axis
void DisplayX() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,1,0,0);
}

// the rotation with 10 degrees about the Oy axis
void DisplayY() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,1,0);
}

// the rotation with 10 degrees about the Oz axis
void DisplayZ() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,0,1);
}

// translation by 0.2, 0.2, 0.2
void DisplayT() {
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(-0.2, -0.2, -0.2);
}

// scaling by 1.2, 1.2, 1.2
void DisplayS() {
  glMatrixMode(GL_MODELVIEW);
  glScalef(1.2, 1.2, 1.2);
}

void Init(void) {
  glClearColor(1, 1, 1, 1);
  glLineWidth(2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10, 10, -10, 10, 30, -30);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(20, 1, 0, 0);
  glRotated(-20, 0, 1, 0);
}

void Display(void) {
  switch(prevKey) 
  {
  case 'a':
    DisplayAxe();
    break;
  case '0':
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(20, 1, 0, 0);
    glRotated(-20, 0, 1, 0);
    break;
  case '1':
    Display1();
    ob = cubw;
    break;
  case '2':
    Display2();
    ob = cubs;
    break;
  case '3':
    Display3();
    ob = sferaw;
    break;
  case '4':
    Display4();
    ob = sferas;
    break;
  case '5':
    Display5();
    ob=tri;
    break;
  case 'x':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayX();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'y':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayY();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'z':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayZ();
    DisplayAxe();
    DisplayObiect();
    break;
  case 't':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayT();
    DisplayAxe();
    DisplayObiect();
    break;
  case 's':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayS();
    DisplayAxe();
    DisplayObiect();
    break;
  default:
    break;
  }
  glutSwapBuffers();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27)
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}