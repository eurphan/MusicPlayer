#include "window.h"
#include "mplayercall.h"
#include <QDebug>
#include <QApplication>
#include <QFileDialog>
#include <skindialog.h>
#include <QPainter>
#include <QPixmap>

Window::Window(QWidget *parent)
    : QDialog(parent)
{
    //init window
    _initWindow();

    _playPauseMode = Pause;
    _timePlayed = 0;
    _timeTotol = 0;

    //connect the signals and slots
    connect(_pushButtonCycleMode, SIGNAL(clicked()), this, SLOT(_slotPushButtonCycleMode()));
    connect(_pushButtonPlayPause, SIGNAL(clicked()), this, SLOT(_slotPushButtonPlayPause()));
    //connect the signals and signals
    connect(_pushButtonNext, SIGNAL(clicked()), this, SIGNAL(nextSignal()));
    connect(_pushButtonPrev, SIGNAL(clicked()), this, SIGNAL(prevSignal()));
    connect(_volumeSlider, &SliderClick::valueChanged, this, &Window::volumeSignal);
    connect(_pushButtonSkin, SIGNAL(clicked()), this, SLOT(_slotPushButtonSkin()));
    connect(_musicList, &QListWidget::itemSelectionChanged, this, &Window::_slotMusicListClicked);
    connect(_playProgressSlider, &SliderClick::valueChanged, this, &Window::playedPercentChangedSignal);
}

/************************************************************************************************
 * init the main window
 * *********************************************************************************************/
void Window::_initWindow(void)
{
    //set the window size
    this->resize(WINDOW_X_PIXEL,WINDOW_Y_PIXEL);

    //set the background image
    this->setAutoFillBackground(false);
    //this->setStyleSheet(QStringLiteral("background-image:url(:/back/pic/back/back2.jpg)"));
    this->setStyleSheet(QStringLiteral("background-image:url(./skin/current/skin)"));

    QIcon icon;
    //cycle mode pushButton
    _pushButtonCycleMode = new QPushButton(this);
    _pushButtonCycleMode->setGeometry(QRect(10, 285, 20, 20));
    _pushButtonCycleMode->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/play_cycle.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonCycleMode->setIcon(icon);
    _pushButtonCycleMode->setIconSize(_pushButtonCycleMode->rect().size());
    _pushButtonCycleMode->setStyleSheet("background:transparent");
    //prev pushButton
    _pushButtonPrev = new QPushButton(this);
    _pushButtonPrev->setGeometry(QRect(60, 285, 20, 20));
    _pushButtonPrev->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/prev.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonPrev->setIcon(icon);
    _pushButtonPrev->setIconSize(_pushButtonPrev->rect().size());
    _pushButtonPrev->setStyleSheet("background:transparent");
    //playpause pushButton
    _pushButtonPlayPause = new QPushButton(this);
    _pushButtonPlayPause->setGeometry(QRect(110, 285, 20, 20));
    _pushButtonPlayPause->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonPlayPause->setIcon(icon);
    _pushButtonPlayPause->setIconSize(_pushButtonPlayPause->rect().size());
    _pushButtonPlayPause->setStyleSheet("background:transparent");
    //next pushButton
    _pushButtonNext = new QPushButton(this);
    _pushButtonNext->setGeometry(QRect(160, 285, 20, 20));
    _pushButtonNext->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/next.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonNext->setIcon(icon);
    _pushButtonNext->setIconSize(_pushButtonNext->rect().size());
    _pushButtonNext->setStyleSheet("background:transparent");
    //volume pushButton
    /*_pushButtonVolume = new QPushButton(this);
    _pushButtonVolume->setGeometry(QRect(210, 285, 20, 20));
    _pushButtonVolume->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/volume.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonVolume->setIcon(icon);
    _pushButtonVolume->setIconSize(_pushButtonVolume->rect().size());
    _pushButtonVolume->setStyleSheet("background:transparent");*/
    //volume slider
    _volumeSlider = new SliderClick(this);
    _volumeSlider->setOrientation(Qt::Horizontal);
    _volumeSlider->setGeometry(QRect(185,235,50,5));
    _volumeSlider->setMaximum(100);
    _volumeSlider->setMinimum(0);
    _volumeSlider->setValue(0);
    _volumeSlider->setMouseTracking(true);
    _volumeSlider->setFixedSize(50,8);
    _volumeSlider->setStyleSheet("background:transparent");
    //skin pushButton
    _pushButtonSkin = new QPushButton(this);
    _pushButtonSkin->setGeometry(QRect(210, 285, 20, 20));
    _pushButtonSkin->setFocusPolicy(Qt::NoFocus);
    icon.addFile(QStringLiteral(":/icon/pic/icon/skin.png"), QSize(), QIcon::Normal, QIcon::Off);
    _pushButtonSkin->setIcon(icon);
    _pushButtonSkin->setIconSize(_pushButtonSkin->rect().size());
    _pushButtonSkin->setStyleSheet("background:transparent");
    //music List
    _musicList = new QListWidget(this);
    _musicList->setGeometry(QRect(2,2,236,230));
    _musicList->setFocusPolicy(Qt::NoFocus);
    _musicList->setStyleSheet("background:transparent");
    _musicList->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));
    _musicList->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);background: transparent"));
    _musicList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//turn off scroll bar
    _musicList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _musicList->setVerticalScrollMode(QListWidget::ScrollPerItem);
    QScroller::grabGesture(_musicList, QScroller::LeftMouseButtonGesture);
    //progress slider
    _playProgressSlider = new SliderClick(this);
    _playProgressSlider->setOrientation(Qt::Horizontal);
    _playProgressSlider->setGeometry(QRect(35,270,200,5));
    _playProgressSlider->setMaximum(100);
    _playProgressSlider->setMinimum(0);
    _playProgressSlider->setValue(0);
    _playProgressSlider->setMouseTracking(true);
    _playProgressSlider->setFixedSize(170,8);//size
    //totol time label

    _labelTimeTotol = new QLabel(this);
    _labelTimeTotol->setGeometry(208,263,50,20);
    _labelTimeTotol->setAlignment(Qt::AlignLeft);
    _labelTimeTotol->setText(QString::number(_timeTotol / 60) + ":" + QString::number(_timeTotol % 60));
    _labelTimeTotol->setFixedSize(28,15);
    _labelTimeTotol->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));
    _labelTimeTotol->setStyleSheet("color: rgb(255, 255, 255);background:transparent");
    //played time label
    _labelTimePlayed = new QLabel(this);
    _labelTimePlayed->setGeometry(3,263,50,20);
    _labelTimePlayed->setAlignment(Qt::AlignRight);
    _labelTimePlayed->setText(QString::number(_timePlayed / 60) + ":" + QString::number(_timePlayed % 60));
    _labelTimePlayed->setFixedSize(28,15);
    _labelTimePlayed->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));
    _labelTimePlayed->setStyleSheet("color: rgb(255, 255, 255);background:transparent");
    //playing music name
    _labelCurrentPlayName = new QLabel(this);
    _labelCurrentPlayName->setGeometry(30,243,210,40);
    _labelCurrentPlayName->setAlignment(Qt::AlignLeft);
    _labelCurrentPlayName->setFixedSize(180,20);
    _labelCurrentPlayName->setFont(QFont("wqy-zenhei",10*FONT_AMP,QFont::Normal));
    _labelCurrentPlayName->setStyleSheet("color: rgb(255, 3, 161);background:transparent");
}

