#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"blockarea.h"
#include"block.h"
class QWidget;

const QString MyMinesweeperVersion="1.0";
const QString MyMinesweeperAuthor="Copyright© 2014 Smango. All rights reserved.";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setMapSize(int row,int column,int mineCount);

//protected:
    //void closeEvent(QCloseEvent *event);

private slots:
    void newGame();
    void slotGameOver(bool isWin);
    void lowGame();
    void midGame();
    void highGame();
    void slotBlockMineMark(bool ismark);
    void slotStartClock(bool start);
    void showTime();
    void guide();
    void about();
    void rank();

//signals:
    //void newTwiceGame();

private:
    Ui::MainWindow *ui;
    BlockArea *blockArea;
    QLabel *timeLabel;
    QLabel *mineLabel;
    int mainRow;
    int mainColumn;
    int mainMineCount;
    int remainMineCount;
    int gameLevel;

    QTime time;
    QTimer *timer;
  //  bool restart;//restart time

};

#endif // MAINWINDOW_H
