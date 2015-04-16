#ifndef BLOCK_H
#define BLOCK_H

#include<QLabel>
class QWidget;

class Block:public QLabel
{
    Q_OBJECT

public:
    explicit Block(bool mineflag,int minecount,QWidget* parent=0);
    void setNearMineCount(int);
    bool isMine()const;
    bool isTurnOver()const;
    bool isMark()const;
    void continueTurnOver();
    int getMineCount();
    void turnOver();
    bool canChange();
    void setBlockMine_0();
    void setBlockNormal();
    void setBlockBomb(); //change image

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void turnOver(int flag);
    void mineMark(bool ismark);
    void setEightBlocks();
    void turnOrNormal();//turn over more blocks or setPixmap normal.png

private:
    int nearMineCount;
    bool mineFlag;
    bool turnOverFlag;
    bool markFlag;

    bool leftPress;
    bool rightPress;
    bool doublePressFlag;
};


#endif // BLOCK_H
