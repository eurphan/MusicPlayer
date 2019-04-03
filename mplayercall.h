#ifndef MPLAYERCALL_H
#define MPLAYERCALL_H

#include <QObject>
#include <QProcess>


class MPlayerCall : public QObject
{
    Q_OBJECT

public:
    enum MplayerAns
    {
        ANS_TIME_POSITION,    //current time
        ANS_PERCENT_POSITION,//percent
        ANS_LENGTH,         //all time
        ANS_EXIT,
    };
    typedef struct
    {
        MplayerAns ans;
        int value;
    } MplayerReturn;

    static MPlayerCall& construction(void)
    {
        static MPlayerCall obj;
        return obj;
    }
    MPlayerCall();
    virtual ~MPlayerCall();
    void setMusic(const QString name);
    void setMusicPlay(const QString name);
    void pause(void);
    void play(void);
    void setVolume(unsigned char volume);
    void getTimePos(void);
    void getTimeLen(void);
    void seek(int percent);
    virtual void timerEvent( QTimerEvent *event);

private:
    void _mplayerWriteCmd(QString cmd);
    void _mplayerReadMessages(void);
    QProcess *_process;
    bool _isPlaying;
    unsigned char _volume;
    int _timer1SID;
    int _timer100MsID;

public slots:


signals:
    void mplayerReturnValue(MplayerReturn);
};

#endif // MPLAYERCALL_H

#define mplayerCall MPlayerCall::construction()

