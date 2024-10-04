#ifndef DESKTOP_VIEW_H
#define DESKTOP_VIEW_H

#include <QBrush>
#include <QColor>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QTimer>

#include "../../common.h"

QT_BEGIN_NAMESPACE
namespace Ui {
/**
 * @brief MainWindow class
 *
 * The main class of the program.
 */
class MainWindow;
}  // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void printAll(GameInfo_t &stats);
  void updateStats(GameInfo_t &stats);
  void printStart();
  void printPause();
  void printGameover();
  void printGameoverWon();

  QTimer *getTimer() { return this->timer; }

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *) override;

 private:
  Ui::MainWindow *ui;
  QTimer *timer;

 private slots:
  void timerSlot();
};

#endif  // DESKTOP_VIEW_H
