#ifndef GLVIEW_HPP
#define GLVIEW_HPP

//#include <QGLWidget>
#define GL_SILENCE_DEPRECATION
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QtGui>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include "../backend/s21_3dviewer.h"
#ifdef __cplusplus
}
#endif

class glView : public QOpenGLFunctions, public QOpenGLWidget {
 public:
  count_of_elements data = {0};
  double *ver_array = nullptr;

  int type_projection = 0;
  double point_size = 3;
  double line_size = 3;
  int type_line_view = 1;
  int type_point_view = 2;

  typedef struct color {
    double red;
    double green;
    double blue;
  } color;
  color color_point = {255, 0, 0};
  color color_line = {0, 0, 255};
  color color_back = {0, 0, 0};

  explicit glView(QWidget *parent = 0);

  void initializeGL() override;
  void paintGL() override;

  void set_projection();
  void rendering();
  int redrawing();

  void draw_points();
  void draw_lines();

  int fill_vertex_array();

  void line_type();

  void reset();

  void free_vertex_array();

 private:
  float xRot{}, yRot{};
  QPoint mPos;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
};

#endif  // GLVIEW_H
