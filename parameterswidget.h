#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>

namespace Ui {
class ParametersWidget;
}

class ParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParametersWidget(QWidget *parent = 0);
    ~ParametersWidget();

    int row() const;
    void setRow(int row);

    int col() const;
    void setCol(int col);

signals:
    void rowsChanged(int);
    void colsChanged(int);

private:
    Ui::ParametersWidget *ui;
};

#endif // PARAMETERSWIDGET_H
