#include "cli_controller.h"

/// @file
/**
 * @brief Process signal
 *
 * Processes user input making it into game action and passing it
 * to game model.
 *
 * This event is called in the game view every frame.
 *
 * @param user_input The code of the user's pressed key.
 */
void processSignal(int user_input) {
  GameInfo_t stats = getStats();
  UserAction_t result;

  switch (user_input) {
    case ' ':
      result = Action;
      break;

    case KEY_DOWN:
      result = Down;
      break;

    case KEY_UP:
      result = Up;
      break;

    case KEY_LEFT:
      result = Left;
      break;

    case KEY_RIGHT:
      result = Right;
      break;

    case 'p':
    case 'P':
      result = Pause;
      break;

    case 'q':
    case 'Q':
      result = Terminate;
      break;

    case ENTER_KEY:
      result = Start;
      break;

    default:
      result = Up;
  }

  if (stats.pause == GAMELOST) {
    if (user_input == 'q' || user_input == 'Q')
      result = Terminate;
    else if (user_input != ENTER_KEY)
      result = Up;
  } else if (stats.pause == STARTING) {
    if (user_input == 'q' || user_input == 'Q')
      result = Terminate;
    else if (user_input != ENTER_KEY)
      result = Up;
  }

  userInput(result, false);
}
