#ifndef BLOCKAREA_H
#define BLOCKAREA_H

#include"block.h"
#include<QtWidgets>
#include<QQueue>

#include<QPair>
class QGridLayout;
class QObject;

class BlockArea:public QWidget
{
    Q_OBJECT
public:
    BlockArea(int row,int column,int mineCount,QWidget* parent=0);
    void setBlockArea(int row,int column,int mineCount);//int initFlag=false
    int calculateMine(bool flag[],int r,int c);
    void continueTurnOver(int r,int c);
    void dClickTurnOver(int r,int c);
    void turnOverMines();

signals:
    void gameOver(bool isWin);
    void blockMineMark(bool ismark);
    void startClock(bool start);

private slots:
    void slotTurnOver(int flag);
    void slotMineMark(bool ismark);
    void slotSetEightBlocks();
    void slotTurnOrNormal();
    //void showOneBomb();

private:
    QGridLayout* mainLayout;

    int brow; //blockArea's row
    int bcolumn;
    int bmineCount;
    int remainMine; //remaining mine blocks
    int turnOverCount;

    //QQueue<std::pair<int,int>>bombs;
    //QTimer *timer ;
};

#endif // BLOCKAREA_H
