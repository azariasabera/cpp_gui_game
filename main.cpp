/* =========PROGRAM DESCRIPTION==========
 *
 * Desc:
 * In this binairo game, the player is presented with a grid of empty cells.
 * There are three ways to fill the grid: randomly, manually, or from a file.
 * When choosing manually it will open a new window where the player can fill the grid.
 * If the filling process is not successful, a message box will appear with the result.
 *
 * GENERAL GAME INSTRUCTIONS:
 * 
 * 1. When a grid button is clicked, three options will come. Choosing empty will undo any previous assignment.
 * 2. The grid must not contain three consecutive 0s or 1s in any row or column. (same logic as the last project).
 * 3. If the above happens, 10 points will be deducted from the player's score. IT WON'T ALLOW TO ADD ERRONEOUS NUMBER!
 * 4. The points are calculated as follows: [60*60(total possible time) * size )/time taken (in seconds) - wrong moves * 10]
 * 5. My code will not allow erroneous filling of the grid, rather it will prevent the player from making a mistake.
 *   IT WILL INDICATE IT WAS ERRONEOUS BY A RED LIGHT!
 * 6. When the game ends, the player can see their score and the time taken to complete the game.
 *   There will aslo be color changes in the text browser, to indicate the game ended.
 * 7. In pause mode, the player can only quit or resume the game. Everything else will be disabled.
 * 8. In reset mode, I only clean the grids that the user filled after the timer started.
 *
 * LASTLY, GAME ENDS WHEN ALL THE BOARD IS FILED WITH NUMBERS. THE AMOUNT OF ERRORS YOU MADE AND THE TIME
 * YOU TAKE TO FILL THE BOARD WILL AFFECT YOUR SCORE NEGATIVELY.
 *
 */

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
