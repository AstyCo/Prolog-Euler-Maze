#ifndef GENERATIONSTATE_H
#define GENERATIONSTATE_H

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
        firstSetBordersState,
        bodyNewRowIndexesState,
        bodySetBottomBordersState,
        finishNewRowIndexesState,
        finishRowHState,
        completedState,

        State_count
    };


    explicit GenerationState(QObject *parent = 0);

    void initState(int rowCnt);
    bool first() const;
    bool last() const;
    int rowsRemain() const;

public:

    functor_t m_newRowIndexesF;
    functor_t m_setBottomBordersF;

    term_t *m_lastRowT;
    int m_rowsRemain;

    State state() const;
    void setState(const State &state);

public slots:
    void onRowReaded(term_t *rowT);
    void setRowsRemain(int rowsRemain);
    void reduceRowsRemain();

private:
    State m_state;
};

#endif // GENERATIONSTATE_H
