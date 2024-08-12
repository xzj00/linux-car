#include "ap3216c.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>

/* AP3216C的构造函数 */
Ap3216c::Ap3216c(QObject *parent) : QObject (parent)
{
    timer = new QTimer();                                            //定义一个定时器容器
    connect(timer, SIGNAL(timeout()),this,SLOT(timer_timeout()));     //将超时信号，与timer_timeout链接起来
}

/* 析构函数 */
Ap3216c::~Ap3216c()
{

}

/* 超时函数 */
void Ap3216c::timer_timeout()
{
    alsdata = readAlsData();
    psdata = readPsData();
    irdata = readIrData();
    emit ap3216cDataChanged();      //发送ap3216c数值变化信息
}

QString Ap3216c::readIrData()
{
    char const *filename = "/sys/class/misc/ap3216c/ir";
    int err = 0;
    int fd;
    char buf[10];

    fd = open(filename,O_RDONLY);      //只读模式打开文件
    if(fd < 0){
        close(fd);
        return "open file error!";
    }

    err = read(fd,buf,sizeof(buf));
    if(err < 0){
        close(fd);
        return "read data error!";
    }
    close(fd);

    QString irValue = buf;
    QStringList list = irValue.split("\n");     //将irValue按照换行符"\n"分割成一个字符串列表，并将结果存储在list对象中
    return list[0];
}

QString Ap3216c::readPsData()
{
    char const *filename = "/sys/class/misc/ap3216c/ps";
    int err = 0;
    int fd;
    char buf[10];

    fd = open(filename,O_RDONLY);       //只读模式打开文件
    if(fd < 0){
        close(fd);
        return "open file error!";
    }

    err = read(fd,buf,sizeof(buf));
    if(err < 0){
        close(fd);
        return "read data error!";
    }
    close(fd);

    QString irValue = buf;
    QStringList list = irValue.split("\n");     //将irValue按照换行符"\n"分割成一个字符串列表，并将结果存储在list对象中
    return list[0];
}

QString Ap3216c::readAlsData()
{
    char const *filename = "/sys/class/misc/ap3216c/als";
    int err = 0;
    int fd;
    char buf[10];

    fd = open(filename,O_RDONLY);       //只读模式打开文件
    if(fd < 0){
        close(fd);
        return "open file error!";
    }

    err = read(fd,buf,sizeof(buf));
    if(err < 0){
        close(fd);
        return "read data error!";
    }
    close(fd);

    QString irValue = buf;
    QStringList list = irValue.split("\n");     //将irValue按照换行符"\n"分割成一个字符串列表，并将结果存储在list对象中
    return list[0];
}

QString Ap3216c::alsData()
{
    return alsdata;
}

QString Ap3216c::irData()
{
    return irdata;
}

QString Ap3216c::psData()
{
    return psdata;
}

void Ap3216c::setCapture(bool str)
{
    if(str)
        timer->start(500);
    else
        timer->stop();
}
