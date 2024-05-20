#include "glview.h"

#include <iostream>

glView::glView(QWidget *parent) : QOpenGLWidget(parent) {}

void glView::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void glView::paintGL() {
  glClearColor(color_back.red, color_back.green, color_back.blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  set_projection();

  glRotated(xRot, 1, 0, 0);
  glRotated(yRot, 0, 1, 0);

  rendering();
}

void glView::set_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float winWidth = data.max_x;
  float winHeight = data.max_y;
  float fov = 60.0 * M_PI / 180;  // 60 угол в градусах
  float heapHeight = winHeight / (2 * tan(fov / 2));

  if (type_projection) {
    glFrustum(-winWidth, winWidth, -winHeight, winHeight, heapHeight, 9999);
  } else {
    glOrtho(-winWidth, winWidth, -winHeight, winHeight, heapHeight, 9999);
  }
  glTranslated(0, 0, -heapHeight * 3);
}

void glView::rendering() {
  if (redrawing()) {
    glVertexPointer(4, GL_DOUBLE, 0, ver_array);

    if (type_point_view != 0) draw_points();

    if (type_line_view != 0) draw_lines();

    free_vertex_array();
  }
}

int glView::redrawing() {
  if (!fill_vertex_array()) return 0;

  return 1;
}

void glView::draw_points() {
  if (ver_array != nullptr) {
    glPointSize(point_size);
    glColor3d(color_point.red, color_point.green, color_point.blue);

    glEnableClientState(GL_VERTEX_ARRAY);

    if (type_point_view == 1)
      glEnable(GL_QUADS);
    else
      glEnable(GL_POINT_SMOOTH);

    glDrawArrays(GL_POINTS, 0, data.count_of_vertexes);

    if (type_point_view == 1)
      glDisable(GL_QUADS);
    else
      glDisable(GL_POINT_SMOOTH);

    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void glView::draw_lines() {
  glLineWidth(line_size);
  glColor3d(color_line.red, color_line.green, color_line.blue);
  glEnableClientState(GL_VERTEX_ARRAY);
  line_type();
  glDrawElements(GL_LINES, data.cout_arg, GL_UNSIGNED_INT, data.facets);

  if (glIsEnabled(GL_LINE_STIPPLE)) glDisable(GL_LINE_STIPPLE);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void glView::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void glView::mouseMoveEvent(QMouseEvent *mo) {
  xRot = 1 / M_PI * (mo->pos().y() - mPos.y());
  yRot = 1 / M_PI * (mo->pos().x() - mPos.x());
  update();
}

int glView::fill_vertex_array() {
  if (data.count_of_vertexes > 0) {
    ver_array = new double[data.count_of_vertexes * 4]();
  }
  if (ver_array == nullptr) return 0;

  int index = 0;
  for (int j = 0; j < data.VERTEXES.columns; j++) {
    for (int i = 0; i < data.VERTEXES.rows; i++) {
      ver_array[index] = data.VERTEXES.matrix[i][j];
      index += 1;
    }
  }

  return 1;
}

void glView::line_type() {
  if (type_line_view == 2) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 15);
  }
}

void glView::free_vertex_array() {
  if (ver_array != nullptr) {
    delete[](ver_array);
    ver_array = nullptr;
  }
}

void glView::reset() {
  free_data(&data);
  free_vertex_array();
}
