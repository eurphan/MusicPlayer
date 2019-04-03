#ifndef SLIDERCLICK_H
#define SLIDERCLICK_H
#include <QSlider>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>]

class SliderClick : public QSlider
{
public:
    SliderClick(QWidget *parent = 0) : QSlider(parent) { }
protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        QSlider::mousePressEvent(ev);
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        double pos = ev->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
        //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
        QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
        QCoreApplication::sendEvent(parentWidget(), &evEvent);

    }
};


#endif // SLIDERCLICK_H
