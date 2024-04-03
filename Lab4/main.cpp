/**********************************
  
  The reader of this program should not limit
  himself/herself to the comments of this 
  program.

  If one wants to read more about the syntax and 
  the semantics of OpenGL functions used in this
  program, one should read the beginning of the
  paragraph 2.6 "Begin/End Paradigm", the 
  paragraph 2.6.1 "Begin and End Objects" from the 
  file glspec15.pdf at page 25/333 and the index
  from the end of that file. One could also
  read the references to the GLUT functions from
  the file glut-3.spec.pdf.
  


  H O W  T O  R E A D  T H I S  P R O G R A M ?
  
  Start from the function "main" and follow the 
  instruction flow, paying attention to the fact that
  this program belongs to the event-driven programming
  paradigm. So pay attention to what happens when 
  the user presses a key, moves the mouse or presses a
  mouse button. There are also some special events: the
  redrawing of the application window, etc.
  Identify what happens in the program when one of these
  events occurs.

  **********************************/


/**********************************
  With respect to the include-file directives, see the 
  example program from homework 1; in the following it
  was made the assumption that you are using GLUT locally,
  in your project and you didn't install it as an
  Administrator
  **********************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <cfloat>
#include <bits/stdc++.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include "glut.h"

// the size of the window measured in pixels
#define dim 300

using namespace std;

unsigned char prevKey;

class CartesianGrid {
    int rows;
    int columns;
    double scale;
    double offset=0.90;
    public:
    CartesianGrid(int rows, int columns)
    {
        this->rows=rows;
        this->columns=columns;
        this->scale=std::max(this->rows, this->columns)/1.9;
    }

    void writePixel(int i, int j) {
        double idisp=i/this->scale-this->offset;
        double jdisp=j/this->scale-this->offset;
        glVertex2f(idisp, jdisp);
    }

    void displayGrid() {
        glColor3f(0.0, 0.0, 0.0);
        //glPointSize(7.0);
        //glBegin(GL_POINTS);
        for (int i = 0; i < columns; i++) 
        {
            for (int j = 0; j < rows; j++) 
            {
                //writePixel(i, j);
                this->drawCirclePixel(i,j);
            }
        }
        //glEnd();
    }

    void writeLine(int i, int j)
    {
        double idisp=i/this->scale-this->offset;
        double jdisp=j/this->scale-this->offset;
        glVertex2f(idisp, jdisp);
    }
    
    void displayLines()
    {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        for (int i=0;i<columns;i++)
        {
            writeLine(i, 0);
            writeLine(i,rows-1);
        }
        glEnd();
        glBegin(GL_LINES);
        for (int j=0;j<rows;j++)
        {
            writeLine(0, j);
            writeLine(columns-1,j);
        }
        glEnd();
    }

    void drawCirclePixel(int a, int b) {
    if (min(a,b)<0||max(a,b)>min(this->columns,this->rows)-1)
        return;
    float raza = 0.1;
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_POLYGON);
    //din curs
    for (double t=0;t<=M_PI*2;t+=0.01)
    {
        float x = raza*cos(t);
        float y = raza*sin(t);
        x+=a;
        y+=b;
        x=x/this->scale-this->offset;
        y=y/this->scale-this->offset;
        glVertex2f(x, y);
    }
    glEnd();
    }

    void drawQuarterCircle(int a, int b, int R) {
    glColor3f(1.0, 0.0, 0.0);
    //glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_LINE_STRIP);
    //din curs
    for (double t=0;t<=M_PI/2;t+=0.01)
    {
        float x = R*cos(t);
        float y = R*sin(t);
        x+=a;
        y+=b;
        x=x/this->scale-this->offset;
        y=y/this->scale-this->offset;
        for (double x0=x-0.01;x0<=x+0.01;x0+=0.005)
            for (double y0=y-0.01;y0<=y+0.01;y0+=0.005)
            {
                glVertex2f(x0,y0);
            }
        //glVertex2f(x, y);
    }
    glEnd();
    }

    void drawFullCircleSupport(int a, int b, int R) {
    glColor3f(1.0, 0.0, 0.0);
    //glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_LINE_STRIP);
    //din curs
    for (double t=0;t<=2*M_PI;t+=0.01)
    {
        float x = R*cos(t);
        float y = R*sin(t);
        x+=a;
        y+=b;
        x=x/this->scale-this->offset;
        y=y/this->scale-this->offset;
        for (double x0=x-0.01;x0<=x+0.01;x0+=0.005)
            for (double y0=y-0.01;y0<=y+0.01;y0+=0.005)
            {
                glVertex2f(x0,y0);
            }
        //glVertex2f(x, y);
    }
    glEnd();
    }

    void drawLine3(int x0,int y0, int xn, int yn)
    {
        if (x0>xn)
        {
            swap(x0,xn);
            swap(y0,yn);
        }
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        writeLine(x0,y0);
        writeLine(xn,yn);
        glEnd();
        //m>0
        if (yn>=y0)
        {
            int dy=yn-y0;
            int dx=xn-x0;
            int x=x0;
            int y=y0;
            int k=dx*y-dy*x;
            this->drawCirclePixel(x0,y0);
            this->drawCirclePixel(x0,y0-1);
            //m<1
            if (dy<=dx)
            {
                //cout<<"YEY1\n";
                //glBegin(GL_POINTS);
                while (x<xn)
                {
                    int val=dx*(2*y+1)-dy*(2*x+2)-2*k;
                    x++;
                    if (val<0)
                        y++;
                    this->drawCirclePixel(x,y);
                    this->drawCirclePixel(x,y+1);
                    this->drawCirclePixel(x,y-1);
                }
                //glEnd();
            }
            else
            {
                //cout<<"YEY2\n";
                while (y<yn)
                {
                    int val=dx*(2*y+2)-dy*(2*x+1)-2*k;
                    y++;
                    if (val>=0)
                        x++;
                    this->drawCirclePixel(x,y);
                    this->drawCirclePixel(x,y+1);
                    this->drawCirclePixel(x,y-1);
                }
            }
            this->drawCirclePixel(xn,yn);
            this->drawCirclePixel(xn,yn+1);
            this->drawCirclePixel(xn,yn-1);
        }
        else
        {
            int dy=yn-y0;
            int dx=xn-x0;
            int x=x0;
            int y=y0;
            int k=dx*y-dy*x;
            this->drawCirclePixel(x0,y0);
            this->drawCirclePixel(x0,y0-1);
            if (abs(dy)<=abs(dx))
            {
                //cout<<"YEY3\n";
                while (x<xn)
                {
                    int val=dx*(2*y-1)-dy*(2*x+2)-2*k;
                    //cout<<val<<" ";
                    x++;
                    if (val>0)
                        y--;
                    //cout<<x<<" "<<y<<endl;
                    this->drawCirclePixel(x,y);
                    this->drawCirclePixel(x,y-1);
                    this->drawCirclePixel(x,y+1);
                }
            }
            else
            {
                //cout<<"YEY4\n";
                while (y>yn)
                {
                    int val=dx*(2*y-2)-dy*(2*x+1)-2*k;
                    //cout<<val<<" ";
                    y--;
                    if (val>0)
                        x++;
                    //cout<<x<<" "<<y<<endl;
                    this->drawCirclePixel(x,y);
                    this->drawCirclePixel(x,y-1);
                    this->drawCirclePixel(x,y+1);
                }
            }
            this->drawCirclePixel(xn,yn);
            this->drawCirclePixel(xn,yn-1);
        }
    }

    void displayPointsCircle4(int a, int b, int x, int y)
    {
        //this->drawCirclePixel(x-1,y);
        //this->drawCirclePixel(x,y);
        //this->drawCirclePixel(x+1,y);
        for (auto cx:{-1,1})
            for (auto cy:{-1,1})
                for (auto px:{-1,0,1})
                    for (auto py:{-1,0,1})
            {
                //this->drawCirclePixel(a+x*cx-1,b+y*cy);
                this->drawCirclePixel(a+x*cx+px,b+y*cy+py);
                //this->drawCirclePixel(a+x*cx+1,b+y*cy);
                //--------------------------------------------
                //this->drawCirclePixel(a+y*cx-1,b+x*cy);
                this->drawCirclePixel(a+y*cx+px,b+x*cy+py);
                //this->drawCirclePixel(a+y*cx+1,b+x*cy);
            }
    }

    void displayPointsCircle3(int x, int y)
    {
        this->drawCirclePixel(x-1,y);
        this->drawCirclePixel(x,y);
        this->drawCirclePixel(x+1,y);
        /*
        for (auto cx:{-1,1})
            for (auto cy:{-1,1})
            {
                this->drawCirclePixel(x*cx-1,y*cy);
                this->drawCirclePixel(x*cx,y*cy);
                this->drawCirclePixel(x*cx+1,y*cy);
                //--------------------------------------------
                this->drawCirclePixel(y*cx-1,x*cy);
                this->drawCirclePixel(y*cx,x*cy);
                this->drawCirclePixel(y*cx+1,x*cy);
            }
        */
    }

    void drawCircle4(int R)
    {
        int x=0, y=R;
        int d=1-R;
        int dE=3, dSE=-2*R+5;
        displayPointsCircle3(x,y);
        while (y>x)
        {
            if (d<0)
            {
                d+=dE;
                dE+=2;
                dSE+=2;
            }
            else
            {
                d+=dSE;
                dE+=2;
                dSE+=4;
                y--;
            }
            x++;
            displayPointsCircle3(x,y);
        }
    }
    void drawCircle5(int a, int b,int R)
    {
        int x=R, y=0;
        int d=1-R;
        int dV=3, dNV=-2*R+5;
        displayPointsCircle3(x+a,y+b);
        while (x>y)
        {
            if (d<0)
            {
                d+=dV;
                dV+=2;
                dNV+=2;
            }
            else
            {
                d+=dNV;
                dV+=2;
                dNV+=4;
                x--;
            }
            y++;
            displayPointsCircle3(x+a,y+b);
        }
    }

    void drawFullCircle(int a, int b,int R)
    {
        int x=R, y=0;
        int d=1-R;
        int dV=3, dNV=-2*R+5;
        displayPointsCircle4(a,b,x,y);
        while (x>y)
        {
            if (d<0)
            {
                d+=dV;
                dV+=2;
                dNV+=2;
            }
            else
            {
                d+=dNV;
                dV+=2;
                dNV+=4;
                x--;
            }
            y++;
            displayPointsCircle4(a,b,x,y);
        }
    }
};

