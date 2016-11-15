#ifndef SWIPL32INPUTDIALOG_H
#define SWIPL32INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class Swipl32InputDialog;
}

class Swipl32InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Swipl32InputDialog(QWidget *parent = 0);
    ~Swipl32InputDialog();

    QString sPathSwipl32() const;

public slots:
    void accept();

private:
    Ui::Swipl32InputDialog *ui;
};

#endif // SWIPL32INPUTDIALOG_H
