#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include<QWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString title=QString::fromLocal8Bit("扫雷");
    setWindowTitle(title);
    setWindowIcon(QIcon(QPixmap(":/images/100.ico")));
    setMapSize(9,9,10);
    gameLevel=1; //1 low 2 mid 3 high
    ui->lowAction->setChecked(true);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);//禁用最大化按钮
    connect(ui->newAction,SIGNAL(triggered()),this,SLOT(newGame()));
    blockArea=new BlockArea(mainRow,mainColumn,mainMineCount);
    setCentralWidget(blockArea);
    connect(blockArea,SIGNAL(gameOver(bool)),this,SLOT(slotGameOver(bool)));
    connect(blockArea,SIGNAL(blockMineMark(bool)),this,SLOT(slotBlockMineMark(bool)));
    connect(blockArea,SIGNAL(startClock(bool)),this,SLOT(slotStartClock(bool)));
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    //connect(this,SIGNAL(newTwiceGame()),this,SLOT(newGame()));
    connect(ui->lowAction,SIGNAL(triggered()),this,SLOT(lowGame()));
    connect(ui->midAction,SIGNAL(triggered()),this,SLOT(midGame()));
    connect(ui->highAction,SIGNAL(triggered()),this,SLOT(highGame()));
    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->guideAction,SIGNAL(triggered()),this,SLOT(guide()));
    connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->rankAction,SIGNAL(triggered()),this,SLOT(rank()));
    QHBoxLayout *statusLayout=new QHBoxLayout(this);
    QLabel *clockIcon=new QLabel;
    clockIcon->setPixmap(QPixmap(":/images/clock.png"));
    timeLabel=new QLabel("0");
    timeLabel->setFixedWidth(23);
    //timeLabel->
    mineLabel=new QLabel(QString::number(mainMineCount,10));
    mineLabel->setFixedWidth(23);
    QLabel *mineIcon=new QLabel;
    mineIcon->setPixmap(QPixmap(":/images/mine.png"));
    //int addWidth=(column-6)*16;
    QSpacerItem *spacerItem= new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    statusLayout->addWidget(clockIcon);
    statusLayout->addWidget(timeLabel);
    statusLayout->addItem(spacerItem);
    statusLayout->addWidget(mineIcon);
    statusLayout->addWidget(mineLabel);
    //statusLayout->addStretch(30);
    QWidget *statusWidget=new QWidget();
    statusWidget->setLayout(statusLayout);
    ui->statusBar->addWidget(statusWidget);
   //timer.start();
    //this->setFixedSize(180,232);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMapSize(int row, int column, int mineCount)
{
    mainRow=row;
    mainColumn=column;
    mainMineCount=mineCount;
    remainMineCount=mineCount;
}

void MainWindow::newGame()
{
    blockArea=new BlockArea(mainRow,mainColumn,mainMineCount);
    //QPushButton *tmpButton=new QPushButton("hello");
    //tmpButton->setFixedSize(50,50);
    //setCentralWidget(tmpButton);
    //this->setFixedSize(300,300);
    //resize( QSize(100,100));
    if(gameLevel==1)
        this->setFixedSize(180,232);
    else if(gameLevel==2)
        this->setFixedSize(307,359);
    else
        this->setFixedSize(558,360);
    setCentralWidget(blockArea);
    connect(blockArea,SIGNAL(gameOver(bool)),this,SLOT(slotGameOver(bool)));
    connect(blockArea,SIGNAL(blockMineMark(bool)),this,SLOT(slotBlockMineMark(bool)));
    connect(blockArea,SIGNAL(startClock(bool)),this,SLOT(slotStartClock(bool)));
    setMapSize(mainRow,mainColumn,mainMineCount);
    if(timer->isActive())
        timer->stop();
    mineLabel->setText(QString::number(remainMineCount,10));
    timeLabel->setText("0");
    //connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));


//    QPalette palette=mineLabel->palette();  //set mineLabel's color failed
//    palette.setColor(QPalette::Window, Qt::lightGray);
//    mineLabel->setPalette(palette);
}

void MainWindow::slotGameOver(bool isWin)
{
    QString str1=QString::fromLocal8Bit("恭喜你赢啦！");
    QString str2=QString::fromLocal8Bit("抱歉,你输了！");
    if(isWin){
        timer->stop();
        QMessageBox::information(NULL, "isWin", str1, QMessageBox::Yes);
        newGame();
    }else{
        timer->stop();
        QMessageBox::warning(this,"isWIn",str2,QMessageBox::Yes);
        newGame();
    }
}

void MainWindow::lowGame()
{
    ui->midAction->setChecked(false);
    ui->highAction->setChecked(false);
    ui->lowAction->setChecked(true);
    setMapSize(9,9,10);
    gameLevel=1;
    //this->setFixedSize(196,271);
    newGame();

    //emit newTwiceGame();
}


void MainWindow::midGame()
{
    ui->lowAction->setChecked(false);
    ui->highAction->setChecked(false);
    ui->midAction->setChecked(true);
    setMapSize(16,16,40);
    gameLevel=2;
    //this->setFixedSize(322,397);
    newGame();
}

void MainWindow::highGame()
{
    ui->lowAction->setChecked(false);
    ui->midAction->setChecked(false);
    ui->highAction->setChecked(true);
    setMapSize(16,30,99);
    gameLevel=3;
    //this->setFixedSize(574,397);
    newGame();
}

void MainWindow::slotBlockMineMark(bool ismark)
{
    //qDebug("Time elapsed: %d ms", timer.elapsed());
    if(ismark)remainMineCount--;
    else remainMineCount++;
    mineLabel->setText(QString::number(remainMineCount,10));
}

void MainWindow::slotStartClock(bool start)
{
    if(start){
        time.start();
        timer->start(1000);
    }
}

void MainWindow::showTime()
{
    int t=time.elapsed()/1000;
    timeLabel->setText(QString::number(t,10));
}

void MainWindow::guide()
{
    QString str1=QString::fromLocal8Bit("新手指南");
    QString str2=QString::fromLocal8Bit("请Google一下或者找度娘");
    QString guideStr(
                "<h2>"+str1+"</h2>"
                +"<p>"+str2+"</p>"
            );
    QMessageBox messageBox(QMessageBox::Information,str1,guideStr,QMessageBox::Ok);
    messageBox.exec();
}

void MainWindow::about()
{
    QString str1=QString::fromLocal8Bit("关于扫雷");
    QString str2=QString::fromLocal8Bit("这是一个简单的扫雷程序，纯属个人兴趣开发。");
    QString str3=QString::fromLocal8Bit("如有问题，请联系我");
    QString str4=QString::fromLocal8Bit("Email: smango123@gmail.com");
    QString introduction(
        "<h2>"+str1+"</h2>"
        +"<p>"+str2+"</p>"
                +"<p>"+str3+"</p>"
                +"<p>"+str4+"</p>"
        +"<br>"+tr("Version ")+MyMinesweeperVersion+"</br>"
        +"<br>"+MyMinesweeperAuthor+"</br>"
    );
    QMessageBox messageBox(QMessageBox::Information,str1,introduction,QMessageBox::Ok);
    messageBox.exec();
}

void MainWindow::rank()
{
    QString str=QString::fromLocal8Bit("该功能待完善,敬请期待!");
    QString introduction(
                "<h2>"+str+"</h2>"
            );
    QMessageBox messageBox(QMessageBox::Information,tr("RankList"),introduction,QMessageBox::Ok);
    messageBox.exec();
}



