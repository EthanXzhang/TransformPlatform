#ifndef PROJECTIONDIALOG_H
#define PROJECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectionDialog;
}

class ProjectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectionDialog(QWidget *parent = 0);
    ~ProjectionDialog();

private:
    Ui::ProjectionDialog *ui;
};

#endif // PROJECTIONDIALOG_H
