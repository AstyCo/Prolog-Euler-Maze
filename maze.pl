% borders(border_right,border_bottom) - ������ ��������
% ���������� ����� �� ��� ������//������ �������.
bordersExample(borders(y,n)).

% ������������� ���������.
%    � ������ �������������, ������� ������ ���������
% �� �����������. ��������������, ��� �������� ����������� ��������,
% ��� �� ������������(���� ����/�����/���������).
mazeExample( maze(
% ����	     �����
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
		eulerCell(2, cell(n, n)),
		eulerCell(2, cell(y, n)),
		eulerCell(3, cell(n, n)) ]).


% main predicate
%
mazeGenerator(Rows,Columns,maze(Entrace,Exit,Matrix)):-
	matrix_generator(Rows,Columns,Matrix),
	entraces_generator(Rows,Columns,Entrace,Exit).

%
matrixGenerator(0,_,[]):-!.
%matrixGenerator(RowCnt,ColumnCnt,[Row|RestRows]):-!. %���������?



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
	   [eulerCell(Index1,cell(R,Y)),eulerCell(Index2,cell(R1,Z))|BorderedRest]):-
	(   Index1=Index2,R=n ; Index1\=Index2,R=y),
	setRightBorders([eulerCell(Index2,cell(n,Z))|Rest],[eulerCell(Index2,cell(R1,Z))|BorderedRest]),!.

setBottomBorders([X],[X]):-!.
setBottomBorders([G|R],[RG|R1]):-
	sameIndexGroup([G|R],R,G,_),
	randomizeGroup(R,RG),
	setBottomBorders(R,R1).

setBottomBorders1([X],[X]):-!.
setBottomBorders1(Before,After):-
	findall(Cells,sameIndexCells(Before,_,Cells),SameGroups),
	setBottomBordersHelper(SameGroups,SameGroupsWithBorders),
	flatten(SameGroupsWithBorders,After).

setBottomBorders1Helper([],[]):-!.
setBottomBorders1Helper([X|Y],[X1|Y1]):-
	randomizeGroup(X,X1),
	setBottomBorders1Helper(Y,Y1).

testRandomize([eulerCell(1,cell(n,n)),eulerCell(1,cell(n,n)),
	       eulerCell(1,cell(y,n))]).

testRandomize1([eulerCell(1,cell(n,y)),eulerCell(1,cell(n,n)),
	       eulerCell(1,cell(y,y))]).

test([X],[X],1):-!.
test(X,Y,N):-
   length(X,N),
   length(Y,N).

testSetBottom([eulerCell(1, cell(y, n)), eulerCell(4, cell(n, n)), eulerCell(4, cell(n, n)), eulerCell(4, cell(y, n)), eulerCell(5, cell(y, n)), eulerCell(6, cell(y, n)), eulerCell(7, cell(n, n))]).

%randomizeGroup(?Group,?RandomizedBorderedGroup)
%
randomizeGroup([X],[X]):-!.
randomizeGroup(X,Y):-
	length(Y,N),
	length(X,N),
	N1 is N+1,
	random(1,N1,NnoBorder),
	randset(NnoBorder,N,NoBorderIndexes),
	getMask(N,NoBorderIndexes,BorderMask),
	installBottomBorders(X,BorderMask,Y).
%getMask(?MaskSize,?IndexesNoBorder,?MaskArray)
%ex.:getMask(4,[2,3],X)->X=[y,n,n,y]
%
getMask(N,I,L):-getMaskH(N,I,L1),reverse(L,L1).

getMaskH(0,_,[]):-!.
getMaskH(N,I,[X|Y]):-
	member(N,I),
	N1 is N-1,
	X=n,
	getMaskH(N1,I,Y),!.

getMaskH(N,I,[X|Y]):-
	%\+ member(N,I),
	N1 is N-1,
	X=y,
	getMaskH(N1,I,Y).


%installBottomBorders(?Row,?Mask,?BorderedRow)
%
installBottomBorders([],[],[]):-!.
installBottomBorders([eulerCell(Index,cell(R,n))|X],
			       [HaveBorders|M],
			       [eulerCell(Index,cell(R,HaveBorders))|Y]):-
	installBottomBorders(X,M,Y).

