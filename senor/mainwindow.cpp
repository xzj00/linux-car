#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QProcess>

QProcess * mypro;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();
}

MainWindow::~MainWindow()
{
}

/* 布局初始化 */
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

    /* 实例化与布局，常规操作 */
    mainWidget = new QWidget();
    rightWidget = new QWidget();

    for(int i = 0; i < 2; i++)          //水平布局
        hboxLayout[i] = new QHBoxLayout();

    for(int i = 0; i < 5; i++)          //容器布局
        widget[i] = new QWidget();

    for(int i = 0; i < 4; i++)          //垂直布局
        vboxLayout[i] = new QVBoxLayout();

    for(int i = 0; i < 3; i++)          //数据变量
        my_label[i] = new QLabel();


/********************** chart表格 *****************************/
    /* 最大储存maxSize - 1个数据 */
    maxSize = 51;
    /* x轴上的最大值 */
    maxX = 5000;
    /* y轴最大值 */
    maxY = 40;

    /* splineSeries曲线实例化（折线用QLineSeries） */
    splineSeries = new QSplineSeries();
    /* 图表实例化 */
    chart = new QChart();
    /* 图表视图实例化 */
    chartView = new QChartView();

    /* 坐标轴 */
    axisY = new QValueAxis();
    axisX = new QValueAxis();

    /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
    chart->legend()->hide();
    /* chart设置标题 */
    chart->setTitle("环境光强度");
    /* 添加一条曲线splineSeries */
    chart->addSeries(splineSeries);

    /* 设置显示格式 */
    axisY->setLabelFormat("%i");
    /* y轴标题 */
    axisY->setTitleText("光通亮/Lux");
    /* y轴标题位置（设置坐标轴的方向） */
    chart->addAxis(axisY, Qt::AlignLeft);
    /* 设置y轴范围 */
    axisY->setRange(0, maxY);
    /* 将splineSeries附加于y轴上 */
    splineSeries->attachAxis(axisY);

    /* 设置显示格式 */
    axisX->setLabelFormat("%i");
    /* x轴标题 */
    axisX->setTitleText("时间/ms");
    /* x轴标题位置（设置坐标轴的方向） */
    chart->addAxis(axisX, Qt::AlignBottom);
    /* 设置x轴范围 */
    axisX->setRange(0, maxX);
    /* 将splineSeries附加于x轴上 */
    splineSeries->attachAxis(axisX);

    /* 将图表的内容设置在图表视图上 */
    chartView->setChart(chart);
    /* 设置抗锯齿 */
    chartView->setRenderHint(QPainter::Antialiasing);
/********************** chart表格 *****************************/

    /* 设置传感器数据标签 */
    QFont font;
    font.setPixelSize(18);

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);

    QStringList list;
    list<<"环境光强度："<<"接近距离："<<"红外强度：";
    for (int i = 0; i < 3; i++) {
        label[i] = new QLabel();
        label[i]->setText(list[i]);
        label[i]->setFont(font);
        label[i]->setPalette(pal);
        label[i]->adjustSize();
    }

    /* 垂直容器布局 */
    vboxLayout[3]->addWidget(widget[0]);
    vboxLayout[3]->addWidget(widget[1]);
    vboxLayout[3]->addWidget(widget[2]);

    hboxLayout[0]->addWidget(chartView);        //将chart表格添加到水平布局中

    widget[3]->setLayout(hboxLayout[0]);
    widget[4]->setLayout(vboxLayout[3]);

    hboxLayout[1]->addWidget(widget[3]);
    hboxLayout[1]->addWidget(widget[4]);

    mainWidget->setLayout(hboxLayout[1]);

    this->setCentralWidget(mainWidget);

    /* als布局 */
    vboxLayout[0]->addWidget(label[0]);
    vboxLayout[0]->addWidget(my_label[0]);
    vboxLayout[0]->setAlignment(Qt::AlignTop | Qt::AlignVCenter);
    widget[0]->setLayout(vboxLayout[0]);

    /* ps布局 */
    vboxLayout[1]->addWidget(label[1]);
    vboxLayout[1]->addWidget(my_label[1]);
    vboxLayout[1]->setAlignment(Qt::AlignTop | Qt::AlignVCenter);
    widget[1]->setLayout(vboxLayout[1]);

    /* ir布局 */
    vboxLayout[2]->addWidget(label[2]);
    vboxLayout[2]->addWidget(my_label[2]);
    vboxLayout[2]->setAlignment(Qt::AlignTop | Qt::AlignVCenter);
    widget[2]->setLayout(vboxLayout[2]);

    /* 退出按钮 */
    exit_button = new QPushButton(this);
    exit_button->setMinimumSize(60, 50);
    exit_button->setMaximumSize(60, 50);
    exit_button->move(740,350);
    exit_button->setStyleSheet("QPushButton{background: green}");

    connect(exit_button,&QPushButton::clicked,[=](){
        mypro->close();
        exit(1);//退出程序
    });

    ap3216c = new Ap3216c(this);

    /* 只能在开发板上开启获取数据，Ubuntu上是没有ap3216c传感器的 */
#if __arm__
    ap3216c->setCapture(true);
#endif

    connect(ap3216c,SIGNAL(ap3216cDataChanged()),this,SLOT(getAp3216cData()));
}

void MainWindow::getAp3216cData()
{
    static QString als = ap3216c->alsData();
//    if (als != ap3216c->alsData()) {
//        als = ap3216c->alsData();
//    }
    als = ap3216c->alsData();

    static QString ps = ap3216c->psData();
//    if (ps != ap3216c->psData()) {
//        ps = ap3216c->psData();
//    }

    ps = ap3216c->psData();

    static QString ir = ap3216c->irData();
//    if (ir != ap3216c->irData()) {
//        ir = ap3216c->irData();
//    }

    ir = ap3216c->irData();

    my_label[0]->setText(als);
    my_label[1]->setText(ps);
    my_label[2]->setText(ir);


/**************** chart表数据 ******************/

    int num = als.toInt();      //字符串转int类型数据

    /* 将数据添加到data中 */
    data.append(num);

    /* 当储存数据的个数大于最大值时，把第一个数据删除 */
    while (data.size() > maxSize) {
        /* 移除data中第一个数据 */
        data.removeFirst();
    }

    /* 先清空 */
    splineSeries->clear();

    /* 计算x轴上的点与点之间显示的间距 */
    int xSpace = maxX / (maxSize - 1);

    /* 添加点，xSpace * i 表示第i个点的x轴的位置 */
    for (int i = 0; i < data.size(); ++i) {
        splineSeries->append(xSpace * i, data.at(i));
    }

}
