#include "scoreboardwindow.hh"
#include "ui_scoreboardwindow.h"
#include "gridwindow.hh"
#include "mainwindow.hh"

ScoreBoardWindow::ScoreBoardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreBoardWindow)
{
    setWindowTitle("Scoreboard");

    ui->setupUi(this);

    initializeTable();
    readScoreboardData();
}

ScoreBoardWindow::~ScoreBoardWindow()
{
    delete ui;
}

void ScoreBoardWindow::init_score(int points, QString time, QString date, int size)
{
    points_ = points;
    time_ = time;
    date_ = date;
    size_ = size;
    writeScoreboardData();
}

void ScoreBoardWindow::initializeTable()
{
    // creates table widget
    scoreTable = new QTableWidget(this);
    scoreTable->setColumnCount(4);
    scoreTable->setHorizontalHeaderLabels(QStringList() << "Score" << "Time Elapsed" << "Date" << "Board Size");

    //initializes the buttons
    resetButton = new QPushButton("Reset", this);
    resetButton->setFixedSize(100, 30);
    resetButton->setStyleSheet("background-color: #f44336; color: white;");

    closeButton = new QPushButton("Close", this);
    closeButton->setFixedSize(100, 30);
    closeButton->setStyleSheet("background-color: #4CAF50; color: white;");

    aboutButton = new QPushButton("About", this);
    aboutButton->setStyleSheet("background-color: #2196F3; color: white;");

    connect(resetButton, &QPushButton::clicked, this, &ScoreBoardWindow::on_resetButton_clicked);
    connect(closeButton, &QPushButton::clicked, this, &ScoreBoardWindow::on_closeButton_clicked);
    connect(aboutButton, &QPushButton::clicked, this, &ScoreBoardWindow::on_about_clicked);

    // fills the layout with the widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(aboutButton);
    layout->addWidget(scoreTable);
    layout->addWidget(resetButton);
    layout->addWidget(closeButton);
    setLayout(layout);
}

void ScoreBoardWindow::readScoreboardData()
{
    // cleans scoreboardData at first
    scoreboardData.clear();

    // reads the data from the file and add it to the scoreboard
    QFile file("scoreboard.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList items = line.split(",");
            QVector<QString> row;
            for (const auto& item : items) {
                row.append(item);
            }
            scoreboardData.append(row);
        }
        file.close();
    }
    setScoreboardData();
}

void ScoreBoardWindow::setScoreboardData()
{
    // sets the data to the table
    scoreTable->setRowCount(scoreboardData.size());
    for (int i = 0; i < scoreboardData.size(); ++i) {
        for (int j = 0; j < scoreboardData.at(i).size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(scoreboardData.at(i).at(j));
            scoreTable->setItem(i, j, item);
        }
    }
}

void ScoreBoardWindow::writeScoreboardData()
{
    // writes on scoreboard.txt in new line
    QFile file("scoreboard.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << points_ << "," << time_ << "," << date_ << "," << size_ << "\n";
        file.close();
    }
    readScoreboardData();
}

void ScoreBoardWindow::on_resetButton_clicked()
{
    // clears scoreboard.txt
    QFile file("scoreboard.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "";
        file.close();
    }
    readScoreboardData();
}

void ScoreBoardWindow::on_closeButton_clicked()
{
    close();
}

void ScoreBoardWindow::on_about_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("The scoreboard comprises of the following columns:\n"
                   "1. Score: The score obtained by the player\n"
                   "2. Time Elapsed: The time taken by the player to complete the game\n"
                   "3. Date: The date on which the game was played\n"
                   "4. Board Size: The size of the game board\n\n"
                   "The scoreboard is sorted in descending order of the date played.\n\n"
                   "Score is calculated as follows:\n"
                   "1. 3600 divided by the time elapsed in seconds\n"
                   "2. Multiply the result by the size of the board to make the score fair\n"
                   "3. Deduct 10 points for each invalid move\n\n"
                   "And there you have it, THE FINAL SCORE!\n"
                   );
    msgBox.exec();
}
