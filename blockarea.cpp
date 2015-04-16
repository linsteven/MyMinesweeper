#include"blockarea.h"
#include<QWidget>
#include<QLayout>
#include<QGridLayout>
#include<algorithm>
#include<ctime>
#include <QDebug>
//#include<windows.h>

BlockArea::BlockArea(int row, int column, int mineCount, QWidget *parent)
    :QWidget(parent)
{
    setBlockArea(row,column,mineCount);
}

void BlockArea::setBlockArea(int row, int column, int mineCount)
{
    brow=row;
    bcolumn=column;
    bmineCount=mineCount;
    remainMine=mineCount;
    turnOverCount=0;
    //timer= new QTimer(this);
    srand(time(NULL));
    mainLayout=new QGridLayout(this);
    mainLayout->setSpacing(2);
    mainLayout->setMargin(2);
    mainLayout->setContentsMargins(10,10,10,0);

    int blockCount=row*column;
    bool *flag=new bool[blockCount];
    for(int i=0;i<mineCount;i++)
        flag[i]=true;
    for(int i=mineCount;i<blockCount;i++)
        flag[i]=false;
    std::random_shuffle(flag,flag+blockCount);

    int count=0;
    for(int i=0;i<row;i++)
        for(int j=0;j<column;j++){
            Block *block=new Block(flag[count++],calculateMine(flag,i,j));
            connect(block,SIGNAL(turnOver(int)),this,SLOT(slotTurnOver(int)));
            connect(block,SIGNAL(mineMark(bool)),this,SLOT(slotMineMark(bool)));
            connect(block,SIGNAL(setEightBlocks()),this,SLOT(slotSetEightBlocks()));
            connect(block,SIGNAL(turnOrNormal()),this,SLOT(slotTurnOrNormal()));
            mainLayout->addWidget(block,i,j);
        }
}

int BlockArea::calculateMine(bool flag[], int r,int c)
{
    int minecount=0;
    for(int i=r-1;i<r+2;i++)
        for(int j=c-1;j<c+2;j++){
            if(i<0||i>=brow||j<0||j>=bcolumn)
                continue;
            if(flag[i*bcolumn+j])
                minecount++;
        }
    return minecount;
}

void BlockArea::slotTurnOver(int flag)
{
    if(flag==0){
        turnOverMines();
        //QThread::msleep(1000);
        emit(gameOver(false));
    }else if(flag==1){
        turnOverCount++;
        if(turnOverCount==brow*bcolumn-bmineCount)
            emit gameOver(true);
        else{
            for(int i=0;i<brow;i++)
                for(int j=0;j<bcolumn;j++)
                    if(sender()==static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget()))
                        continueTurnOver(i,j);
        }
    }else if(flag==2){
        for(int i=0;i<brow;i++)
            for(int j=0;j<bcolumn;j++)
                if(sender()==static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget()))
                    dClickTurnOver(i,j);
    }
    else{//flag==3
        if(turnOverCount==0){
            //qDebug()<<"send true";
            emit startClock(true);
        }else
            emit startClock(false);
    }
}

void BlockArea::slotMineMark(bool ismark)
{
    emit blockMineMark(ismark);
}

void BlockArea::slotSetEightBlocks()
{
    int isok=0;
    for(int i=0;i<brow;i++){
        for(int j=0;j<bcolumn;j++){
            if(sender()==static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget())){
                for(int r=i-1;r<=i+1;r++)
                    for(int c=j-1;c<=j+1;c++)
                        if(r>=0&&r<brow&&c>=0&&c<bcolumn)
                            static_cast<Block*>(mainLayout->itemAtPosition(r,c)->widget())->setBlockMine_0();
                isok=0;
                break;
            }
        }
        if(isok)break;
    }//end for
}

