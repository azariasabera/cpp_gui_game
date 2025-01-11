#include "gridwindow.hh"
#include "ui_gridwindow.h"

#include <QMenu>

GridWindow::GridWindow(Grid_state grid_state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GridWindow),
    grid_state_(grid_state)
{
    ui->setupUi(this);

    setWindowTitle("Playing Grid");

    connect(ui->done, &QPushButton::clicked, this, &GridWindow::on_done_button_clicked);
    connect(ui->pause_button, &QPushButton::clicked, this, &GridWindow::on_pause_button_clicked);
    connect(ui->reset_button, &QPushButton::clicked, this, &GridWindow::on_reset_button_clicked);
    connect(ui->quit_button, &QPushButton::clicked, this, &GridWindow::on_quit_button_clicked);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GridWindow::on_timeout);
    ui->minutes->setStyleSheet("background-color: rgb(0, 128, 128)");
    ui->seconds->setStyleSheet("background-color: green");

    disableOnStart();
}

GridWindow::~GridWindow()
{
    delete ui;
}

void GridWindow::init_game_board(std::shared_ptr<GameBoard> gameboard, int size)
{
    gameboard_ = gameboard;
    size_ = size;

    createGameBoard();

    if (grid_state_ == GAME)
        startGame();
}

void GridWindow::createGameBoard()
{
    ui->gridLayout->setSpacing(0);
    for (unsigned int i = 0; i < size_; i++) {
        for (unsigned int j = 0; j < size_; j++) {

            QPushButton *button = new QPushButton(this);
            ui->gridLayout->addWidget(button, i, j);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            connect(button, &QPushButton::clicked, this, &GridWindow::on_button_clicked);
            button->setObjectName(QString::number(i) + QString::number(j));
            button->setStyleSheet("background-color: white");
}}}

void GridWindow::startGame()
{
    ui->done->hide();
    ui->pause_button->show();
    ui->reset_button->show();
    ui->minutes->show();
    ui->seconds->show();

    updateButtonValues();
    startTimer();
    disableButtons();
}

void GridWindow::updateButtonValues()
{
    for (unsigned int i = 0; i < size_; ++i) {
        for (unsigned int j = 0; j < size_; ++j) {

            QPushButton *button;
            button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(i, j)->widget());

            int value = gameboard_->get_board().at(i).at(j);
            QString buttonText;
            if (value == 0)
                buttonText = "0";
            else if (value == 1)
                buttonText = "1";
            else if (value == 2)
                buttonText = " ";
            button->setText(buttonText);
}}}

void GridWindow::startTimer()
{
    timer->start(1000);
}

void GridWindow::disableButtons()
{
    for (unsigned int i = 0; i < size_; ++i) {
        for (unsigned int j = 0; j < size_; ++j) {

            QPushButton *button;
            button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(i, j)->widget());

            if (button->text() != " ") {
                button->setEnabled(false);
                button->setStyleSheet("QPushButton:disabled { color: black; background-color: "
"                                      lightgray; border: 1px solid black; }");
                buttons_at_start.push_back({button});
}}}}

void GridWindow::on_button_clicked()
{
    // to determine which button was clicked
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QMenu menu(button);
        QAction *actionZero = menu.addAction("Zero");
        QAction *actionOne = menu.addAction("One");
        QAction *actionEmpty = menu.addAction("Empty");
        QAction *selectedAction = menu.exec(button->mapToGlobal(QPoint(0, button->height())));

        int row = 0; int col = 0; char symbol = ' ';

        if (selectedAction) {
            int row_col = button->objectName().toInt();
            row = row_col / 10;
            col = row_col % 10;}

    if (selectedAction == actionZero) {
        symbol = '0';
        button->setText("0");
    } else if (selectedAction == actionOne) {
        symbol = '1';
        button->setText("1");
    } else if (selectedAction == actionEmpty) {
        symbol = ' ';
        button->setText(" ");
    } else return;
    emit new_element(col, row, symbol);

    if (!gameboard_) return;

    if (timer->isActive() && !gameboard_->add_symbol(col, row, symbol)) {
        onInvalidMove();
        score -= 10;
    }
    if (gameboard_->is_game_over())
        onGameEnd();
}}

