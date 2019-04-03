#ifndef SKINDIALOG_H
#define SKINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QStringList>
#include <QLabel>

#define SKIN_PATCH "./skin"

class SkinDialog : public QDialog
{
    Q_OBJECT

public:
    SkinDialog();
    virtual ~SkinDialog();
private:
    void _initWindow(void);
    void _updatePicList(void);
    QPushButton *_pushButtonOK;
    QPushButton *_pushButtonCancel;
    QListWidget *_fileList;
    QLabel *_labelPicShow;
    QStringList _skinList;

public slots:
    void _slotPicShow(void);
    void _slotOK(void);
    void _slotCancel(void);

signals:

};

#endif // SKINDIALOG_H
