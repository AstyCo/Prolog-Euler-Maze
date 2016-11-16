#include "parameterswidgetaction.h"
#include "parameterswidget.h"

ParametersWidgetAction::ParametersWidgetAction(QObject *parent) :
    QWidgetAction(parent)
{

}

QWidget *ParametersWidgetAction::createWidget(QWidget *parent)
{
    return (new ParametersWidget(parent));
}
