#ifndef GENERATIONSTATE_H
#define GENERATIONSTATE_H

#include "cellobject.h"
#include <QObject>

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

class GenerationState : public QObject
{
    Q_OBJECT
public:
    enum State{
        startState,
        makeFirstHelperState,
        setBordersState,
        bodyNewRowIndexesState,
        lastBodySetBottomBordersState,
        finishNewRowIndexesState,
        finishRowHState,
        completedState,

        State_count
    };


    explicit GenerationState(QObject *parent = 0);

    void initState(int rowCnt);
//    fid_t fid() const;
    bool first() const;
    bool last() const;
    int rowsRemain() const;
    term_t lastRowT() const;

signals:
    void stateChanged(GenerationState::State state);

public:

//    fid_t m_frameId;
    functor_t m_newRowIndexesF;
    functor_t m_setBottomBordersF;

    QVector<Cell> m_lastRow;
    int m_rowsRemain;

    State state() const;
    void setState(const State &state);

    void setLastRow(const QVector<Cell> &lastRow);

public slots:
    void onRowReaded(const QVector<Cell> &row);
    void setRowsRemain(int rowsRemain);
    void reduceRowsRemain();

private:
    State m_state;
};

#endif // GENERATIONSTATE_H
