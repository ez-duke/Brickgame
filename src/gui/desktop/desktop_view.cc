#include "desktop_view.h"

#include "./ui_desktop_view.h"
/// @file
/**
 * @brief MainWindow Constructor
 *
 * Constructs main window object. Sets up ui, grabs keyboard,
 * connects timer and sets window title to Brickgame.
 *
 * @param parent Parental class to inheriit from
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  grabKeyboard();

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  timer->start(5);

  setWindowTitle("Brickgame");
}

/**
 * @brief MainWindow Destructor
 *
 * Deletes ui on destruction.
 */
MainWindow::~MainWindow() { delete ui; }

/**
 * @brief Entry point
 *
 * Execution of the program starts here.
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return Program exit status
 */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

/**
 * @brief Print everything
 *
 * Prints game overlay, stats, field, states (such as pause and gameover),
 * and controls.
 *
 * @param stats Basic game structure, passed from game model
 */
void MainWindow::printAll(GameInfo_t &stats) {
  updateStats(stats);
  ui->GameStatus->setText("");
  if (stats.next) {
    ui->NextFigureLabels->setText("Next");
  }

  if (stats.pause == STARTING) {
    printStart();
  } else if (stats.pause == PAUSED) {
    printPause();
  } else if (stats.pause == GAMELOST) {
    printGameover();
  } else if (stats.pause == GAMEWON) {
    printGameoverWon();
  }
}

/**
 * @brief Print start
 *
 * Prints starting screen which waits for user input to start the game.
 */
void MainWindow::printStart() {
  ui->GameStatus->setText("Start game\nPress 'Enter' to start\nOr 'Q' to quit");
}

/**
 * @brief Print pause
 *
 * Prints pause screen which waits for user input to resume the game.
 */
void MainWindow::printPause() {
  ui->GameStatus->setText("Game paused\nPress 'P' to resume\nOr 'Q' to quit");
}

/**
 * @brief Print gameover
 *
 * Prints gameover screen which waits for user input to restart the game.
 */
void MainWindow::printGameover() {
  ui->GameStatus->setText(
      "GAME OVER\nPress 'Enter' to try again\nOr 'Q' to quit");
}

/**
 * @brief Print game won
 *
 * Prints game won screen which waits for user input to restart the game.
 */
void MainWindow::printGameoverWon() {
  ui->GameStatus->setText(
      "YOU WON\nPress 'Enter' to try again\nOr 'Q' to quit");
}

/**
 * @brief Update stats
 *
 * Updates and prints game stats, including score, highscore, level, speed
 * and next figure (in tetris only).
 *
 * @param stats Basic game structure, passed from game model
 */
void MainWindow::updateStats(GameInfo_t &stats) {
  ui->ScoreLabel->setText(QString::number(stats.score));
  ui->HighscoreLabel->setText(QString::number(stats.high_score));
  ui->LevelLabel->setText(QString::number(stats.level));
  ui->SpeedLabel->setText(QString::number(stats.speed));
}

/**
 * @brief Paint event
 *
 * Paints game field, cycling through every coordinate, painting a cell
 * of the field if it exists.
 *
 * Also paints next figure (in tetris only).
 *
 * @param * Called paint event
 */
void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter;
  GameInfo_t stats = getStats();
  painter.begin(this);
  painter.setPen(Qt::NoPen);

  for (int i = 0; stats.field && i < FIELD_HEIGHT; ++i) {
    for (int j = 0; stats.field && j < FIELD_WIDTH; ++j) {
      if (stats.field[i][j] == 1)
        painter.setBrush(QColor{0, 200, 0});
      else if (stats.field[i][j] == 2)
        painter.setBrush(QColor{200, 50, 0});
      else
        painter.setBrush(QColor{75, 75, 75});

      painter.drawRect(40 * j, 40 * i, 40, 40);
    }
  }

  if (stats.next) {
    painter.setBrush(QColor{0, 200, 0});
    for (int i = 0; i < BRICK_SIDE; i++) {
      for (int j = 0; j < BRICK_SIDE; j++) {
        if (stats.next[i][j])
          painter.drawRect(425 + 20 * j, 410 + 20 * i, 20, 20);
      }
    }
  }
  painter.end();
}

/**
 * @brief Timer slot
 *
 * This slot is called every time a timeout is executed
 * from the corresponding timer.
 *
 * It updates the game state, repaints the field and prints stats.
 */
void MainWindow::timerSlot() {
  GameInfo_t stats = updateCurrentState();
  repaint();
  printAll(stats);
}
