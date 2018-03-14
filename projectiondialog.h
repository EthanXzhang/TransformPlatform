#ifndef PROJECTIONDIALOG_H
#define PROJECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <movieinfo.h>

namespace Ui {
class ProjectionDialog;
}

class ProjectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectionDialog(QWidget *parent = 0);
    ~ProjectionDialog();
    MovieInfo* movieinfo;

private:
    Ui::ProjectionDialog *ui;

    QComboBox *inputformatbox,*outputformatbox,*algorithmbox;
    QLineEdit *widthline,*heightline,*maxcubelenghtline,*cubelenghtline,*vsline,*hsline;
    QCheckBox *enablelowpassbox,*enableMTbox;

private:
    void initUI();
    void initComboBox();
    void getMovieInfo();
};

#endif // PROJECTIONDIALOG_H
