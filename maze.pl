% borders(border_right,border_bottom) - ������ ��������
% ���������� ����� �� ��� ������//������ �������.
bordersExample(borders(y,n)).

% ������������� ���������.
%    � ������ �������������, ������� ������ ���������
% �� �����������. ��������������, ��� �������� ����������� ��������,
% ��� �� ������������(���� ����/�����/���������).

testEntrace3([0,2],[2,0]).
testMaze3([
    [cell(n,n),cell(y,y),cell(n,n)],
    [cell(y,n),cell(n,n),cell(n,y)],
    [cell(n,n),cell(n,n),cell(n,n)] ]
    ).

testMaze2([
    [cell(y,n),cell(n,n)],
    [cell(n,n),cell(n,n)]]).
testEntrace2([0,0],[1,0]).



testEulerCellsRow([ eulerCell(1, cell(n, n)),
		eulerCell(1, cell(y, n)),
		eulerCell(2, cell(y, n)),
		eulerCell(1, cell(n, n)),
		eulerCell(1, cell(n, n)),
		eulerCell(1, cell(y, n)),
		eulerCell(3, cell(n, n)) ]).

testRandomize2([eulerCell(4, cell(n, n)), eulerCell(4, cell(n, n)), eulerCell(4, cell(y, n))]).


% main predicates
%
mazeGenerator(Rows,Columns,maze(Entrace,Exit,Matrix)):-
	matrixGenerator(Rows,Columns,EulerMatrix),
	maplist(maplist(fromEuler),EulerMatrix,Matrix),
	entraceGenerator(Rows,Columns,Entrace,Exit).

eulerMazeGenerator(R,C,maze(In,Out,EM)):-
	matrixGenerator(R,C,EM),
	entraceGenerator(R,C,In,Out).


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

%uses from C++ code
makeEulerCell(I,Rb,Bb,C):-
	C = eulerCell(I,cell(Rb,Bb)).

%matrixGenerator(?RowCount,?ColumnCount,?EulerMatrix)
matrixGenerator(0,_,[]):-!.
matrixGenerator(1,0,[]):-!.
matrixGenerator(1,C,[eulerCell(1,cell(n,n))|R]):-
	C1 is C-1,
	matrixGenerator(1,C1,R),!.

matrixGenerator(N,C,R):-
	makeFirst(C,F),
	N1 is N-1,
	matrixGeneratorH(N1,C,R,F).

%	maplist(maplist(fromEuler),L,R).

%matrixGeneratorH(?nRow,?nCol,?matrix,?lastRow)
%
%matrixGeneratorH(0,_,[F],F):-!.
matrixGeneratorH(1,_,[L|[R]],L):-
	finishRow(L,R),!.
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


%connectedEulerCells(?connectedCells,?cells)
connectedEC([],[],_):-!.
connectedEC([eulerCell(I,cell(y,B))],[eulerCell(I,cell(y,B))|R],R).
connectedEC([eulerCell(I,cell(n,B))|R],[eulerCell(I,cell(n,B))|R1],R2):-
	connectedEC(R,R1,R2).


%sameIndexGroup(?List,?Rest,?Group,?Index)
%
sameIndexGroup([],[],[],none):-!.
sameIndexGroup([eulerCell(I,Cell)|L1],R1,G1,I1):-
	sameIndexGroupH([eulerCell(I,Cell)|L1],R,G,I),
	(   R1=R,G1=G,I1=I;
	sameIndexGroup(R,R1,G1,I1) ).

sameIndexGroupH([],[],[],_):-!.
sameIndexGroupH([eulerCell(I,Cell)|L1],R1,[eulerCell(I,Cell)|G1],I):-
	sameIndexGroupH(L1,R1,G1,I),!.
sameIndexGroupH([eulerCell(I1,Cell)|R],[eulerCell(I1,Cell)|R],[],_).

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
	setBottomBorders(R1,Rnew).

newRowIndexes([],[]):-!.
newRowIndexes(B,A):-
	maxIndex(Max, B),
	updateIndexes(B,A1,Max),
	rJIN(A1,A2,JI),
	joinGroups(A2,A,JI).
%updateIndexes(?BeforeRow,?AfterRow,?MaxHelper)
updateIndexes([],[],_):-!.
updateIndexes([eulerCell(Index,cell(_,n))|B],[eulerCell(Index,cell(n,n))|A],Max):-
	updateIndexes(B,A,Max).
updateIndexes([eulerCell(_,cell(_,y))|B],[eulerCell(Max1,cell(n,n))|A],Max):-
	Max1 is Max+1,
	updateIndexes(B,A,Max1).

