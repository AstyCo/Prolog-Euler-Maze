#include "generationstate.h"
#include "global.h"
#include "mainwindow.h"

#include <QDebug>

GenerationState::GenerationState(QObject *parent) : QObject(parent)
{
    m_lastRowT = 0;
    initState(-1);
}

void GenerationState::initState(int rowCnt)
{
    m_state = startState;
    if(m_lastRowT)
        delete m_lastRowT;
    m_lastRowT = 0;
    m_rowsRemain = rowCnt;

    PlCall("consult(\'../Prolog-Euler-Maze/maze.pl\')");
    m_newRowIndexesF = PL_new_functor(PL_new_atom("newRowIndexes"),2);;
    m_setBottomBordersF = PL_new_functor(PL_new_atom("setBottomBorders"),2);;

}

bool GenerationState::first() const
{
    return m_lastRowT == 0;
}

bool GenerationState::last() const
{
    return m_rowsRemain == 1;
}


void GenerationState::onRowReaded(term_t *rowT)
{
    qDebug() << "onRowReaded";
    if(m_lastRowT)
        delete m_lastRowT;
    qDebug() << "onRowReaded";
    m_lastRowT = rowT;
    reduceRowsRemain();
    qDebug() << "onRowReaded";
}

int GenerationState::rowsRemain() const
{
    return m_rowsRemain;
}

void GenerationState::setRowsRemain(int rowsRemain)
{
    m_rowsRemain = rowsRemain;
    if(!m_rowsRemain)
        setState(completedState);
}

void GenerationState::reduceRowsRemain()
{
    if(!--m_rowsRemain)
        setState(completedState);
}

GenerationState::State GenerationState::state() const
{
    return m_state;
}

void GenerationState::setState(const State &state)
{
    m_state = state;
}

