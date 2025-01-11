/*
 * This window is used for displaying the scoreboard.
*/

#ifndef SCOREBOARDWINDOW_HH
#define SCOREBOARDWINDOW_HH

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "gridwindow.hh"

namespace Ui {
class ScoreBoardWindow;
}

class ScoreBoardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreBoardWindow(QWidget *parent = nullptr);
    ~ScoreBoardWindow();

    /*
        * @brief Initializes the scoreboard. gameFinished from gridwindow sends the data here
                through on_game_finished signal in mainwindow, and then this function is called.
        * @param points the points of the game
        * @param time the time of the game
        * @param date the date of the game
        * @param size the size of the game board
    */
    void init_score(int points, QString time, QString date, int size);

private:
    // Instead of using QTDesigner, I created the UI elements in the code
    Ui::ScoreBoardWindow *ui;
    QTableWidget *scoreTable;
    QPushButton *resetButton;
    QPushButton *closeButton;
    QPushButton *aboutButton;

    QVector<QVector<QString>> scoreboardData; // the scoreboard data

    int points_ = 0;
    QString time_ = "";
    QString date_ = "";
    int size_ = 0;

    // helper functions

    /*
        * @brief Initializes the table widget
    */
    void initializeTable();

    /*
        * @brief Reads the scoreboard data from text file
        * I used QFile and QTextStream to write and read.
    */
    void readScoreboardData();

    /*
        * @brief Writes the scoreboard data to text file, in new line
        * I used QFile and QTextStream to write and read.
    */
    void writeScoreboardData();

    /*
        * @brief Sets the scoreboard data to the table widget
    */
    void setScoreboardData();


    // slots

    /*
        * @brief Resets the scoreboard data
    */
    void on_resetButton_clicked();

    /*
        * @brief Closes the scoreboard window
    */
    void on_closeButton_clicked();

    /*
        * @brief Shows description on a smessage box
    */
    void on_about_clicked();
};

#endif // SCOREBOARDWINDOW_HH
