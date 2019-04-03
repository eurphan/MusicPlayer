#ifndef MUSICPLAY_H
#define MUSICPLAY_H

#include <QDialog>
#include "window.h"
#include "mplayercall.h"

#define PLAY_LIST_PATCH "./Music"
#define VOLUME_INIT     80

class MusicPlay : public QObject
{
    Q_OBJECT

public:
    static MusicPlay& construction(void)
    {
        static MusicPlay play;
        return play;
    }

    MusicPlay();
    ~MusicPlay();
    void updatePlayList(void);

protected:

private:
    QStringList _playList;
    int _musicIndex;
    Window::CycleMode _cycleMode;

public slots:
    void playPauseDel(Window::PlayPause mode);
    void cycleModeDel(Window::CycleMode mode);
    void changeMusicSlot(unsigned int index);
    void prevDel(void);
    void nextDel(void);
    void mpReturnDel(MPlayerCall::MplayerReturn ret);
    void percentChangedDel(int percent);
    void volumeChangedDel(int volume);
signals:

};

#endif // MUSICPLAY_H

#define musicPlay MusicPlay::construction()