%problem on iterations:
% on cells connection need to join groups,
% easy to join all next cells from the same groups, but also need to
% join past cells.
% Subproblems:
% it is possible situation where groups
% 1->(joins)2,
% 1->3 (next columns, same row)
% if we will try to write simple Prolog-kind algorithm
% we will stuck to the next error:
% cells joined to the group 2, and cells joined to the group 3
% represented like cells from different groups -- mistake.

joinGroups(R,R1,JI):-
	jiToJs(JI,JS),
	joinSets(R,R1,JS).

joinSets(R,R,[]):-!.
joinSets(R,RA,[[I0|IGR]|IR]):-
	joinSomeSet(R,R1,[I0|IGR],I0),
	joinSets(R1,RA,IR).

joinSomeSet(R,R,[],_):-!.
joinSomeSet(R,RA,[I|IR],I0):-
	changeIndexes(R,R1,I,I0),
	joinSomeSet(R1,RA,IR,I0).


testMyJs([[1,2],[1,3],[4,5],[6,7],[3,7]]).

jiToJs([],[]):-!.
jiToJs([[Index1,Index2]|R],JS3):-
        jiToJs(R,JS1),
	flatten(JS1,L),
	(   member(Index1,L),
	    (	member(Index2,L),
		pasteTogether(JS1,[Index1,Index2],JS2),!;
	    JS2=JS1
	    ),
	    addToSomeSet((Index1,Index2),JS2,JS3),!;
	member(Index2,L),
	addToSomeSet((Index2,Index1),JS1,JS3),!;

	JS3=[[Index1,Index2]|JS1]
	)
	.

pasteTogether([],_,[]):-!.
pasteTogether(JS1,Indexes,JS2):-
	list_to_ord_set(Indexes,OSI),
	pTH(JS1,OSI,WO,W),
	flatten(W,FW1),
	list_to_ord_set(FW1,FW),
	JS2=[FW|WO].

pTH([],_,[],[]).
pTH([S|R],IS,WO,W):-
    pTH(R,IS,WO1,W1),
    list_to_ord_set(S,OS),
    (	ord_intersect(IS,OS),
	W=[S|W1],
	WO=WO1,!;
    W=W1,
    WO=[S|WO1]
    ).

addToSomeSet((Index1,Index2),[S|R1],[S1|R2]):-
	member(Index1,S),
	S1=[Index2|S],
	R2=R1,!;
	addToSomeSet((Index1,Index2),R1,R2),
	S=S1.



/*
randomJoinIndexesNew([],[]):-!.
randomJoinIndexesNew([X],[X]):-!.
randomJoinIndexesNew([eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B],A):-
	rJIN([eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B],A,Index0),!.
randomJoinIndexesNew(B,A):-
	maxIndex(Max,B),
	Max1 is Max+1,
	rJIN(B,A,Max1 ),!.
*/

mys([eulerCell(1,cell(y,n)),eulerCell(2,cell(y,n)),eulerCell(1,cell(n,n)),eulerCell(1,cell(y,n)),eulerCell(1,cell(n,n)),eulerCell(1,cell(n,n)),eulerCell(1,cell(n,n))]).



%
rJIN([],[],[]):-!.
rJIN([X],[X],[]):-!.
rJIN([eulerCell(Index0,cell(_,_)),eulerCell(Index0,cell(_,_))|B],[eulerCell(Index0,cell(y,n)),eulerCell(Index0,cell(X,n))|A],JI):-
	rJIN([eulerCell(Index0,cell(n,n))|B],[eulerCell(Index0,cell(X,n))|A],JI),
	!.
rJIN([eulerCell(Index1,cell(_,_)),eulerCell(Index2,cell(_,_))|B],[eulerCell(Index1,cell(X1,n)),eulerCell(Index21,cell(X2,n))|A],JI1):-
	(
        maybe,
	X1 = y,
	Index21=Index2,
        B=B1,
        JI1=JI,

        !;

        X1 = n,
        Index21=Index1,
        changeIndexes(B,B1,Index2,Index1),
        JI1=[[Index1,Index2]|JI],
        !
	),
	rJIN([eulerCell(Index21,cell(n,n))|B1],[eulerCell(Index21,cell(X2,n))|A],JI),!.

changeIndexes([],[],_,_):-!.
changeIndexes([eulerCell(Index1,Cell)|R],[eulerCell(Index2,Cell)|R1],Index1,Index2):-
	changeIndexes(R,R1,Index1,Index2),!.
