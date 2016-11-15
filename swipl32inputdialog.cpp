#include "swipl32inputdialog.h"
#include "ui_swipl32inputdialog.h"

Swipl32InputDialog::Swipl32InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Swipl32InputDialog)
{
    ui->setupUi(this);
}

Swipl32InputDialog::~Swipl32InputDialog()
{
    delete ui;
}

QString Swipl32InputDialog::sPathSwipl32() const{
    return ui->lineEdit->text();
}

void Swipl32InputDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        ui->labelHelp->setText(QString::fromUtf8("Путь не может быть пустым!"));
        ui->lineEdit->setFocus();
        return;
    }
    QDialog::accept();
}
