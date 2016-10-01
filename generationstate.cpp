#include "generationstate.h"
#include "global.h"
#include "mainwindow.h"

#include <QDebug>

GenerationState::GenerationState(QObject *parent) : QObject(parent)
{
//    m_frameId = 0;
    initState(-1);
}

void GenerationState::initState(int rowCnt)
{
//    if(m_frameId)
//        PL_close_foreign_frame(m_frameId);
//    m_frameId = PL_open_foreign_frame();
    m_state = startState;
    m_lastRow.clear();
    m_rowsRemain = rowCnt;

    m_newRowIndexesF = PL_new_functor(PL_new_atom("newRowIndexes"),2);
    m_setBottomBordersF = PL_new_functor(PL_new_atom("setBottomBorders"),2);

}

//PL_fid_t GenerationState::fid() const
//{
//    return m_frameId;
//}

bool GenerationState::first() const
{
    return m_lastRow.isEmpty();
}

bool GenerationState::last() const
{
    return m_rowsRemain == 1;
}


void GenerationState::onRowReaded(const QVector<Cell> &row)
{
    m_lastRow = row;
    reduceRowsRemain();
}

int GenerationState::rowsRemain() const
{
    return m_rowsRemain;
}

term_t GenerationState::lastRowT() const
{
    return MainWindow::row_to_term(m_lastRow);
}

void GenerationState::setLastRow(const QVector<Cell> &lastRow)
{
    m_lastRow = lastRow;
}

void GenerationState::setRowsRemain(int rowsRemain)
{
    m_rowsRemain = rowsRemain;
}

void GenerationState::reduceRowsRemain()
{
    --m_rowsRemain;
}

GenerationState::State GenerationState::state() const
{
    return m_state;
}

void GenerationState::setState(const State &state)
{
    m_state = state;
    emit stateChanged(state);
}