void GridWindow::onInvalidMove()
{
    // When the user makes an invalid move, the button will turn
    // red for a second to indicate the invalid move
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        button->setStyleSheet("background-color: red");
        QTimer::singleShot(1000, this, [button]() {
            button->setStyleSheet("background-color: white");
        });
        // make the button empty again
            button->setText(" ");
}}

void GridWindow::onGameEnd()
{
    disableOnStart();
    ui->end_info->show();
    ui->reset_button->show();

    // more info about the score calculation can be found in scoreboard window
    score = score + (score_base*size_ / seconds_);

    ui->end_info->setText("Game End\n\nTime elapsed\n" + QString::number(seconds_ / 60) +
                          " minutes " + QString::number(seconds_ % 60) +
                          " seconds\n" + "Score: " + QString::number(score));

    timer->stop();
    emit gameFinished(score, QString::number(seconds_ / 60) + " min " +
                      QString::number(seconds_ % 60) + " sec",
                      QDate::currentDate().toString("yyyy-MM-dd"), size_);

    // set of colors
    QStringList colors = {"background-color: green", "background-color: blue",
                        "background-color: red", "background-color: yellow",
                          "background-color: orange"};

    // loops through the colors and assigns it to end_info background
    for (int i = 0; i < colors.size(); ++i) {
        QTimer::singleShot(1000 * i, this, [this, colors, i]() {
            ui->end_info->setStyleSheet(colors.at(i));
        });
}}

void GridWindow::on_pause_button_clicked()
{
    // does both pause and resume functions
    if (timer->isActive()) {
        timer->stop();
        ui->pause_button->setText("Resume");

        ui->end_info->show();
        ui->end_info->setText("Game Paused");

        // Disable menu options for the buttons
        for (unsigned int i = 0; i < size_; ++i) {
            for (unsigned int j = 0; j < size_; ++j) {

                QPushButton *button;
                button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(i, j)->widget());
                button->setEnabled(false);
}}} else {
        timer->start();
        ui->pause_button->setText("Pause");

        ui->end_info->setText(" ");
        ui->end_info->hide();

        // Enable menu options for the buttons
        for (unsigned int i = 0; i < size_; ++i) {
            for (unsigned int j = 0; j < size_; ++j) {

                QPushButton *button;
                button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(i, j)->widget());

                // if button is not in the buttons_at_start vector, enable it
                if (std::find(buttons_at_start.begin(), buttons_at_start.end(), button)
                        == buttons_at_start.end()) {
                    button->setEnabled(true);
                }
}}}}

void GridWindow::on_reset_button_clicked()
{
    timer->stop();
    seconds_ = 0;
    ui->seconds->display(seconds_);
    ui->minutes->display(seconds_);

    gameboard_->recreate_board(size_);

    // Reset the buttons to the original state, leaving those
    // in buttons_at_start disabled, not removing them
    for (unsigned int i = 0; i < size_; ++i) {
        for (unsigned int j = 0; j < size_; ++j) {
            QPushButton *button;
            button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(i, j)->widget());
            if (std::find(buttons_at_start.begin(), buttons_at_start.end(), button)
                    != buttons_at_start.end()) {
                char symbol = button->text().toStdString().c_str()[0];
                gameboard_->add_symbol(j, i, symbol);
            }
        }
    }
    updateButtonValues();
    timer->start(1000);

    // if there was any change made by onGameEnd, reset it
    ui->end_info->setStyleSheet("background-color: white");
    ui->end_info->hide();
}

void GridWindow::on_timeout()
{
    seconds_++;
    ui->seconds->display(seconds_ % 60);
    ui->minutes->display(seconds_ / 60);
}

void GridWindow::disableOnStart()
{
    ui->pause_button->hide();
    ui->reset_button->hide();
    ui->done->hide();
    ui->minutes->hide();
    ui->seconds->hide();
    ui->end_info->hide();
    if(grid_state_ == INPUT)
        ui->done->show();
}

void GridWindow::on_done_button_clicked()
{
    hide();
}

void GridWindow::on_quit_button_clicked()
{
    close();
}
