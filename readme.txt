make first:

N
	makeFirstHelper(N,1,BorderFreeRow)
	setBorders(BorderFreeRow,Row)
N--	

body:

if(N>1)
	newRowIndexes(R,R1)
	setBottomBorders(R1,Rnew)
	N--
else
	newRowIndexes(B,A1)
	finishRowH(A1,A)
	break
	