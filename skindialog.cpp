#include "skindialog.h"
#include "QPushButton"
#include "QWidgetList"
#include "window.h"
#include "QDir"
#include <QApplication>
#include <QDebug>
#include <QPixmap>

SkinDialog::SkinDialog()
{
    _initWindow();
    _updatePicList();
    connect(_fileList, &QListWidget::itemSelectionChanged, this, &SkinDialog::_slotPicShow);
    connect(_pushButtonCancel, &QPushButton::clicked, this, &SkinDialog::_slotCancel);
    connect(_pushButtonOK, &QPushButton::clicked, this, &SkinDialog::_slotOK);
}

void SkinDialog::_initWindow(void)
{
    this->resize(160,200);
    this->setWindowTitle("Select a picture");
    this->setFont(QFont("wqy-zenhei",FONT_AMP,QFont::Normal));
    this->setStyleSheet(QStringLiteral("background-color:rgb(164,241,255)"));

    _pushButtonOK = new QPushButton(this);
    _pushButtonOK->setGeometry(QRect(110, 150, 40, 25));
    _pushButtonOK->setStyleSheet("background:transparent");
    _pushButtonOK->setText("OK");
    _pushButtonOK->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));

    _pushButtonCancel = new QPushButton(this);
    _pushButtonCancel->setGeometry(QRect(110, 175, 40, 25));
    _pushButtonCancel->setStyleSheet("background:transparent");
    _pushButtonCancel->setText("Cancel");
    _pushButtonCancel->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));

    _labelPicShow = new QLabel(this);
    _labelPicShow->setGeometry(QRect(2, 144, 100, 54));
   // _labelPicShow->setStyleSheet("background:transparent");
    //_labelPicShow->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));

    _fileList = new QListWidget(this);
    _fileList->setGeometry(QRect(2,2,156,140));
    _fileList->setFocusPolicy(Qt::NoFocus);
    _fileList->setStyleSheet("background:transparent");
    _fileList->setFont(QFont("wqy-zenhei",8*FONT_AMP,QFont::Normal));
    _fileList->setStyleSheet(QStringLiteral("color: rgb(255, 100, 0);background: transparent"));
    _fileList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//turn off scroll bar
    _fileList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _fileList->setVerticalScrollMode(QListWidget::ScrollPerItem);
    QScroller::grabGesture(_fileList, QScroller::LeftMouseButtonGesture);
}

void SkinDialog::_updatePicList(void)
{
    QDir dir(SKIN_PATCH);

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
    _skinList.clear();
    for(int i = 0; i < fileCount; i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString suffix = fileInfo.suffix();
        if((QString::compare(suffix, QString("bmp"), Qt::CaseInsensitive) == 0) || \
           (QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0) || \
           (QString::compare(suffix, QString("jpeg"), Qt::CaseInsensitive) == 0))
        {
            _skinList.append(fileInfo.filePath());
        }
    }
    for(int i = 0; i < _skinList.count(); i++)
    {
        QString str = _skinList.at(i);
        str = str.right( str.count() - str.lastIndexOf("/") - 1);
        _fileList->addItem(QApplication::translate(" ",str.toLocal8Bit().data(), 0));
    }
}

void SkinDialog::_slotPicShow(void)
{
    QPixmap pic;
    pic = _skinList.at(_fileList->currentRow());
    //pic.scaled(QSize(40,60), Qt::KeepAspectRatio);
    _labelPicShow->setPixmap(pic);
    _labelPicShow->setScaledContents(true);
}

void SkinDialog::_slotCancel(void)
{
    this->close();
}


void SkinDialog::_slotOK(void)
{
    if(_fileList->currentRow() < 0)
    {
        return;
    }
    QString sourceDir = _skinList.at(_fileList->currentRow());
    QString targetDir = SKIN_PATCH;
    targetDir += "/current/skin";

    QDir *createfile = new QDir;
    if (createfile->exists(targetDir))
    {
        createfile->remove(targetDir);
    }

    QFile::copy(sourceDir, targetDir);

    this->close();
}

SkinDialog::~SkinDialog()
{

}

