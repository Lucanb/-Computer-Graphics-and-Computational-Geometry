#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

unsigned char prevKey;

class CartesianGrid {
  int rows; // nr de linii
  int cols; // nr de coloane
  float xStart; // stanga
  float yStart; // jos
  float padding; // distanta de la grid la margimea ferestrei
  float colDim; // width al unei celule din grid
  float rowDim; // height al unei celule din grid
  float radius; // raza pixelului pe care il desenam la rasterizare

public:
  CartesianGrid(int rows, int cols) : rows(rows), cols(cols) {
    this->padding = 0.1;
    this->xStart = -1 + padding;
    this->yStart = -1 + padding;
    this->colDim = 2 * (1 - padding) / float(cols);
    this->rowDim = 2 * (1 - padding) / float(rows);
    this->radius = std::min(this->rowDim, this->colDim) / 4;
    drawGrid();
  }

  void drawGrid() {
    glColor3f(0, 0, 0);

    float x = this->xStart;
    for (int i = 0; i <= this->cols; i++) {
      glBegin(GL_LINES);
        glVertex2f(x, -1 + this->padding);
        glVertex2f(x, +1 - this->padding);
        x += colDim;
      glEnd();
    }

    float y = this->yStart;
    for (int j = 0; j <= this->rows; j++) {
      glBegin(GL_LINES);
        glVertex2d(-1 + this->padding, y);
        glVertex2d(+1 - this->padding, y);
        y += rowDim;
      glEnd();
    }

    glFlush();
  }

  void writePixel(int i, int j) {
    double x = this->xStart + i * this->colDim;
    double y = this->yStart + j * this->rowDim;
    double pi = 4 * atan(1.0);
    int segments = 1000;

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(x, y);
      for (int k = 0; k <= segments; k++) {
        float xAux = x + this->radius * cos(2 * pi * k / segments);
        float yAux = y + this->radius * sin(2 * pi * k / segments);
        glVertex2f(xAux, yAux);
      }
    glEnd();
  }

  void scanConvertSegments3(int x0, int y0, int xn, int yn) {
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINES);
      glVertex2f(this->xStart + x0 * this->colDim, this->yStart + y0 * this->rowDim);
      glVertex2f(this->xStart + xn * this->colDim, this->yStart + yn * this->rowDim);
    glEnd();

    if (x0 > xn) {
      std::swap(x0, xn);
      std::swap(y0, yn);
    }

    int dx = abs(xn - x0);
    int dy = abs(yn - y0);

    if (dx == 0) {
      int ymin = y0 < yn ? y0 : yn;
      int ymax = y0 > yn ? y0 : yn;
      for (int y = ymin; y <= ymax; y++) {
        this->drawMultiplePixels(x0, y);
      }
    }
    else if (dy == 0) {
      int xmin = x0 < xn ? x0 : xn;
      int xmax = x0 > xn ? x0 : xn;
      for (int x = xmin; x <= xmax; x++) {
        this->drawMultiplePixels(x, y0);
      }
    }
    else if (dx >= dy) {
      int d = 2 * dy - dx;
      int dE = 2 * dy;
      int dNE = 2 * (dy - dx);
      int x = x0, y = y0;

      while (x <= xn) {
        if ((y0 >= yn && y >= yn) || (y0 <= yn && y <= yn)) {
          this->drawMultiplePixels(x, y);
        }
        if (d <= 0) {
          d += dE;
          x++;
        }
        else {
          d += dNE;
          x++;
          y += y0 < yn ? 1 : -1;
        }
      }
    }
    else {
      int d = 2 * dx - dy;
      int dN = 2 * dx;
      int dNE = 2 * (dx - dy);
      int x = x0, y = y0;

      while (x <= xn) {
        if ((y0 >= yn && y >= yn) || (y0 <= yn && y <= yn)) {
          this->drawMultiplePixels(x, y);
        }
        if (d < 0) {
          d += dN;
          y += y0 < yn ? 1 : -1;
        }
        else {
          d += dNE;
          x++;
          y += y0 < yn ? 1 : -1;
        }
      }
    }
  }

  void drawMultiplePixels(int x, int y) {
    this->writePixel(x, y);
    if (x + 1 <= 20) {
      this->writePixel(x + 1, y);
    }
    if (x - 1 >= this->xStart) {
      this->writePixel(x - 1, y);
    }
    if (y + 1 <= 20) {
      this->writePixel(x, y + 1);
    }
    if (y - 1 >= this->yStart) {
      this->writePixel(x, y - 1);
    }
  }

  void scanConvertCircle4(int radius) {
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      double pi = 4 * atan(1.0);
      int segments = 10000;

      for (int k = 0; k < segments; k++) {
        float xAux = this->xStart + (radius * this->colDim) * cos(2 * pi * k / segments);
        float yAux = this->yStart + (radius * this->rowDim) * sin(2 * pi * k / segments);
        if (
          xAux <= 1 - this->padding && yAux <= 1 - this->padding &&
          xAux >= -1 + this->padding && yAux >= -1 + this->padding
        ) {
          glVertex2f(xAux, yAux);
        }
      }
    glEnd();

    int x = 0;
    int y = radius;
    int d = 1 - radius;
    int dE = 3;
    int dSE = -2 * radius + 5;

    this->drawMultiplePixels(x, y);
    if (x != y) {
      this->drawMultiplePixels(y, x);
    }

    while (y > x) {
      if (d < 0) {
        d += dE;
        dE += 2;
        dSE += 2;
      }
      else {
        d += dSE;
        dE += 2;
        dSE += 4;
        y--;
      }
      x++;

      this->drawMultiplePixels(x, y);
      if (x != y) {
        this->drawMultiplePixels(y, x);
      }
    }
  }
};

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(1);
  glPolygonMode(GL_FRONT, GL_LINE);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);

  CartesianGrid cg = CartesianGrid(20, 20);

  switch (prevKey) {
    case '1':
      cg.scanConvertSegments3(2, 4, 18, 15);
      break;
    case '2':
      cg.scanConvertSegments3(1, 3, 7, 20);
      break;
    case '3':
      cg.scanConvertSegments3(19, 0, 2, 6);
      break;
    case '4':
      cg.scanConvertSegments3(16, 0, 1, 20);
      break;
    case '5':
      cg.scanConvertSegments3(10, 5, 20, 5);
      break;
    case '6':
      cg.scanConvertCircle4(16);
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

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow(argv[0]);
  Init();
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(KeyboardFunc);
  glutDisplayFunc(Display);
  glutMainLoop();
  return 0;
}