#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>

#include "glview.h"
#include "qgifimage/gifimage/qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
  //    file_data data_2;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_open_file_clicked();
  void paint_model(QString);

  void on_scale_valueChanged(double scale_count);

  void on_comboBox_currentIndexChanged(int index);

  void on_choice_x_valueChanged(double arg1);

  void on_choice_y_valueChanged(double arg2);

  void on_choice_z_valueChanged(double arg3);

  void on_rotate_x_valueChanged(double arg4);

  void on_rotate_y_valueChanged(double arg5);

  void on_rotate_z_valueChanged(double arg6);

  void on_change_mode_currentIndexChanged(int index);

  void on_red_index_valueChanged(int value);

  void on_green_index_valueChanged(int value);

  void on_blue_index_valueChanged(int value);

  void on_point_size_valueChanged(int value);

  void on_line_size_valueChanged(int value);

  void on_point_type_circle_clicked();

  void on_point_type_cube_clicked();

  void on_point_type_exist_clicked();

  void on_solid_line_clicked();

  void on_dashed_line_clicked();

  void on_exist_line_clicked();

  void save_settings();
  void upload_settings();

  void on_save_image_clicked();

  void on_save_gif_clicked();

  void gif_record();

 private:
  Ui::MainWindow *ui;
  QString filename, empty;

  QGifImage *gif;
  QTimer *timerGif;

  int frame;

  double prev_scale = 1;

  double prev_move_x = 0;
  double prev_move_y = 0;
  double prev_move_z = 0;

  double prev_rotate_x = 0;
  double prev_rotate_y = 0;
  double prev_rotate_z = 0;
};
#endif  // MAINWINDOW_H
