#include "projectiondialog.h"
#include "ui_projectiondialog.h"

ProjectionDialog::ProjectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectionDialog)
{
    ui->setupUi(this);
}

ProjectionDialog::~ProjectionDialog()
{
    delete ui;
}