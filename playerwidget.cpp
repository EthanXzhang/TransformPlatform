#include "playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent,IVideoWindow *p) : QWidget(parent)
{
    m_pVW=p;
}
void PlayerWidget::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    m_pVW->SetWindowPosition(0,0,size.width(),size.height());
}