void BlockArea::slotTurnOrNormal()
{
    int r,c;
    for(int i=0,isok=0;i<brow;i++){
        for(int j=0;j<bcolumn;j++){
            if(sender()==static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget())){
                r=i;c=j;
                isok=1;
                break;
            }
        }
        if(isok)break;
    }//end for i
    Block *block=static_cast<Block*>(mainLayout->itemAtPosition(r,c)->widget());
    for(int i=r-1;i<=r+1;i++)
        for(int j=c-1;j<=c+1;j++){
            if(i>=0&&i<brow&&j>=0&&j<bcolumn)
                static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget())->setBlockNormal();
        }
    if(block->isTurnOver())//turn over more blocks
        dClickTurnOver(r,c);
}
/*
void BlockArea::showOneBomb()
{
    std::pair<int,int>cur=bombs.front();
    bombs.pop_front();
    static_cast<Block*>(mainLayout->itemAtPosition(cur.first,cur.second)->widget())->setBlockBomb();
    if(bombs.isEmpty())
        timer->stop();
}
*/

void BlockArea::continueTurnOver(int r, int c)
{
    //qDebug()<<"Hello,Qt!";
    if(static_cast<Block*>(mainLayout->itemAtPosition(r,c)->widget())->getMineCount()==0){
        QQueue<std::pair<int,int> >queue;
        queue.push_back(std::make_pair(r,c));
        while(!queue.empty()){
            std::pair<int,int>cur=queue.front();
            queue.pop_front();
            for(int i=cur.first-1;i<=cur.first+1;i++)
                for(int j=cur.second-1;j<=cur.second+1;j++)
                    if(i>=0&&i<brow&&j>=0&&j<bcolumn){
                        Block *block=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
                        if(!block->isTurnOver()&&!block->isMark()){
                            block->turnOver();
                            turnOverCount++;
                            if(block->getMineCount()==0)
                                queue.push_back(std::make_pair(i,j));
                        }
                    }
        }//end while
        if(turnOverCount==brow*bcolumn-bmineCount)
            emit gameOver(true);
    }//end if
}

void BlockArea::dClickTurnOver(int r, int c)
{
    bool canTurn=true;
    for(int i=r-1;i<=r+1;i++){
        for(int j=c-1;j<=c+1;j++){
            if(i>=0&&i<brow&&j>=0&&j<bcolumn){
                Block *block=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
                if((block->isMine()&&!block->isMark())||(!block->isMine()&&block->isMark())){//雷没被标记||非雷被标记
                    canTurn=false;
                    break;
                }
            }
        }
        if(!canTurn)break;
    }//end for
    QQueue<std::pair<int,int> >queue;
    if(canTurn){
        for(int i=r-1;i<=r+1;i++)
            for(int j=c-1;j<=c+1;j++)
                if(i>=0&&i<brow&&j>=0&&j<bcolumn){
                    Block *block=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
                    if(!block->isMine()&&!block->isTurnOver()){
                        block->turnOver();
                        turnOverCount++;
                        if(block->getMineCount()==0)
                            queue.push_back(std::make_pair(i,j));
                    }
                }
    }
    if(turnOverCount==brow*bcolumn-bmineCount){
        emit gameOver(true);
        return ;
    }
    while(!queue.empty()){
        std::pair<int,int>cur=queue.front();
        queue.pop_front();
        continueTurnOver(cur.first,cur.second);
        if(turnOverCount==brow*bcolumn-bmineCount)
            break;
    }
    while(!queue.empty())
        queue.pop_front();
}

void BlockArea::turnOverMines()
{
    for(int i=0;i<brow;i++){
        for(int j=0;j<bcolumn;j++){
            Block *block=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
            if(!block->isMark() && block->isMine()){
                //bombs.push_back(std::make_pair(i,j));
                block->setBlockBomb();  //设置延时失败！！！
            }
        }
    }
    //QThread::msleep(100);
  //  if(!bombs.isEmpty()){
 //       timer->start(100);
   //     connect(timer,SIGNAL(timeout()),this,SLOT(showOneBomb()));
  //  }
}
