% borders(border_right,border_bottom) - клетка содержит
% информацию имеет ли она правую//нижнюю границу.
bordersExample(borders(y,n)).

% ѕредставление лабиринта.
%    ¬ данном представлении, границы вокруг лабиринта
% не учитываютс€. ѕредполагаетс€, что средства отображени€ понимают,
% как их отрисовывать(зна€ вход/выход/структуру).
mazeExample( maze(
% вход	     выход
  cell(0,0), cell(2,5),
%    0	          1            2            3            4            5
  [ [borders(y,n),borders(y,n),borders(n,n),borders(n,n),borders(y,n),borders(n,n)],
    [borders(y,n),borders(y,n),borders(y,n),borders(y,y),borders(n,y),borders(n,n)],
    [borders(n,n),borders(y,y),borders(n,n),borders(y,n),borders(y,n),borders(n,y)],
    [borders(n,y),borders(y,n),borders(y,y),borders(n,n),borders(y,y),borders(n,n)],
    [borders(n,n),borders(n,n),borders(n,n),borders(n,n),borders(n,n),borders(n,n)]
])).

eulerCellsRow([ eulerCell(1, cell(n, n)),
		eulerCell(1, cell(n, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(3, cell(n, n)) ]).

eulerCellsRow1([ eulerCell(1, cell(n, n)),
		eulerCell(1, cell(y, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(n, y)),
		eulerCell(2, cell(y, n)),
		eulerCell(3, cell(n, n)) ]).


% main predicate
%
mazeGenerator(Rows,Columns,maze(Entrace,Exit,Matrix)):-
	matrix_generator(Rows,Columns,Matrix),
	entraces_generator(Rows,Columns,Entrace,Exit).

%
matrixGenerator(0,_,[]):-!.
%matrixGenerator(RowCnt,ColumnCnt,[Row|RestRows]):-!. %отсечение?



%
%entracesGenerator(Rows,Columns,Entrace,Exit).

makeFirst(0,[]):-!.
makeFirst(N,Row):-
	makeFirstHelper(N,1,BorderFreeRow),
	setBorders(BorderFreeRow,Row).

makeFirstHelper(0,_,[]):-!.
makeFirstHelper(N,Index,Row):-
	Nnew is N-1,
	IndexRange is Index + 2,
	random(Index,IndexRange,IndexNew),
	makeFirstHelper(Nnew,IndexNew,RowNew),
	Row = [eulerCell(Index,cell(n,n))|RowNew].

setBorders(BorderFreeRow,Row):-
	setRightBorders(BorderFreeRow,RowWithRightBorders),
	setBottomBorders(RowWithRightBorders,Row),!.


% setRightBorders(<border free row>,<row with right borders>)
% determinated function
setRightBorders([X],[X]):-!.
setRightBorders([eulerCell(Index1,cell(n,Y)),eulerCell(Index2,cell(n,Z))|Rest],
	   [eulerCell(Index1,cell(R,Y))|BorderedRest]):-
	(   Index1=Index2,R=n ; Index1\=Index2,R=y),
	setRightBorders([eulerCell(Index2,cell(n,Z))|Rest],BorderedRest),!.


setBottomBorders([X],[X]):-!.
setBottomBorders(Before,After):-
	findall(Cells,sameIndexCells(Before,_,Cells),SameGroups),
	setBottomBordersHelper(SameGroups,SameGroupsWithBorders),
	flatten(SameGroupsWithBorders,After).

setBottomBordersHelper([],[]):-!.
setBottomBordersHelper([X|Y],[X1|Y1]):-
	randomizeGroups(X,X1),
	setBottomBordersHelper(Y,Y1).

randomizeGroups([],_):-!.
randomizeGroups(X,Y):-
	length(X,N),
	N1 is N+1,
	random(1,N1,NnoBorder),
	randset(NnoBorder,N,NoBorderIndexes),
	getMask(N,NoBorderIndexes,BorderMask),
	installRandomizedBottomBorders(X,BorderMask,Y).

getMask(N,I,L):-reverse(L,L1),getMaskH(N,I,L1).

getMaskH(0,_,[]):-!.
getMaskH(N,I,[X|Y]):-
	member(N,I),
	N1 is N-1,
	X=n,
	getMask(N1,I,Y),!.

getMaskH(N,I,[X|Y]):-
	%\+ member(N,I),
	N1 is N-1,
	X=y,
	getMask(N1,I,Y).



installRandomizedBottomBorders([],_,[]):-!.
installRandomizedBottomBorders([eulerCell(Index,cell(R,n))|X],
			       [HaveBorders|M],
			       [eulerCell(Index,cell(R,HaveBorders))|Y]):-
	installRandomizedBottomBorders(X,M,Y).


sameIndexCells([],_,[]):-!.
sameIndexCells([eulerCell(Index,CellInfo)|Rest],Index,
	       [eulerCell(Index,CellInfo)|RestCells]):-
	sameIndexCells(Rest,Index,RestCells).
sameIndexCells([eulerCell(Index1,_)|Rest],Index,RestCells):-
	sameIndexCells(Rest,Index,RestCells)
	,Index1\=Index.

























sameIndexCellsH(L,N,C):-notIsEmpty(C),sameIndexCells(L,N,C).

same(L):-maplist(=(_),L).
notIsEmpty([_]).