void Display1()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    //cg.drawLine3(0,15,15,10);
    cg.drawLine3(0,0,15,5);
}
void Display2()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(15,5,0,0);
}
void Display3()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(0,0,7,15);
}
void Display4()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(7,15,0,0);
}
void Display5()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(0,15,15,10);
    //cg.drawLine3(0,0,15,7);
}
void Display6()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(15,10,0,15);
}
void Display7()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(0,15,15,5);
}
void Display8()
{
    CartesianGrid cg(16,16);
    //cg.displayGrid();
    cg.displayLines();
    //cg.drawCircle(0,0);
    //cg.drawCircle(0,1);
    //cg.drawCirclePixel(0,0);
    cg.drawLine3(15,5,0,15);
}

void Display0()
{
    CartesianGrid cg(15,15);
    cg.displayLines();
    cg.drawQuarterCircle(0,0,13);
    //cg.drawCircle4(5);
    cg.drawCircle5(0,0,13);
    //cg.drawCircle4(0,0,1);
}

void Display9()
{
    CartesianGrid cg(15,15);
    cg.displayLines();
    cg.drawFullCircleSupport(7,7,5);
    //cg.drawCircle4(5);
    cg.drawFullCircle(7,7,5);
    //cg.drawCircle4(0,0,1);
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '0':
      Display0();
      break;
    case '1':
      Display1();
      break;
    case '2':
      Display2();
      break;
    case '3':
      Display3();
      break;
    case '4':
      Display4();
      break;
    case '5':
      Display5();
      break;
    case '6':
      Display6();
      break;
    case '7':
      Display7();
      break;
    case '8':
      Display8();
      break;
    case '9':
      Display9();
      break;
   default:
      break;
   }
   glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}
