#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H
#include <atlbase.h>
#include <dshow.h>
#include <atlstr.h>
#include <QWidget>
#include <QResizeEvent>
class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    IVideoWindow *m_pVW;
public:
    explicit PlayerWidget(QWidget *parent = nullptr,IVideoWindow *p=nullptr);
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // PLAYERWIDGET_H
