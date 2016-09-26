% borders(border_right,border_bottom) - клетка содержит
% информацию имеет ли она правую//нижнюю границу.
bordersExample(borders(y,n)).

% Представление лабиринта.
%    В данном представлении, границы вокруг лабиринта
% не учитываются. Предполагается, что средства отображения понимают,
% как их отрисовывать(зная вход/выход/структуру).
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

eulerCellsRow1([ eulerCell(1, cell(n, y)),
		eulerCell(1, cell(y, n)),
		eulerCell(2, cell(n, y)),
		eulerCell(2, cell(y, n)),
		eulerCell(2, cell(n, y)),
		eulerCell(2, cell(y, n)),
		eulerCell(3, cell(n, n)) ]).


% main predicate
%
mazeGenerator(Rows,Columns,maze(Entrace,Exit,Matrix)):-
	matrixGenerator(Rows,Columns,Matrix),
	entraceGenerator(Rows,Columns,Entrace,Exit).


entraceGenerator(R,C,cell(InX,InY),cell(OutX,OutY)):-
	(
        maybe,
        random(1,R,InX),
        InY = 0;

	InX = 0,
	random(1,C,InY)
	),

	OutX is R - InX - 1,
	OutY is C - InY - 1.


%
matrixGenerator(0,_,[]):-!.
matrixGenerator(1,0,[]):-!.
matrixGenerator(1,C,[cell(n,n)|R]):-
	C1 is C-1,
	matrixGenerator(1,C1,R),!.

matrixGenerator(N,C,R):-
	makeFirst(C,F),
	N1 is N-1,
	matrixGeneratorH(N1,C,L,F),
	maplist(maplist(fromEuler),L,R).
%matrixGeneratorH(?nRow,?nCol,?matrix,?lastRow)
%
matrixGeneratorH(0,_,[F],F):-!.
matrixGeneratorH(1,_,[L|R],L):-
	finishRow(L,R).
matrixGeneratorH(N,C,[L|R],L):-
	N1 is N-1,
	newRow(L,L1),
	matrixGeneratorH(N1,C,R,L1).



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

%(i,o)
setBottomBorders([],[]):-!.
setBottomBorders(L, RA):-
	sameIndexGroup(L,R,G,_),!,
	randomizeGroup(G,RG),
	setBottomBorders(R,R1),
	append(RG,R1,RA).

testRandomize2([eulerCell(4, cell(n, n)), eulerCell(4, cell(n, n)), eulerCell(4, cell(y, n))]).

%randomizeGroup(?Group,?RandomizedBorderedGroup)
%
randomizeGroup([X],[X]):-!.
randomizeGroup(X,Y):-
	%is_list(X),
	%is_list(Y),
	length(Y,N),
	length(X,N),
	N1 is N+1,
	random(1,N1,NnoBorder),
	randset(NnoBorder,N,NoBorderIndexes),
	getMask(N,NoBorderIndexes,BorderMask),
	(
	    installBottomBorders(X,BorderMask,Y),!;
	    installBottomBorders(X,_,Y),!
	).


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

testSetBottom1([eulerCell(1, cell(y, n))]).


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
fromEuler(eulerCell(_,C),C).


%maxIndex(?eulerList,?maxMember)
%
maxIndex(Max,E):-
	eulerToSet(E,S),
	max_member(Max,S).

myrow([eulerCell(1, cell(y, n)), eulerCell(2, cell(y, n)), eulerCell(3, cell(y, n)), eulerCell(4, cell(n, n)), eulerCell(4, cell(n, n))]).
myrow1([eulerCell(1, cell(y, n)), eulerCell(2, cell(n, n)), eulerCell(2, cell(n, n)), eulerCell(2, cell(y, n)), eulerCell(3, cell(n, n))]).

%newRow(?Row,?NewRow)
%
newRow(R,Rnew):-
	newRowIndexes(R,R1),
	setBorders(R1,Rnew).

newRowIndexes([],[]):-!.
newRowIndexes(B,A):-
	maxIndex(Max, B),
	updateIndexes(B,A1,Max),
	randomJoinIndexesNew(A1,A).
%updateIndexes(?BeforeRow,?AfterRow,?MaxHelper)
updateIndexes([],[],_):-!.
updateIndexes([eulerCell(Index,cell(_,n))|B],[eulerCell(Index,cell(n,n))|A],Max):-
	updateIndexes(B,A,Max).
