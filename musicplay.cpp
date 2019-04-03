#include "musicplay.h"
#include "window.h"
#include <QDebug>
#include <QDir>
#include <QTime>

/************************************************************************************************
 * init
 * *********************************************************************************************/
MusicPlay::MusicPlay()
{
    _musicIndex = -1;
    _cycleMode = Window::Cycle;
    updatePlayList();
    win.setVolume(VOLUME_INIT);
    mplayerCall.setVolume(VOLUME_INIT);

    connect(&win, &Window::playPauseSignal, this, &MusicPlay::playPauseDel);
    connect(&win, &Window::currentMusicChangedSignal, this, &MusicPlay::changeMusicSlot);
    connect(&win, &Window::prevSignal, this,&MusicPlay::prevDel);
    connect(&win, &Window::nextSignal, this, &MusicPlay::nextDel);
    connect(&win, &Window::cycleModeSignal, this, &MusicPlay::cycleModeDel);
    connect(&mplayerCall, &MPlayerCall::mplayerReturnValue, this, &MusicPlay::mpReturnDel);
    connect(&win, &Window::playedPercentChangedSignal, this, &MusicPlay::percentChangedDel);
    connect(&win, &Window::volumeSignal, this, &MusicPlay::volumeChangedDel);
}

/************************************************************************************************
 * playPause Button signal del
 * *********************************************************************************************/
void MusicPlay::playPauseDel(Window::PlayPause mode)
{
    if(mode == Window::Play)
    {
        if(_musicIndex == -1)
        {
            _musicIndex = 0;
            win.setCurrentPlayName(_musicIndex);
        }
        changeMusicSlot(_musicIndex);
        mplayerCall.play();
    }
    else if(mode == Window::Pause)
    {
        mplayerCall.pause();
    }
}

void MusicPlay::prevDel(void)
{
    if((_cycleMode == Window::Cycle) || (_cycleMode == Window::Single))
    {
        _musicIndex = (_musicIndex - 1 < 0) ? (_playList.count() - 1) : (_musicIndex - 1);
        changeMusicSlot(_musicIndex);
        win.setCurrentPlayName(_musicIndex);
    }
    else if(_cycleMode == Window::Random)
    {
        int tmp;
        qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        tmp = qrand() % _playList.count();
        _musicIndex = (tmp == _musicIndex) ? (_musicIndex + 1) : tmp;
        _musicIndex = (_musicIndex >= _playList.count()) ? 0 : _musicIndex;
        changeMusicSlot(_musicIndex);
        win.setCurrentPlayName(_musicIndex);
    }
}

void MusicPlay::nextDel(void)
{
    if((_cycleMode == Window::Cycle) || (_cycleMode == Window::Single))
    {
        _musicIndex = (_musicIndex + 1) % _playList.count();
        changeMusicSlot(_musicIndex);
        win.setCurrentPlayName(_musicIndex);
    }
    else if(_cycleMode == Window::Random)
    {
        int tmp;
        qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        tmp = qrand() % _playList.count();
        _musicIndex = (tmp == _musicIndex) ? (_musicIndex + 1) : tmp;
        _musicIndex = (_musicIndex >= _playList.count()) ? 0 : _musicIndex;
        changeMusicSlot(_musicIndex);
        win.setCurrentPlayName(_musicIndex);
    }
}

void MusicPlay::cycleModeDel(Window::CycleMode mode)
{
    _cycleMode = mode;
}

void MusicPlay::changeMusicSlot(unsigned int index)
{
    _musicIndex = index;
    mplayerCall.setMusic(_playList.at(index));
    mplayerCall.play();
    win.setPlayPauseButton(Window::Play);
    win.setCurrentPlayName(_musicIndex);
}

void MusicPlay::mpReturnDel(MPlayerCall::MplayerReturn ret)
{
    if(ret.ans == MPlayerCall::ANS_LENGTH)
    {
        win.setTimeLength(ret.value);
    }
    else if(ret.ans == MPlayerCall::ANS_PERCENT_POSITION)
    {

    }
    else if(ret.ans == MPlayerCall::ANS_TIME_POSITION)
    {
        win.setCurrentTime(ret.value);
    }
    else if(ret.ans == MPlayerCall::ANS_EXIT)
    {
        nextDel();
    }
}

void MusicPlay::percentChangedDel(int percent)
{
    mplayerCall.seek(percent);
}

void MusicPlay::volumeChangedDel(int volume)
{
    mplayerCall.setVolume(volume);
}

/************************************************************************************************
 * update play list
 * *********************************************************************************************/
void MusicPlay::updatePlayList(void)
{
    QDir dir(PLAY_LIST_PATCH);

    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    int fileCount = list.count();
    if(fileCount <= 0)
    {
        return;
    }
    _playList.clear();
    for(int i = 0; i < fileCount; i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString suffix = fileInfo.suffix();
        if((QString::compare(suffix, QString("mp3"), Qt::CaseInsensitive) == 0) || \
           (QString::compare(suffix, QString("wav"), Qt::CaseInsensitive) == 0) || \
           (QString::compare(suffix, QString("wma"), Qt::CaseInsensitive) == 0))
        {
            _playList.append(fileInfo.filePath());
        }
    }
    win.musicListAddItems(_playList);
}

MusicPlay::~MusicPlay()
{

}

