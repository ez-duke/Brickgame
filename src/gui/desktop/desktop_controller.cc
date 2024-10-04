#include "../../common.h"
#include "desktop_view.h"

/**
 * @brief Key press event
 *
 * Processes user input making it into game action and passing it
 * to game model.
 *
 * This event is called in the game view when user presses a key.
 *
 * @param event Event of the pressed key
 */
void MainWindow::keyPressEvent(QKeyEvent *event) {
  GameInfo_t stats = getStats();
  UserAction_t result;

  switch (event->key()) {
    case Qt::Key::Key_Space:
      result = Action;
      break;

    case Qt::Key::Key_Down:
      result = Down;
      break;

    case Qt::Key::Key_Up:
      result = Up;
      break;

    case Qt::Key::Key_Left:
      result = Left;
      break;

    case Qt::Key::Key_Right:
      result = Right;
      break;

    case Qt::Key::Key_P:
      result = Pause;
      break;

    case Qt::Key::Key_Q:
      result = Terminate;
      this->close();
      break;

    case Qt::Key::Key_Enter:
    case Qt::Key::Key_Return:
      result = Start;
      break;

    default:
      result = Up;
  }

  if (stats.pause == GAMELOST) {
    if (event->key() == Qt::Key::Key_Q)
      result = Terminate;
    else if (event->key() != Qt::Key::Key_Enter &&
             event->key() != Qt::Key::Key_Return)
      result = Up;
  } else if (stats.pause == STARTING) {
    if (event->key() == Qt::Key::Key_Q)
      result = Terminate;
    else if (event->key() != Qt::Key::Key_Enter &&
             event->key() != Qt::Key::Key_Return)
      result = Up;
  }

  userInput(result, false);
}