updateIndexes([eulerCell(_,cell(_,y))|B],[eulerCell(Max1,cell(n,n))|A],Max):-
	Max1 is Max+1,
	updateIndexes(B,A,Max1).

randomJoinIndexesNew([],[]):-!.
randomJoinIndexesNew([X],[X]):-!.
randomJoinIndexesNew([eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B],A):-
	maxIndex(Max,[eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B]),
	rJIN([eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B],A,Index0,Max),!.
randomJoinIndexesNew(B,A):-
	maxIndex(Max,B),
	Max1 is Max+1,
	rJIN(B,A,Max1,Max),!.




%
rJIN([],[],_,_):-!.
rJIN([X],[X],_,_):-!.
rJIN([eulerCell(Index0,cell(_,_)),eulerCell(Index1,cell(_,_))|B],[eulerCell(Index0,cell(n,n)),eulerCell(Index2,cell(n,n))|A],Index1,Max):-
	Index2 is Max+1,
	rJIN([eulerCell(Index2,cell(n,n))|B],[eulerCell(Index2,cell(n,n))|A],Index1,Index2),
	!.
rJIN([eulerCell(Index1,cell(_,_)),eulerCell(Index2,cell(_,_))|B],[eulerCell(Index1,cell(n,n)),eulerCell(Index21,cell(n,n))|A],_,Max):-
	(
        maybe,
	Max1 is Max+1,
	Index21=Max1,!;

        Max1 = Max,
        Index21 = Index1,!
	),
	rJIN([eulerCell(Index21,cell(_,_))|B],[eulerCell(Index21,cell(n,n))|A],Index2,Max1),
	!.






/*
newRowIndexes1([],[]):-!.
newRowIndexes1([eulerCell(Index,cell(_,n))|R],[eulerCell(Index,cell(n,n))|R1]):-
	maxIndex(Max,[eulerCell(Index,cell(_,n))|R]),
	newRowIndexesH1(R,R1,Max,Index,n),!.
newRowIndexes1([eulerCell(Index,cell(_,y))|R],[eulerCell(NewIndex,cell(n,n))|R1]):-
	maxIndex(Max,[eulerCell(Index,cell(_,n))|R]),
	(
	    maybe,
	    NewIndex = Index,
	    newRowIndexesH1(R,R1,Max,Index,n),!;

	    Max1 is Max+1,
	    NewIndex = Max1,
	    newRowIndexesH1(R,R1,Max1,NewIndex,y),!
	).




% newRowIndexesH(?Row,?R1,?CurrentMaxIndex,?LastIndex,?LastBottomBorder(..y/n))
newRowIndexesH1([],[],_,_,_):-!.
newRowIndexesH1([eulerCell(Index,cell(_,Y))|R],[eulerCell(NewIndex,cell(n,n))|R1],
	       Max,LastIndex,LastBB):-
	(
            LastBB=y;
	    Y=y;
	    Index\=LastIndex
	), maybe,
	NewIndex = LastIndex,
	newRowIndexesH1(R,R1,Max,NewIndex,Y),!;

	Max1 is Max+1,
	NewIndex = Max1,
	newRowIndexesH1(R,R1,Max1,NewIndex,Y),!.

*/
finishRow(B,A):-
	newRowIndexes(B,A1),
	finishRowH(A1,A).


%finishRow(?Row,?FinishRow)
%
finishRowH([],[]):-!.
finishRowH([X],[X]):-!.
finishRowH([eulerCell(Index1,cell(Y,_)),eulerCell(Index1,cell(Z,_))|Rest],
	   [eulerCell(Index1,cell(Y,n)),eulerCell(Index1,cell(Z,n))|BorderedRest]):-
	finishRowH([eulerCell(Index1,cell(Z,n))|Rest],[eulerCell(Index1,cell(Z,n))|BorderedRest]),!.
finishRowH([eulerCell(Index1,_),eulerCell(Index2,cell(Y,_))|Rest],
	  [eulerCell(Index1,cell(n,n)),eulerCell(Index2,cell(Z,n))|BorderedRest])
:-
	finishRowH([eulerCell(Index2,cell(Y,n))|Rest],
			[eulerCell(Index2,cell(Z,n))|BorderedRest]).


















sameIndexCellsH(L,N,C):-notIsEmpty(C),sameIndexCells(L,N,C).

same(L):-maplist(=(_),L).
notIsEmpty([_]).