%archive
sameIndexCells([],_,[]):-!.
sameIndexCells([eulerCell(Index,CellInfo)|Rest],Index,
	       [eulerCell(Index,CellInfo)|RestCells]):-
	sameIndexCells(Rest,Index,RestCells).
sameIndexCells([eulerCell(Index1,_)|Rest],Index,RestCells):-
	sameIndexCells(Rest,Index,RestCells)
	,Index1\=Index.

%sameIndexGroup1(?List,?Rest,?Group,?Index) //archive
sameIndexGroup1([],[],[],_):-!.
sameIndexGroup1([eulerCell(I1,Cell)|L],[eulerCell(I1,Cell)|L],[],I):-
	I1\=I,!.
sameIndexGroup1([eulerCell(I,Cell)|L1],R1,[eulerCell(I,Cell)|G1],I):-
	sameIndexGroup1(L1,R1,G1,I).


%sameIndexGroup(?List,?Rest,?Group,?Index)
%
sameIndexGroup(L,R,G,I):-
	sameIndexGroupH(L,R,G,I,no).
sameIndexGroupH([],[],[],_,_):-!.
sameIndexGroupH([eulerCell(I,Cell)|L1],R1,[eulerCell(I,Cell)|G1],I,_):-
	sameIndexGroupH(L1,R1,G1,I,yes).
sameIndexGroupH([eulerCell(I1,Cell)|L],[eulerCell(I1,Cell)|L],[],I,yes):-
	I1\=I,!.
sameIndexGroupH([eulerCell(I1,Cell)|L],R,G,I,no):-
	isIndex(I,[eulerCell(I1,Cell)|L]),
	I1\=I,
	sameIndexGroupH(L,R,G,I,no).

eulerToSet(E,S):-
	maplist(cellIndex,E,L),
	list_to_set(L,S).
isIndex(I,E):-
	eulerToSet(E,S),
	member(I,S).

cellIndex(eulerCell(I,_),I).


%maxIndex(?eulerList,?maxMember)
%
maxIndex(Max,E):-
	eulerToSet(E,S),
	max_member(Max,S).

%newRow(?Row,?NewRow)
%
newRow(R,Rnew):-
	newRowIndexes(R,R1),
	setBorders(R1,Rnew).

newRowIndexes([],[]):-!.
newRowIndexes([eulerCell(Index,cell(_,n))|R],[eulerCell(Index,cell(n,n))|R1]):-
	maxIndex(Max,[eulerCell(Index,cell(_,n))|R]),
	newRowIndexesH(R,R1,Max,Index,n),!.
newRowIndexes([eulerCell(Index,cell(_,y))|R],[eulerCell(NewIndex,cell(n,n))|R1]):-
	maxIndex(Max,[eulerCell(Index,cell(_,n))|R]),
	(
	    maybe,
	    NewIndex = Index,
	    newRowIndexesH(R,R1,Max,NewIndex,n),!;
	    Max1 is Max+1,
	    newIndex = Max1,
	    newRowIndexesH(R,R1,Max1,NewIndex,y),!
	).



% newRowIndexesH(?Row,?R1,?CurrentMaxIndex,?LastIndex,?LastBottomBorder(..y/n))
%
%
newRowIndexesH([],[],_,_,_):-!.
newRowIndexesH([eulerCell(Index,cell(_,Y))|R],[eulerCell(NewIndex,cell(n,n))|R1],
	       Max,LastIndex,LastBB):-
	(
            LastBB=y;
	    Y=y;
	    Index\=LastIndex
	),
	maybe,
	NewIndex = LastIndex,
	newRowIndexesH(R,R1,Max,NewIndex,Y),!;

	Max1 is Max+1,
	NewIndex = Max1,
	newRowIndexesH(R,R1,Max1,NewIndex,Y),!.






















sameIndexCellsH(L,N,C):-notIsEmpty(C),sameIndexCells(L,N,C).

same(L):-maplist(=(_),L).
notIsEmpty([_]).
