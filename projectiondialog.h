#ifndef PROJECTIONDIALOG_H
#define PROJECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

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
    QComboBox* input

private:
    initUI();
};

#endif // PROJECTIONDIALOG_H
