#include"block.h"
#include<QPixmap>
#include<QMouseEvent>
#include<QtWidgets>

Block::Block(bool mineflag,int minecount,QWidget* parent)
    :QLabel(parent)
{
    mineFlag=mineflag;
    nearMineCount=minecount;
    markFlag=false;
    turnOverFlag=false;
    leftPress=false;
    rightPress=false;
    doublePressFlag=false;
    setPixmap(QPixmap(":/images/normal.png"));
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setScaledContents(true);
    //setHorizontalStretch(1);
    //setVerticalStretch(1);
    //setSizePolicy(QSizePolicy::setHorizontalStretch(1));
    //setSizePolicy(QSizePolicy::setVerticalStretch(1));
    //setSizePolicy(hasHeightForWidth());
    //QSizePolicy::setHeightForWidth(true);
}

void Block::setNearMineCount(int count)
{
    nearMineCount=count;
}

bool Block::isMine()const
{
    return mineFlag;
}

bool Block::isMark()const
{
    return markFlag;
}

bool Block::isTurnOver()const
{
    return turnOverFlag;
}

int Block::getMineCount()
{
    return nearMineCount;
}

void Block::turnOver()
{
    if(!markFlag&&!turnOverFlag){
        setPixmap(QPixmap(":/images/mine_"+QString::number(nearMineCount,10)+".png"));
        turnOverFlag=true;
    }
}

bool Block::canChange()
{
    //this function is to judge if the block could be setPixmap mine_0 or be turned over
    if(!turnOverFlag && !markFlag)
        return true;
    return false;
}

void Block::setBlockMine_0()
{
    if(canChange()){
        setPixmap(QPixmap(":/images/mine_0.png"));
        update();
    }
}

void Block::setBlockNormal()
{
    if(canChange()){
        setPixmap(QPixmap(":/images/normal.png"));
        update();
    }
}

void Block::setBlockBomb()
{
    setPixmap(QPixmap(":/images/bomb.png"));
    update();
    //QThread::msleep(100);
}

void Block::mousePressEvent(QMouseEvent *event)
{
    emit turnOver(3);//use to start time
    if(event->button()==Qt::LeftButton){
        leftPress=true;
        if(!rightPress ){
            setBlockMine_0();
        }else{
            emit setEightBlocks();//set mine_0
            doublePressFlag=true;
        }
    }else{
        rightPress=true;
        if(leftPress){
            emit setEightBlocks();
            doublePressFlag=true;
        }
        else{
            if(!turnOverFlag){
                if(!markFlag){
                    setPixmap(QPixmap(":/images/flag.png"));
                    markFlag=true;
                    emit mineMark(true);
                }
                else{
                    setPixmap(QPixmap(":/images/normal.png"));
                    markFlag=false;
                    emit mineMark(false);
                }
                update();
            }
        }
    }
}

void Block::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(turnOverFlag&&nearMineCount>0){
        emit turnOver(2);
    }
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        leftPress=false;
        if(rightPress){
            emit turnOrNormal();
        }else{// rightPress=false
            if(!doublePressFlag){
                if(canChange()){
                    if(mineFlag){
                        setPixmap(QPixmap(":/images/bomb.png"));
                        update();
                        //QThread::msleep(100);
                        emit turnOver(0);
                    }
                    else{
                        setPixmap(QPixmap(":/images/mine_"+QString::number(nearMineCount,10)+".png"));
                        turnOverFlag=true;
                        update();
                        emit turnOver(1);
                    }
                }
            }else
                doublePressFlag=false;
        }
    }else{
        rightPress=false;
        if(leftPress){
            emit turnOrNormal();
        }else{
            if(doublePressFlag)
                doublePressFlag=false;
        }
    }
}
