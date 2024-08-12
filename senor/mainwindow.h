#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QDebug>
#include <QValueAxis>
#include <QPushButton>
#include "ap3216c.h"

/*  必需添加命名空间 */
QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 主容器，Widget也可以当作一种容器 */
    QWidget *mainWidget;

    /* 界面水平区域布局 */
    QHBoxLayout *hboxLayout[2];

    /* 界面右侧区域布局 */
    QVBoxLayout *vboxLayout[4];

    /* 界面右侧区域容器 */
    QWidget *rightWidget;

    /* 用一个 QLabel 对象用于显示字符串 */
    QLabel *labelString;

    /* 容器作用，用于布局 */
    QWidget *widget[5];

    /* 标签文本 */
    QLabel *label[3];

    /* 数据标签 */
    QLabel *my_label[3];

    /* 数据最大个数 */
    int maxSize;

    /* x轴上的最大值 */
    int maxX;

    /* y轴上的最大值 */
    int maxY;

    /* y轴 */
    QValueAxis *axisY;

    /* x轴 */
    QValueAxis *axisX;

    /* QList int类型容器 */
    QList<int> data;

    /* QSplineSeries对象（曲线）*/
    QSplineSeries *splineSeries;

    /* QChart图表 */
    QChart *chart;

    /* 图表视图 */
    QChartView *chartView;

    /* ii2传感器类 */
    Ap3216c *ap3216c;

    /* 退出进程按钮 */
    QPushButton *exit_button;

    /* 接收数据接口 */
    void receivedData(int);

    /* 布局初始化 */
    void layoutInit();


private slots:
    /* 获取ap3216传感器数据 */
    void getAp3216cData();
};
#endif // MAINWINDOW_H
