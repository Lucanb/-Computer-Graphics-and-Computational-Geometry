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

#include <GL/freeglut.h>

// the size of the window measured in pixels
#define dim 300

unsigned char prevKey;

// the graph of the Conchoid of Nicomedes
void Display1() {
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1.0);
   double ratia = 0.05;
   double t;

   /**********************************
      Maximum/minimum values of x and y are computed.
      These values will be further used in scaling the
      original graph of the curve. 
      These values are stored in the variables
      xmax, ymax, xmin, ymin: e.g., xmax is initialized
      with a value smaller than any of possible values
      of x; if in doubt or you cannot estimate it, use 
      DBL_MIN instead (or DBL_MAX for minimum values).
      These DBL_* constants are found in <float.h> which
      need to be included.
      E.g., xmax is initialized with a - b - 1 because
      x(t) = a +/- b * cos(t) and for t in (-pi/2, pi/2),
      cos(t) is in (0, 1), so b * cos(t) is in (0, b),
      so +/- b * cos(t) is in (-b, b), so x(t) is in
      (a-b, a+b) and one can safely choose a-b-1 because
      a-b-1 < a-b. 
      For y(t) we see that in its expression appears
      tan(t) which varies in (-inf,+inf) when t
      varies in (-pi/2, pi/2).
     **********************************/
   xmax = a - b - 1; 
   xmin = a + b + 1;
   ymax = ymin = 0;
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   /**********************************
      At this line, we have found that the graph of the Conchoid
      is included in the rectangle having the edges x = xmin,
      x = xmax, y = ymin and y = ymax. 
      We would like that the rectangle should be symmetric with
      respect to the Ox and Oy axes.
      We store in xmax and ymax the maximum of absolute values
      max(|xmax|,|xmin|) and max(|ymax|,|ymin|).
      Now we know that the graph of the Conchoid is included in
      the rectangle [-xmax, xmax] x [-ymax, ymax].
     **********************************/

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   /**********************************
      At this line, we can perform the scaling. All the points
      scaled are visible (i.e., in the rectangle [-1,1]x[-1,1]).
      Because we have -xmax <= x <= xmax we get
      -1 <= x / xmax <= 1. Idem for y.
      In order to correctly display the graph of the Conchoid,
      one should use the exact same points that were used in
      the computation of the scaling factor.
     **********************************/

   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
}

// The graph of the function 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
   double pi = 4 * atan(1.0);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;


   /**********************************
      For this example, the computation of the scaling factors
      is not needed. Because x is in the interval [0, 8pi]
      the scaling factor for x is 8pi because x/(8pi) is in
      [0,1]. In the case of the exponential function we know
      that if x is [0,8pi] then sin x is in [-1,1] so
      e^(-sin x) is in [1/e, e] and thus it is safe to use 
      e^(1.1) as a scaling factor.
     **********************************/
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();
}

void Display3() {
  double xmax = 100;
  double ymax = 1.1;
  double ratio = 0.05;

  glColor3f(1, .1, .1);
  glBegin(GL_LINE_STRIP);
  for (double x = 0; x < xmax; x += ratio) {
    double dist = x - floor(x) < ceil(x) - x ? x - floor(x) : ceil(x) - x;
    double y = x == 0 ? 1 : dist / x;
    glVertex2f(x / (xmax / 4), y / ymax);
  }
  glEnd();
}

void Display4() {
  double pi = 4 * atan(1.0);
  double eps = 1e-3;
  double a = 0.2;
  double ratio = 0.01;
  int count = 0;

  glColor3d(1, .1, .1);
  glBegin(GL_TRIANGLES);
  for (double t = -pi / 2 + ratio; t <= pi / 2; t += ratio) {
    if (abs(t - pi / 6) < eps) continue;
    if (abs(t + pi / 6) < eps) continue;
    double x = a / (4 * cos(t) * cos(t) - 3);
    double y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);
    if (x < 0 && y > 0) {
      glVertex2f(x, y);
      if (++count % 2 == 0)
        glVertex2f(-1, 1);
    }
  }
  glEnd();

  glColor3d(.1, .1, .8);
  glBegin(GL_LINE_LOOP);
  for (double t = -pi / 2 + ratio; t <= pi / 2; t += ratio) {
    if (abs(t - pi / 6) < eps) continue;
    if (abs(t + pi / 6) < eps) continue;
    double x = a / (4 * cos(t) * cos(t) - 3);
    double y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);
    if (x < 0 && y > 0)
      glVertex2f(x, y);
  }
  glVertex2f(-1, 1);
  glEnd();
}

void Display5() {
  double a = 0.1;
  double b = 0.2;
  double tmax = 100;
  double ratio = 0.05;

  glColor3f(1, .1, .1);
  glBegin(GL_LINE_STRIP);
  for (double t = -tmax; t < tmax; t += ratio) {
    double x = a * t - b * sin(t);
    double y = a - b * cos(t);
    glVertex2f(x, y);
  }
  glEnd();
}

void Display6() {
  double pi = 4 * atan(1.0);
  double a = 0.4;
  double ratio = 0.001;

  glColor3f(1, .1, .1);
  glBegin(GL_LINE_STRIP);
  for (double t = -pi / 4; t < pi / 4; t += ratio) {
    double r = a * sqrt(2 * cos(2 * t));
    double x1 = r * cos(t);
    double y1 = r * sin(t);
    double x2 = -r * cos(t);
    double y2 = -r * sin(t);
    glVertex2f(x1, y1);
  }
  glEnd();
  
  glBegin(GL_LINE_STRIP);
  for (double t = -pi / 4; t < pi / 4; t += ratio) {
    double r = a * sqrt(2 * cos(2 * t));
    double x1 = r * cos(t);
    double y1 = r * sin(t);
    double x2 = -r * cos(t);
    double y2 = -r * sin(t);
    glVertex2f(x2, y2);
  }
  glEnd();
}

void Display7() {
  double a = 0.02;
  double ratio = 0.05;
  double tmax = 100;

  glColor3f(1, .1, .1);
  glBegin(GL_LINE_STRIP);
  for (double t = ratio; t < tmax; t += ratio) {
    double r = a * exp(1 + t);
    double x = r * cos(t);
    double y = r * sin(t);
    glVertex2f(x, y);
  }
  glEnd();
}

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(1);
  glPolygonMode(GL_FRONT, GL_LINE);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  switch (prevKey) {
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

void MouseFunc(int button, int state, int x, int y) { }

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(dim, dim);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow(argv[0]);
  Init();
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(KeyboardFunc);
  glutMouseFunc(MouseFunc);
  glutDisplayFunc(Display);
  glutMainLoop();
  return 0;
}