changeIndexes([X|R],[X|R1],Index1,Index2):-
	changeIndexes(R,R1,Index1,Index2).





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
	   [eulerCell(Index1,cell(Y,n)),eulerCell(Index1,cell(Z1,n))|BorderedRest]):-
	finishRowH([eulerCell(Index1,cell(Z,n))|Rest],[eulerCell(Index1,cell(Z1,n))|BorderedRest]),!.
finishRowH([eulerCell(Index1,_),eulerCell(Index2,cell(Y,_))|Rest],
	  [eulerCell(Index1,cell(n,n)),eulerCell(Index21,cell(Z,n))|BorderedRest])
:-
	joinGroups([eulerCell(Index2,cell(Y,_))|Rest],[eulerCell(Index21,cell(Y,n))|Rest1],[[Index1,Index2]]),
	finishRowH([eulerCell(Index21,cell(Y,_))|Rest1],
			[eulerCell(Index21,cell(Z,n))|BorderedRest]).



sameIndexCellsH(L,N,C):-notIsEmpty(C),sameIndexCells(L,N,C).

same(L):-maplist(=(_),L).
notIsEmpty([_]).

inRange(X,Min,Max):-
	X>=Min,
	X=<Max.

%isConnected(?[?x1,?y1],?[?x2,?y2],?Max)
%
isConnected4(C1,C2,limits(R,C)):-
	isConnected(C1,C2,R,C).
isConnected([X1,Y1],[X2,Y2],R,C):-
	succ(N1,C),
	(
	       succ(X2,X1);
               succ(X1,X2)
           ),
	inRange(X1,0,N1),
	inRange(X2,0,N1),
	Y1=Y2;

	succ(N1,R),
	(
	       succ(Y2,Y1);
               succ(Y1,Y2)
	   ),
	inRange(Y1,0,N1),
	inRange(Y2,0,N1),
	X1=X2.

mazeElem(Maze,[X,Y],Elem):-
	nth0(Y,Maze,Row),nth0(X,Row,Elem).

hasRightBorder(cell(y,_)).

hasBottomBorder(cell(_,y)).

openPath(Maze,[X1,Y1],[X2,Y2]):-
	(
	    succ(X1,X2),
	    mazeElem(Maze,[X1,Y1],Elem);
	    succ(X2,X1),
	    mazeElem(Maze,[X2,Y2],Elem) ),
	\+ hasRightBorder(Elem) ;
	(   succ(Y1,Y2),
	    mazeElem(Maze,[X1,Y1],Elem);
	    succ(Y2,Y1),
	    mazeElem(Maze,[X2,Y2],Elem) ),
	\+ hasBottomBorder(Elem).


mazeLimits2(Maze,limits(R,C)):-
	mazeLimits3(Maze,R,C).
mazeLimits3(Maze,R,C):-
	length(Maze,R),nth0(0,Maze,Row),length(Row,C).

eulerFindPath(In,Out,EulerMatrix,P):-
	maplist(maplist(fromEuler),EulerMatrix,Matrix),
	findPath(In,Out,Matrix,P).


findPath2(maze(I,O,M),P):-
	findPath(I,O,M,P).
%findPath(?in,?out,?maze,?PathIndexes)
%
findPath(In,Out,Maze,[In1|P]):-
	cellIndexToIndex(In,In1),
	cellIndexToIndex(Out,Out1),
	mazeLimits2(Maze,L),
	findPathH(In1,Out1,(L,Maze),P,[In1]).

cellIndexToIndex(cell(R,C),[C,R]).

%findPathH(?f,?l,?maze,?PI,?visitedIndexes)
%
findPathH(In,In,_,[],_):-!.
findPathH(In,Out,(Limits,Maze),[Next|P],V):-
	isConnected4(In,Next,Limits),
	\+ member(Next,V),
	openPath(Maze,In,Next),
	findPathH(Next,Out,(Limits,Maze),P,[Next|V])
	.

comma:-
	mazeGenerator(30,30,maze(I,O,M)),cellIndexToIndex(I,I1),cellIndexToIndex(O,O1),print('build '),findPath(I1,O1,M,P),print(P).

bugCaseMaze([[cell(y, n), cell(y, n), cell(y, n), cell(n, n)], [cell(n, y), cell(n, y), cell(n, y), cell(n, n)], [cell(n, n), cell(y, n), cell(n, n), cell(n, n)], [cell(y, n), cell(n, n), cell(y, n), cell(n, n)]]).
bugCaseEntrace(cell(0, 3),cell(3, 0)).
testComma:-
	bugCaseMaze(M),bugCaseEntrace(I,O),findPath(I,O,M,P),print(P).
