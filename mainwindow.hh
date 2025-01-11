/*
 *
 * This window is initiating the game by choosing a starting way.
 * It launches the game to be played in grid window
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>

#include "gridwindow.hh"
#include "gameboard.hh"
#include "scoreboardwindow.hh"

#include <iostream>
#include <memory>
#include <fstream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
    * @brief The Starting_way enum
    * represents the way the game is started
    * RAND: the game is started with random values
    * MANUAL: the game is started with manual values
    * FROM_FILE: the game is started with values from a file
*/
enum Starting_way { RAND, MANUAL, FROM_FILE };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GridWindow *gridWindow;
    ScoreBoardWindow *scoreboardWindow;

    std::shared_ptr<GameBoard> gameboard;
    std::string user_input = std::string(SIZE*SIZE + 2, ' '); // default input
    unsigned int size = SIZE; // default size
    int seed = 0; // default seed

    Starting_way starting_way_ = RAND;

    // helper functions

    /*
        * @brief Initializes the fill_randomly menu
        * activates launch button, which does the actual filling
    */
    void fillRandomly();

    /*
        * @brief Initializes the fill_manually menu
        * activates launch button, which does the actual filling
    */
    void fillManually();

    /*
        * @brief Initializes the fill_from_file menu
        * activates user_given and default_file radio buttons
    */
    void fillFromFile();

    /*
        * @brief Reads the input from the file
        * @param file_name name of the file
    */
    void readFromInput(std::string file_name); // helper function for fillFromFile

    /*
        * @brief Creates the menu for the start button
        * consists of fill_randomly, fill_manually and fill_from_file
    */
    void createMenu();

    /*
        * @brief Disables button not needed on start
    */
    void disableOnStart();

    // slots

    /*
        * @brief Slot for start button
        * creates the menu for the start button
    */
    void on_start_clicked();

    /*
        * @brief Slot for size spinbox
        * @param arg1 new size
    */
    void on_size_valueChanged(int arg1);

    /*
        * @brief Slot for default_file radio button
        * @param checked state of the radio button
        * calls readFromInput with default file name
    */
    void on_default_file_toggled(bool checked);

    /*
        * @brief Slot for user_given radio button
        * @param checked state of the radio button
        * calls readFromInput with user given file name
    */
    void on_user_given_toggled(bool checked);

    /*
        * @brief Slot for seed text
        * @param arg1 new seed
    */
    void on_seed_textChanged(const QString &arg1);

    /*
        * @brief Slot for launch button
        * fills the gameboard according to the starting way
    */
    void on_launch_clicked();

    /*
        * @brief Slot for new_element signal, needed for manual filling
        * @param column column of the new element
        * @param row row of the new element
        * @param type type of the new element
    */
    void on_new_element(int column, int row, char type);

    /*
        * @brief Slot for gameFinished signal
        * @param points points of the game
        * @param time time of the game
        * @param date date of the game
        * @param size size of the game
    */
    void on_gameFinished(int points, QString time, QString date, int size);

    /*
        * @brief Slot for back button
        * disables buttons not needed on start
    */
    void on_back_clicked();

    /*
        * @brief Slot for quit button
    */
    void on_quit_clicked();
};
#endif // MAINWINDOW_HH
