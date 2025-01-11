/*
 * This window is used for displaying the grid; it is where the game is played.
*/

#ifndef GRIDWINDOW_HH
#define GRIDWINDOW_HH

#include <QDialog>
#include <QTimer>
#include <QPushButton>
#include "gameboard.hh"
#include <QDate>

namespace Ui {
class GridWindow;
}

/*
    * @brief The Grid_state enum
    * represents the state of the grid
    * GAME: the game is running
    * INPUT: the user is manually inputting the coordinates
*/
enum Grid_state { GAME, INPUT };

const int score_base = 3600; // base score for the game 1 hour in seconds

class GridWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GridWindow(Grid_state grid_state, QWidget *parent = nullptr);
    ~GridWindow();

    /*
        * @brief Initializes the game board, gets the parameters from the main window
        * @param gameboard the game board
        * @param size the size of the game board
    */
    void init_game_board(std::shared_ptr<GameBoard> gameboard, int size);

signals:
    /*
        * @brief Signal to send new element to the game board
        * @param column the column of the new element
        * @param row the row of the new element
        * @param type the type of the new element
    */
    void new_element(int column, int row, char type);

    /*
        * @brief Signal to send the game result to the main window
        * @param points the points of the game
        * @param time the time of the game
        * @param date the date of the game
        * @param size the size of the game board
    */
    void gameFinished(int points, QString time, QString date, int size);


private:
    Ui::GridWindow *ui;

    QTimer* timer; // timer for the game

    /*
        @brief These are buttons given at the start. They are disabled
        when the game starts, allowing the user to only interact with the rest of the buttons
    */
    std::vector<QPushButton*> buttons_at_start;
    std::shared_ptr<GameBoard> gameboard_;

    std::string input_coordinates;
    int seconds_ = 0;
    int score = 0; // score of the game
    unsigned int size_ = SIZE;
    Grid_state grid_state_;

    // helper functions

    /*
        * @brief Creates the game board
    */
    void createGameBoard();

    /*
        * @brief disables buttons not needed on start
    */
    void disableOnStart();

    /*
        * @brief Updates the values of the buttons
        * takes the values from the game board and updates the buttons' text
    */
    void updateButtonValues();

    /*
        * @brief Starts the timer
    */
    void startTimer();

    /*
        * @brief Disables the buttons
        * disables buttons_at_start
    */
    void disableButtons();

    /*
        * @brief hides/shows widgets
        * calls updateButtonValues, disables buttons_at_start, starts the timer
    */
    void startGame();

    /*
        * @brief Updates the game board
        * @param col the column of the new element
        * @param row the row of the new element
        * @param symbol the symbol of the new element
    */
    void updateGameBoard(int col, int row, char symbol);

    /*
        * @brief Shows a signal that the move is invalid
    */
    void onInvalidMove();

    /*
        * @brief This will display the game points and the time elapsed in a text browser
        * I implemented an extra feature where the text browser's background color changes to
        * different colors and then settles in orange
        * reset will reset the text browser and the game
    */
    void onGameEnd();

    // slots

    /*
        * @brief Slot for the quit button
        * closes the game
    */
    void on_quit_button_clicked();

    /*
        * @brief Slot for the pause button
        * pauses the game, freezes the timer, disables widgets except quit and itself
        * once paused, the button will change to resume
    */
    void on_pause_button_clicked();

    /*
        * @brief Slot for the reset button
        * resets the game, resets the timer, resets buttons except for buttons_at_start
        * RESET WILL NOT REMOVE THE BUTTONS ASSIGNED AT THE START in my implementation
    */
    void on_reset_button_clicked();

    /*
        * @brief Slot for the done button
        * closes the grid window once the manual filling is done
    */
    void on_done_button_clicked();

    /*
        * @brief Slot for the timer
        * updates the time
    */
    void on_timeout();

    /*
        * @brief Slot for the buttons on the grid.
        * When clicked three option will come to fill the button, in turn the gameboard.
        * Here there is a third option called Empty which will UNDO an assigned letter 0 or 1.
    */
    void on_button_clicked();

};

#endif // GRIDWINDOW_HH
