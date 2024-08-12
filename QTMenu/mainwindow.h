#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QProcess>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * 作者：混分巨兽龙某某
 * csdn：混分巨兽龙某某
 * 邮箱：1178305328@qq.com
 * 嵌入式技术交流群：958820627
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLCDNumber *m_pLCD;
    QProcess *my_pro;
    QTimer *time1;

    /* 布局初始化 */
    void layoutInit();

private slots:
    //void onTimeOut();
};
#endif // MAINWINDOW_H
