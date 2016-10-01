#include "parameterswidget.h"
#include "ui_parameterswidget.h"

ParametersWidget::ParametersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametersWidget)
{
    ui->setupUi(this);

    connect(ui->spinBoxRowCnt,SIGNAL(valueChanged(int)),this,SIGNAL(rowsChanged(int)));
    connect(ui->spinBoxColCnt,SIGNAL(valueChanged(int)),this,SIGNAL(colsChanged(int)));
}

ParametersWidget::~ParametersWidget()
{
    delete ui;
}

int ParametersWidget::row() const
{
    return ui->spinBoxRowCnt->value();
}

void ParametersWidget::setRow(int row)
{
    ui->spinBoxRowCnt->setValue(row);
}

int ParametersWidget::col() const
{
    return ui->spinBoxColCnt->value();
}

void ParametersWidget::setCol(int col)
{
    ui->spinBoxColCnt->setValue(col);
}

