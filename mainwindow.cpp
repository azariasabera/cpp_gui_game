#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Binairo");

    gameboard =  std::make_shared<GameBoard>();
    scoreboardWindow = new ScoreBoardWindow(this);

    connect(ui->launch, &QPushButton::clicked, this, &MainWindow::on_launch_clicked);
    connect(ui->quit, &QPushButton::clicked, this, &MainWindow::on_quit_clicked);
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::on_start_clicked);
    connect(ui->back, &QPushButton::clicked, this, &MainWindow::on_back_clicked);
    connect(ui->seed, &QLineEdit::textChanged, this, &MainWindow::on_seed_textChanged);
    connect(ui->size, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_size_valueChanged);
    connect(ui->default_file, &QRadioButton::toggled, this, &MainWindow::on_default_file_toggled);
    connect(ui->user_given, &QRadioButton::toggled, this, &MainWindow::on_user_given_toggled);

    createMenu();
    disableOnStart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    QMenu menu(ui->start);
    QAction *actionRandom = menu.addAction("Fill randomly");
    QAction *actionUser = menu.addAction("Fill manually");
    QAction *actionFile = menu.addAction("Fill from file");

    QAction *selectedAction = menu.exec(ui->start->mapToGlobal(QPoint(0, ui->start->height())));

    if (selectedAction == actionRandom)
        fillRandomly();
    else if (selectedAction == actionUser)
        fillManually();
    else if (selectedAction == actionFile)
        fillFromFile();
}

void MainWindow::fillRandomly()
{
    disableOnStart();
    starting_way_ = RAND;
    ui->seed->show();
    ui->seed_label->show();
}

void MainWindow::fillManually()
{
    disableOnStart();
    starting_way_ = MANUAL;
    gridWindow = new GridWindow(INPUT, this); // input mode
    connect(gridWindow, &GridWindow::new_element, this, &MainWindow::on_new_element);
    gridWindow->init_game_board(gameboard, size); // useful for the grid window
    gridWindow->show();
    ui->launch->show();
}

void MainWindow::fillFromFile()
{
    disableOnStart();
    starting_way_ = FROM_FILE;
    ui->from_file->show();
    ui->default_file->show();
    ui->user_given->show();
}

void MainWindow::on_default_file_toggled(bool checked)
{
    if (checked){
    ui->from_file->setText("default.txt");
    readFromInput("default.txt");
}}


void MainWindow::on_user_given_toggled(bool checked)
{
    if (checked){ // opens a file dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        ui->from_file->setText(fileName);
        readFromInput(fileName.toStdString());
    }
}}

void MainWindow::readFromInput(std::string file_name)
{
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        user_input = " " + line + " "; // so it fits in fill_from_input
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Unable to read! Try again.");
        msgBox.exec();
        return;
    }
    file.close();
        ui->launch->show();
}

void MainWindow::on_launch_clicked()
{
    switch (starting_way_) {
    case MANUAL:
    case FROM_FILE:
        if (!gameboard->fill_from_input(user_input, size)){
            QMessageBox msgBox;
            msgBox.setText("Invalid input! Try again.");
            msgBox.exec();

            user_input = std::string(size*size + 2, ' ');
            return;
        }
        break;
    case RAND:
        if (!gameboard->fill_randomly(seed, size)){
            QMessageBox msgBox;
            msgBox.setText("Invalid seed value! Try again.");
            msgBox.exec();
            return;
        }
        break;
        }

    gridWindow = new GridWindow(GAME, this); // game mode
    gridWindow->init_game_board(gameboard, size); // useful for the grid window
    gridWindow->show();
    connect(gridWindow, &GridWindow::gameFinished, this, &MainWindow::on_gameFinished);
}

void MainWindow::disableOnStart()
{
    ui->seed->hide();
    ui->seed_label->hide();
    ui->user_given->hide();
    ui->from_file->hide();
    ui->default_file->hide();
    ui->launch->hide();
}

void MainWindow::on_new_element(int column, int row, char type)
{
   if (type == '0')
        user_input[size * row + column + 1] = '0';
    else
       user_input[size * row + column + 1] = '1';
}

void MainWindow::on_gameFinished(int points, QString time, QString date, int size)
{
    scoreboardWindow->init_score(points, time, date, size);
}

void MainWindow::on_size_valueChanged(int arg1)
{
    size = arg1;
    user_input = std::string(size*size +2, ' ');
}

void MainWindow::on_seed_textChanged(const QString &arg1)
{
    seed = arg1.toInt();
    ui->launch->show();
}

void MainWindow::on_back_clicked()
{
    disableOnStart();
}

void MainWindow::on_quit_clicked()
{
    close();
}

void MainWindow::createMenu()
{
   // creates action for help and scoreboard
    QAction *helpAction = new QAction(tr("Help"), this);
    QAction *scoreboardAction = new QAction(tr("Scoreboard"), this);

    QMenu *menu = menuBar()->addMenu(tr("Menu"));
    menu->addAction(helpAction);
    menu->addAction(scoreboardAction);

    connect(scoreboardAction, &QAction::triggered, this, [this](){
        scoreboardWindow->show();
    });

    connect(helpAction, &QAction::triggered, this, [this](){
    QMessageBox::information(this, tr("Help"), tr("Game rules: \n\n"
                           "1. The board is filled with 0s and 1s\n"
                           "2. No more than two similar numbers can be adjacent\n"
                           "3. No three rows or columns can be the same\n"
                           "4. Every wrong move deducts 10 points\n"
                           "5. The game is won when all the rules are followed\n"
                           "6. The game can be paused and reset\n"
                           "7. The game can be initialized in three ways: random, manual, and from file\n"
                           "8. The game can be played in different sizes from 2 to 10\n"
                           "9. The game can be quit at any time\n"));
    });
}