void Window::setCurrentPlayName(int index)
{
    _labelCurrentPlayName->setText(_musicList->item(index)->text());
}

/************************************************************************************************
 * cycle mode pushbutton slot
 * *********************************************************************************************/
void Window::_slotPushButtonCycleMode(void)
{
    QIcon icon;
    static CycleMode mode = Cycle;

    if(mode == Random)//random play
    {
        mode = Single;
        icon.addFile(QStringLiteral(":/icon/pic/icon/play_single.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    else if(mode == Single)//single paly
    {
        mode = Cycle;
        icon.addFile(QStringLiteral(":/icon/pic/icon/play_cycle.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    else if(mode == Cycle)//cycle play
    {
        mode = Random;
        icon.addFile(QStringLiteral(":/icon/pic/icon/play_random.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    _pushButtonCycleMode->setIcon(icon);

    emit
        cycleModeSignal(mode);
}

/************************************************************************************************
 * play pause pushbutton slot
 * *********************************************************************************************/
void Window::_slotPushButtonPlayPause(void)
{
    QIcon icon;

    if(_playPauseMode == Pause)
    {
        _playPauseMode = Play;
        icon.addFile(QStringLiteral(":/icon/pic/icon/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    else
    {
        _playPauseMode = Pause;
        icon.addFile(QStringLiteral(":/icon/pic/icon/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    _pushButtonPlayPause->setIcon(icon);

    emit playPauseSignal(_playPauseMode);
}

void Window::setPlayPauseButton(PlayPause mode)
{
    QIcon icon;

    if(mode == Play)
    {
        _playPauseMode = Play;
        icon.addFile(QStringLiteral(":/icon/pic/icon/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    else
    {
        _playPauseMode = Pause;
        icon.addFile(QStringLiteral(":/icon/pic/icon/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    _pushButtonPlayPause->setIcon(icon);
}

void Window::_slotPushButtonSkin(void)
{
    SkinDialog skinDialog;

    skinDialog.exec();
    this->setStyleSheet(QStringLiteral("background-image:url(./skin/current/skin)"));
}

void Window::_slotMusicListClicked(void)
{
    emit currentMusicChangedSignal(_musicList->currentRow());
}

void Window::musicListClear(void)
{
    _musicList->clear();
}

void Window::musicListAddItem(const QString name)
{

    QString str = name;
    str = str.right(str.count() - str.lastIndexOf("/") - 1);
    _musicList->addItem(str);
}

void Window::musicListAddItems(const QStringList list)
{
    for(int i = 0; i < list.count(); i++)
    {
        QString str = list.at(i);
        str = str.right( str.count() - str.lastIndexOf("/") - 1);
        _musicList->addItem(QApplication::translate(" ",str.toLocal8Bit().data(), 0));
    }
}

void Window::setCurrentTime(int second)
{
    QString show;

    _timePlayed = second;
    show = QString::number(second / 60);
    show += ':';
    show += QString::number(second % 60);
    this->_labelTimePlayed->setText(show);

    if(!_playProgressSlider->isSliderDown())
    {
        //this->_playProgressBar->setValue(_timePlayed * 100 / _timeTotol);
        disconnect(_playProgressSlider, &SliderClick::valueChanged, this, &Window::playedPercentChangedSignal);
        this->_playProgressSlider->setValue(_timePlayed * 100 / _timeTotol);
        connect(_playProgressSlider, &SliderClick::valueChanged, this, &Window::playedPercentChangedSignal);
    }
}

void Window::setTimeLength(int second)
{
    QString show;

    _timeTotol = second;
    show = QString::number(second / 60);
    show += ':';
    show += QString::number(second % 60);
    this->_labelTimeTotol->setText(show);
}

void Window::setVolume(int volume)
{
    this->_volumeSlider->setValue(volume);
}


Window::~Window()
{

}
