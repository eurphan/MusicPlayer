#include "mplayercall.h"
#include "QDebug"
#include <QTimerEvent>
#include <string>

#define TIMER_1SECOND    1000
#define TIMER_100MSECOND 100

static bool seeked = false;

/************************************************************************************************
 * init the mplayer
 * *********************************************************************************************/
MPlayerCall::MPlayerCall()
{
    _isPlaying = false;
    _volume = 80;

    _process = new QProcess();
    _process->setProcessChannelMode(QProcess::ForwardedErrorChannel);

    QStringList args;
    args << "-slave"; //使用slave模式
    args << "-quiet"; //不要输出冗余信息
 //   args << "-wid" << QString::number(widget->winId()); //将输出定位到ui下的widget窗口内
  //  args << "-zoom"; //自适应窗口
   // args << "-vo";
  //  args << "x11"; //使用x11模式播放(只有这种模式下才支持23行的指定输出窗口到ui->widget)
  //  args << "./1.mp3"; //播放file_name文件
    _process->start("mplayer", args); //启动该进程，并传入参数args

    _timer1SID = this->startTimer(TIMER_1SECOND);
    _timer100MsID = this->startTimer(TIMER_100MSECOND);
}

/************************************************************************************************
 * start play a music
 * *********************************************************************************************/
void MPlayerCall::play(void)
{
    if(!_isPlaying)
    {
        _process->write("pause\n");
        _isPlaying  = true;
    }
}

/************************************************************************************************
 * stop play
 * *********************************************************************************************/
void MPlayerCall::pause(void)
{
    if(_isPlaying)
    {
        _process->write("pause\n");
        _isPlaying = false;
    }
}

/************************************************************************************************
 * set a music but not play
 * *********************************************************************************************/
void MPlayerCall::setMusic(const QString name)
{
    if(_process != NULL)
    {
        _process->kill();
    }
    _process = new QProcess();

    QStringList args;
    args << "-slave"; //使用slave模式
    args << "-quiet"; //不要输出冗余信息
    args << name; //播放file_name文件
    _process->start("mplayer", args); //启动该进程，并传入参数args
    setVolume(_volume);
    pause();
    getTimeLen();
}

/************************************************************************************************
 * set a music and play
 * *********************************************************************************************/
void MPlayerCall::setMusicPlay(const QString name)
{
    if(_process != NULL)
    {
        _process->kill();
    }
    _process = new QProcess();

    QStringList args;
    args << "-slave"; //使用slave模式
    args << "-quiet"; //不要输出冗余信息
    args << name; //播放file_name文件
    _process->start("mplayer", args); //启动该进程，并传入参数args
    setVolume(_volume);
    _isPlaying = true;
    getTimeLen();
}

/************************************************************************************************
 * set the volume
 * *********************************************************************************************/
void MPlayerCall::setVolume(unsigned char volume)
{
    if(_process == NULL)
    {
        return;
    }
    _volume = volume;
    _mplayerWriteCmd("volume " + QString::number(volume) + " 2\n");
}

/************************************************************************************************
 * send a command to get the time position
 * *********************************************************************************************/
void MPlayerCall::getTimePos(void)
{
    _mplayerWriteCmd("get_time_pos\n");
}

/************************************************************************************************
 * send a command to get the media file time length
 * *********************************************************************************************/
void MPlayerCall::getTimeLen(void)
{
    _mplayerWriteCmd("get_time_length\n");
}

/************************************************************************************************
 * set the played percent
 * *********************************************************************************************/
void MPlayerCall::seek(int percent)
{
    _mplayerWriteCmd("seek " + QString::number(percent) + " 1\n");
    seeked = true;
    getTimePos();
}

/************************************************************************************************
 * write command to mplayer
 * *********************************************************************************************/
void MPlayerCall::_mplayerWriteCmd(QString cmd)
{
    if(_process == NULL)
    {
        return;
    }
    _process->write(cmd.toUtf8());
}

/************************************************************************************************
 * read the messages form mplayer
 * *********************************************************************************************/
void MPlayerCall::_mplayerReadMessages(void)
{
    while(_process->canReadLine())
    {
        char data[200];

        memset(data, 0, 200);
        _process->readLine(data,200);
        if((data[0] == 'E') && (data[1] == 'x') && (data[2] == 'i') && (data[3] == 't'))//exit
        {
            if(_isPlaying)
            {
                MplayerReturn mpRet;
                mpRet.ans = ANS_EXIT;
                emit mplayerReturnValue(mpRet);
            }
        }
        else if((data[0] == 'A') && (data[1] == 'N') && (data[2] == 'S'))
        {
            MplayerReturn mpRet;
            QString ans = NULL;
            QString value = NULL;

            int i = 4;
            while(data[i] != '=')
            {
                ans += data[i];
                i++;
            }
            i++;
            while((data[i] != '\n') && (data[i] != '.'))
            {
                value += data[i];
                i++;
            }
            mpRet.value = value.toInt();
            if(ans == "LENGTH")
            {
                mpRet.ans = ANS_LENGTH;
                emit mplayerReturnValue(mpRet);
            }
            else if(ans == "TIME_POSITION")
            {
                if(!seeked)
                {
                    mpRet.ans = ANS_TIME_POSITION;
                    emit mplayerReturnValue(mpRet);
                }
                else
                {
                    seeked = false;
                }
            }
            else if(ans == "PERCENT_POSITION")
            {
                mpRet.ans = ANS_PERCENT_POSITION;
                emit mplayerReturnValue(mpRet);
            }
        }
    }
}

/************************************************************************************************
 * timer event
 * *********************************************************************************************/
void MPlayerCall::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == _timer100MsID)
    {
        _mplayerReadMessages();
    }
    else if(event->timerId() == _timer1SID)
    {
        if(_isPlaying)
        {
            getTimePos();
        }
    }
}

/************************************************************************************************
 * delete private
 * *********************************************************************************************/
MPlayerCall::~MPlayerCall()
{
    _process->kill();
    delete _process;
}

