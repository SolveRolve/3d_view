#include "mainwindow.h"

#include <QtOpenGL>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  upload_settings();

  ui->red_index->setValue(ui->openGLWidget->color_line.red);
  ui->red_value->setNum(ui->openGLWidget->color_line.red);
  ui->green_index->setValue(ui->openGLWidget->color_line.green);
  ui->green_value->setNum(ui->openGLWidget->color_line.green);
  ui->blue_index->setValue(ui->openGLWidget->color_line.blue);
  ui->blue_value->setNum(ui->openGLWidget->color_line.blue);

  ui->point_size_value->setNum(ui->openGLWidget->point_size);
  ui->line_size_value->setNum(ui->openGLWidget->line_size);

  ui->solid_line->setChecked(ui->openGLWidget->type_line_view);
  ui->dashed_line->setChecked(!ui->openGLWidget->type_line_view);
  ui->exist_line->setChecked(!ui->openGLWidget->type_line_view);

  ui->point_type_circle->setChecked(!ui->openGLWidget->type_point_view);
  ui->point_type_cube->setChecked(ui->openGLWidget->type_point_view);
  ui->point_type_exist->setChecked(ui->openGLWidget->type_point_view);

  ui->comboBox->setCurrentIndex(ui->openGLWidget->type_projection);

  if (filename != empty) {
    paint_model(filename);
  }
}

MainWindow::~MainWindow() {
  save_settings();
  ui->openGLWidget->reset();
  delete ui;
}

void MainWindow::on_open_file_clicked() {
  filename = QFileDialog::getOpenFileName(this, "Open file", "/Users",
                                          "OBJ files (*.obj)");

  if (!filename.isEmpty()) {
    ui->openGLWidget->reset();
    paint_model(filename);
  } else {
    QMessageBox msgBox;
    msgBox.setText("Can`t read file");
  }
}

void MainWindow::paint_model(QString filename) {
  QByteArray ba = filename.toLocal8Bit();
  char *path_to_file = ba.data();

  if (!view(path_to_file, &ui->openGLWidget->data)) {
    ui->filename_label->setText(filename);
    QString result_numver_of_vertex =
        QString::number(ui->openGLWidget->data.count_of_vertexes);
    ui->vertex_count_label->setText("Vertex count: " + result_numver_of_vertex);
    QString result_numver_of_facets =
        QString::number(ui->openGLWidget->data.count_of_facets);
    ui->facets_count_label->setText("Facets count: " + result_numver_of_facets);
    ui->openGLWidget->update();
  } else {
    ui->openGLWidget->reset();
  }
}

void MainWindow::on_scale_valueChanged(double scale_count) {
  double shift = scale_count / prev_scale;
  scaling(&ui->openGLWidget->data.VERTEXES, shift);
  prev_scale = scale_count;
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
  ui->openGLWidget->type_projection = index;
  ui->openGLWidget->update();
}

