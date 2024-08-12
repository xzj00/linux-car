#ifndef AP3216C_H
#define AP3216C_H

#include <QObject>
#include <QTimer>

class Ap3216c : public QObject
{
    Q_OBJECT

public:
    explicit Ap3216c(QObject *parent = 0);     //构造函数声明,"explicit"是一个C++关键字，用于修饰构造函数或函数，以禁止隐式转换
    ~Ap3216c();                                 //析构函数

    Q_INVOKABLE void setCapture(bool str);      //Q_INVOKABLE是一个宏，setCapture函数接受一个bool类型的参数str，用于设置某个对象的捕获状态

    QString alsData();      //光强传感器（ ALS: Ambient Light Sensor）
    QString psData();       //接近传感器（ PS: Proximity Sensor）
    QString irData();       //红外 LED（ IR LED）

private:
    QTimer *timer;          //定时器timer
    QString alsdata;        //光强数据
    QString psdata;         //接近数据
    QString irdata;         //红外数据

    QString readAlsData();  //读取Ais数据
    QString readPsData();   //读取Ps数据
    QString readIrData();   //读取IR数据

    Q_PROPERTY(QString alsData READ alsData NOTIFY ap3216cDataChanged)
    Q_PROPERTY(QString psData READ psData NOTIFY ap3216cDataChanged)
    Q_PROPERTY(QString irData READ irData NOTIFY ap3216cDataChanged)
    /*Q_PROPERTY：这是一个宏，用于在类中声明属性。
     *QString：alsData是属性的类型，这里是一个QString类型的变量。
     *READ alsData：这表示该属性可以通过"alsData"方法进行读取。
     *NOTIFY ap3216cDataChanged：这表示当该属性发生变化时，会发出"ap3216cDataChanged"信号
     */

public slots:
    void timer_timeout();

signals:
    void ap3216cDataChanged();
};

#endif // AP3216C_H
