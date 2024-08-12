#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QTimer>
#include<QTime>
#include<QProcess>
#include "menubutton.h"
#include <QScreen>
#include<QFont>

/*
 * 作者：混分巨兽龙某某
 * csdn：混分巨兽龙某某
 * 邮箱：1178305328@qq.com
 * 嵌入式技术交流群：958820627
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();
}

/* 析构函数 */
MainWindow::~MainWindow()
{

}

/* 屏幕布局初始化 */
void MainWindow::layoutInit()
{
    /* 获取屏幕的分辨率，Qt官方建议使用这
    * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
    * 注意，这是获取整个桌面系统的分辨率
    */
    QList <QScreen *> list_screen = QGuiApplication::screens();

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
     /* 重设大小 */
     this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
     /* 否则则设置主窗体大小为800x480 */
     this->resize(800,400);
#endif

    /* 构建布局标题和背景色 */
    this->setWindowTitle("智能车载系统");
    this->setStyleSheet("background-color: rgb(240, 255, 255);");   //QT颜色标：http://t.csdn.cn/SF7Rc

    //给进程分配空间
    my_pro = new QProcess(this);

    //时间显示label
    QLabel *time_label = new QLabel(this);
    time_label->setGeometry(10,20,240,100);//设置坐标
    //time_label->setStyleSheet("font-size:55px;color:black");//设置大小颜色
    time_label->setFont(QFont("Helvetica", 72));
    //time_label->ssetFont(QFont("Helvetica", 30)); // 设置字体为Arial，大小为16

    //日期显示label
    QLabel *date_label = new QLabel(this);
    date_label->setGeometry(200,100,240,100);//设置坐标
    date_label->setStyleSheet("font-size:25px;color:green");//设置大小颜色

    //智能车载装置
    QFont font("Arial", 30, QFont::Normal);
    QLabel *title_label = new QLabel(this);
    title_label->setGeometry(40,180,260,50);//设置坐标
    title_label->setFont(font);
    title_label->setText("智能车载终端");

    m_pLCD = new QLCDNumber(this);
    // 设置能显示的位数
    m_pLCD->setDigitCount(8);

    m_pLCD->setGeometry(20,30,230,100);
    // 设置显示的模式为十进制
    m_pLCD->setMode(QLCDNumber::Dec);
    // 设置样式
    m_pLCD->setStyleSheet("border: 1px solid green;font-size:70%;color: green");
    m_pLCD->resize(300,100);

    //定时器刷新显示
    QTimer *timer = new QTimer(this);
    timer->start(200);


    //QTimer *pTimer = new QTimer(this);
    // 设置定时间隔
    //pTimer->setInterval(1000);
    //connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    // 启动定时器
    //pTimer->start();

    connect(timer,&QTimer::timeout,[=](){
        /* 获取当前日期 */
        QDate date = QDate::currentDate();//获取当前日期
        QString date_msg = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
        date_label->setText(date_msg);

        // 获取系统当前时间
        QDateTime dateTime = QDateTime::currentDateTime();
        // 显示的内容
        m_pLCD->display(dateTime.toString("HH:mm:ss"));


        /* 判断进程状态 */
        if(my_pro->state() == QProcess::NotRunning)
        {
            this->show();//重新显示窗口
        }
        else
        {
            this->hide();
        }

    });

    /* 菜单按钮切屏定时器 */
    time1= new QTimer(this);

    //音乐按钮
    MenuButton * music_button=new MenuButton(":/picture/music.png","",245,245);
    music_button->setParent(this);
    music_button->move(400,10);
    //音乐按钮按下处理
    connect(music_button,&MenuButton::clicked,[=](){
        /* 设计动作图标 */
        music_button->zoom1();//弹跳
        music_button->zoom2();
        /* 延迟500ms后启动播放器进程 */
        time1->start(500);
        connect(time1,&QTimer::timeout,[=](){
            time1->stop();
            my_pro->close();
            my_pro->start("./QMusicPlayer");});
        });


    //视频按钮
    MenuButton * video_button=new MenuButton(":/picture/bofangqi.png","",215,215);
    //time1= new QTimer(this);
    video_button->setParent(this);
    video_button->move(710,20);
    //视频按钮按下处理
    connect(video_button,&MenuButton::clicked,[=](){
        video_button->zoom1();//弹跳
        video_button->zoom2();
        time1->start(500);//定时500ms
        connect(time1,&QTimer::timeout,[=](){
            time1->stop(); //关闭定时器
            my_pro->close();
            my_pro->start("./QVideo");   });
        });

    //家居按钮
    MenuButton * sensor_button=new MenuButton(":/picture/sensor.png","",245,245);
    //time1= new QTimer(this);
    sensor_button->setParent(this);
    sensor_button->move(45,300);
    //硬件数据按下处理
    connect(sensor_button,&MenuButton::clicked,[=](){
        sensor_button->zoom1();//弹跳
        sensor_button->zoom2();
        time1->start(500);
        connect(time1,&QTimer::timeout,[=](){
            time1->stop();
            my_pro->close();
            my_pro->start("./senor");});
        });

    //地图按钮
    MenuButton * map_button=new MenuButton(":/picture/map.png","",240,245);
    map_button->setParent(this);
    map_button->move(400,300);

    //天气按钮
    MenuButton * weather_button=new MenuButton(":/picture/weather.png","",245,245);
    weather_button->setParent(this);
    weather_button->move(700,300);

    //进程结束处理
    connect(my_pro,&QProcess::stateChanged,[=](){

    });
}

//void MainWindow::onTimeOut()
//{
//    // 获取系统当前时间
//    QDateTime dateTime = QDateTime::currentDateTime();
//    // 显示的内容
//    m_pLCD->display(dateTime.toString("HH:mm:ss"));
//}





