#ifndef PARAMETERSWIDGETACTION_H
#define PARAMETERSWIDGETACTION_H

#include <QWidgetAction>

class ParametersWidgetAction : public QWidgetAction
{
public:
    ParametersWidgetAction(QObject *parent = NULL);

protected:
    QWidget *createWidget(QWidget *parent);
};

#endif // PARAMETERSWIDGETACTION_H
