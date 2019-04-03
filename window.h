#ifndef WINDOW_H
#define WINDOW_H

/*includes*/
#include <QDialog>
#include <QPalette>
#include <QPixmap>
#include <QBrush>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QScroller>
#include <QProgressBar>
#include <QSlider>
#include <sliderclick.h>

/*defines*/
#define WINDOW_X_PIXEL 240
#define WINDOW_Y_PIXEL 320
//#define FONT_AMP 4
#define FONT_AMP 1

class Window : public QDialog
{
    Q_OBJECT

public:
    enum CycleMode{Random,Single,Cycle};
    enum PlayPause{Play,Pause};

    Window(QWidget *parent = 0);
    ~Window();
    static Window& construction(void)
    {
        static Window win;
        return win;
    }
    void musicListClear(void);
    void musicListAddItem(const QString name);
    void musicListAddItems(const QStringList list);
    void setPlayPauseButton(PlayPause mode);
    void setCurrentPlayName(int index);
    void setCurrentTime(int second);
    void setTimeLength(int second);
    void setVolume(int volume);

protected:

private:
    void _initWindow(void);

    QPushButton* _pushButtonCycleMode;
    QPushButton* _pushButtonSkin;
    QPushButton* _pushButtonPlayPause;
    QPushButton* _pushButtonNext;
    QPushButton* _pushButtonPrev;
    //QPushButton* _pushButtonVolume;
    QListWidget* _musicList;
    SliderClick *_volumeSlider;
    //QProgressBar* _playProgressBar;
    SliderClick* _playProgressSlider;
    QLabel* _labelTimeTotol;
    QLabel* _labelTimePlayed;
    QLabel* _labelCurrentPlayName;
    PlayPause _playPauseMode;
    unsigned int _timeTotol;
    unsigned int _timePlayed;

public slots:
    void _slotPushButtonCycleMode(void);
    void _slotPushButtonPlayPause(void);
    void _slotMusicListClicked(void);
    void _slotPushButtonSkin(void);

signals:
    void cycleModeSignal(CycleMode mode);
    void playPauseSignal(PlayPause mode);
    void prevSignal(void);
    void nextSignal(void);
    void volumeSignal(int volume);
    void currentMusicChangedSignal(unsigned int index);
    void playedPercentChangedSignal(int percent);
};

#endif // WINDOW_H

#define win Window::construction()