void MainWindow::on_choice_x_valueChanged(double arg1) {
  double shift = arg1 - prev_move_x;
  move_x(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_move_x = arg1;
}

void MainWindow::on_choice_y_valueChanged(double arg2) {
  double shift = arg2 - prev_move_y;
  move_y(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_move_y = arg2;
}

void MainWindow::on_choice_z_valueChanged(double arg3) {
  double shift = arg3 - prev_move_z;
  move_z(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_move_z = arg3;
}

void MainWindow::on_rotate_x_valueChanged(double arg4) {
  double shift = arg4 - prev_rotate_x;
  rotation_by_ox(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_rotate_x = arg4;
}

void MainWindow::on_rotate_y_valueChanged(double arg5) {
  double shift = arg5 - prev_rotate_y;
  rotation_by_oy(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_rotate_y = arg5;
}

void MainWindow::on_rotate_z_valueChanged(double arg6) {
  double shift = arg6 - prev_rotate_z;
  rotation_by_oz(&ui->openGLWidget->data.VERTEXES, shift);
  ui->openGLWidget->update();
  prev_rotate_z = arg6;
}

void MainWindow::on_change_mode_currentIndexChanged(int index) {
  if (index == 0) {
    ui->red_index->setValue(ui->openGLWidget->color_line.red);
    ui->red_value->setNum(ui->openGLWidget->color_line.red);
    ui->green_index->setValue(ui->openGLWidget->color_line.green);
    ui->green_value->setNum(ui->openGLWidget->color_line.green);
    ui->blue_index->setValue(ui->openGLWidget->color_line.blue);
    ui->blue_value->setNum(ui->openGLWidget->color_line.blue);
  } else if (index == 1) {
    ui->red_index->setValue(ui->openGLWidget->color_point.red);
    ui->red_value->setNum(ui->openGLWidget->color_point.red);
    ui->green_index->setValue(ui->openGLWidget->color_point.green);
    ui->green_value->setNum(ui->openGLWidget->color_point.green);
    ui->blue_index->setValue(ui->openGLWidget->color_point.blue);
    ui->blue_value->setNum(ui->openGLWidget->color_point.blue);
  } else {
    ui->red_index->setValue(ui->openGLWidget->color_back.red);
    ui->red_value->setNum(ui->openGLWidget->color_back.red);
    ui->green_index->setValue(ui->openGLWidget->color_back.green);
    ui->green_value->setNum(ui->openGLWidget->color_back.green);
    ui->blue_index->setValue(ui->openGLWidget->color_back.blue);
    ui->blue_value->setNum(ui->openGLWidget->color_back.blue);
  }
}

void MainWindow::on_red_index_valueChanged(int value) {
  if (ui->change_mode->currentIndex() == 0)
    ui->openGLWidget->color_line.red = value;
  else if (ui->change_mode->currentIndex() == 1)
    ui->openGLWidget->color_point.red = value;
  else if (ui->change_mode->currentIndex() == 2)
    ui->openGLWidget->color_back.red = value;
  ui->red_value->setNum(value);
  ui->openGLWidget->update();
}

void MainWindow::on_green_index_valueChanged(int value) {
  if (ui->change_mode->currentIndex() == 0)
    ui->openGLWidget->color_line.green = value;
  else if (ui->change_mode->currentIndex() == 1)
    ui->openGLWidget->color_point.green = value;
  else if (ui->change_mode->currentIndex() == 2)
    ui->openGLWidget->color_back.green = value;
  ui->green_value->setNum(value);
  ui->openGLWidget->update();
}

void MainWindow::on_blue_index_valueChanged(int value) {
  if (ui->change_mode->currentIndex() == 0)
    ui->openGLWidget->color_line.blue = value;
  else if (ui->change_mode->currentIndex() == 1)
    ui->openGLWidget->color_point.blue = value;
  else if (ui->change_mode->currentIndex() == 2)
    ui->openGLWidget->color_back.blue = value;
  ui->blue_value->setNum(value);
  ui->openGLWidget->update();
}

void MainWindow::on_point_size_valueChanged(int value) {
  ui->openGLWidget->point_size = value;
  ui->point_size_value->setNum(value);
  ui->openGLWidget->update();
}

void MainWindow::on_line_size_valueChanged(int value) {
  ui->openGLWidget->line_size = value;
  ui->line_size_value->setNum(value);
  ui->openGLWidget->update();
}

void MainWindow::on_point_type_circle_clicked() {
  ui->openGLWidget->type_point_view = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_point_type_cube_clicked() {
  ui->openGLWidget->type_point_view = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_point_type_exist_clicked() {
  ui->openGLWidget->type_point_view = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_solid_line_clicked() {
  ui->openGLWidget->type_line_view = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_dashed_line_clicked() {
  ui->openGLWidget->type_line_view = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_exist_line_clicked() {
  ui->openGLWidget->type_line_view = 0;
  ui->openGLWidget->update();
}

void MainWindow::save_settings() {
  QSettings settings(
      QCoreApplication::applicationDirPath() + "/4d_settings.conf",
      QSettings::NativeFormat);
  settings.setValue("Backgroung color_blue", ui->openGLWidget->color_back.blue);
  settings.setValue("Backgroung color_green",
                    ui->openGLWidget->color_back.green);
  settings.setValue("Backgroung color_red", ui->openGLWidget->color_back.red);

  settings.setValue("point_color color_blue",
                    ui->openGLWidget->color_point.blue);
  settings.setValue("point_color color_green",
                    ui->openGLWidget->color_point.green);
  settings.setValue("point_color color_red", ui->openGLWidget->color_point.red);

  settings.setValue("lines_color color_blue",
                    ui->openGLWidget->color_line.blue);
  settings.setValue("lines_color color_green",
                    ui->openGLWidget->color_line.green);
  settings.setValue("lines_color color_red", ui->openGLWidget->color_line.red);

  settings.setValue("line_type_view", ui->openGLWidget->type_line_view);
  settings.setValue("point_type_view", ui->openGLWidget->type_point_view);

  settings.setValue("size_point", ui->openGLWidget->point_size);
  settings.setValue("size_line", ui->openGLWidget->line_size);

  settings.setValue("type_projection", ui->openGLWidget->type_projection);

  settings.setValue("Previous_file", filename);
}

void MainWindow::upload_settings() {
  QSettings settings(
      QCoreApplication::applicationDirPath() + "/4d_settings.conf",
      QSettings::NativeFormat);
  ui->openGLWidget->color_back.blue =
      settings.value("Backgroung color_blue", 255).toDouble();
  ui->openGLWidget->color_back.green =
      settings.value("Backgroung color_green", 0).toDouble();
  ui->openGLWidget->color_back.red =
      settings.value("Backgroung color_red", 0).toDouble();

  ui->openGLWidget->color_point.blue =
      settings.value("point_color color_blue", 0).toDouble();
  ui->openGLWidget->color_point.green =
      settings.value("point_color color_green", 0).toDouble();
  ui->openGLWidget->color_point.red =
      settings.value("point_color color_red", 255).toDouble();

  ui->openGLWidget->color_line.blue =
      settings.value("lines_color color_blue", 0).toDouble();
  ui->openGLWidget->color_line.green =
      settings.value("lines_color color_green", 0).toDouble();
  ui->openGLWidget->color_line.red =
      settings.value("lines_color color_red", 0).toDouble();

  filename = settings.value("Previous_file", empty).toString();

  ui->openGLWidget->type_line_view =
      settings.value("line_type_view", 1).toInt();
  ui->openGLWidget->type_point_view =
      settings.value("point_type_view", 2).toInt();

  ui->openGLWidget->point_size = settings.value("size_point", 3).toInt();
  ui->openGLWidget->line_size = settings.value("size_line", 3).toInt();

  ui->openGLWidget->type_projection =
      settings.value("type_projection", 0).toInt();
}

void MainWindow::on_save_image_clicked() {
  QImage image = ui->openGLWidget->grabFramebuffer();
  QString fileName = QFileDialog::getSaveFileName(
      this, "Save Image", "", "JPEG (*.jpeg);; BMP (*.bmp)");

  if (!fileName.isEmpty()) {
    QString format = QFileInfo(fileName).suffix().toLower();
    if (format == "jpeg" || format == "jpg") {
      image.save(fileName, "JPEG");
    } else if (format == "bmp") {
      image.save(fileName, "BMP");
    } else {
      QMessageBox::critical(this, "Error", "Invalid file format");
    }
  }
}

void MainWindow::on_save_gif_clicked() {
  gif = new QGifImage(QSize(1810, 1810));
  timerGif = new QTimer(this);
  connect(timerGif, &QTimer::timeout, this, &MainWindow::gif_record);
  timerGif->setInterval(100);
  timerGif->start();
  //    frame = 0;
}

void MainWindow::gif_record() {
  QImage image(640, 480, QImage::Format_RGB32);
  image = ui->openGLWidget->grabFramebuffer();
  gif->addFrame(image, 100);  // цифра это задержка
  frame++;
  if (frame == 50) {
    QString file_name =
        QFileDialog::getSaveFileName(this, "Save Gif", "", "Gif Files (*.gif)");
    timerGif->stop();
    gif->save(file_name);
    QFileInfo check_file(file_name);
    if (check_file.exists())
      QMessageBox::information(NULL, QObject::tr("Информация"),
                               tr("Файл охранен успешно"));
    else
      QMessageBox::critical(NULL, QObject::tr("Ошибка"),
                            tr("Файл не сохранен"));
    frame = 0;
    delete gif;
    delete timerGif;
  }
}
