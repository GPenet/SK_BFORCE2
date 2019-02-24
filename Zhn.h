
/*
ZhouSolver.h
Based on code posted to <http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html>
by user zhouyundong_2012.
The copyright is not specified.

this is a remorphing of the code to work in recursive mode 
*/

// tables specific to the brute force zh_tables
const extern int TblRowUnsolved[8];
const extern int TblRowMask[8];// rows where single  found  000 to 111
const extern int  Tblstartblock[27]; // (i%3) * 27 in zhou brute force
const extern int TblShrinkMask[512];// existing minirows 000 to 111
const extern int TblComplexMask[512]; // keep mini rows still valid optimised process
const extern int TblMaskSingle[512]; // kill in other blocks locked column /box
const extern int TblMaskDouble[512];// kill for locked in box / column
const extern int TblColumnSingle[512]; // single in column applied to shrinked bloc
const extern int TblShrinkSingle[512]; // keep only rows with single
const extern int TblRowUniq[512]; // 1 is row not defined in block  mode  to 111
const extern T128 AssignMask_Digit[81];
const extern T128 zhoustart[19];


/*ZH_1D class to solve one digit 3 bands
all valid solutions  are stored in table
the table is supplied by the caller
this is somehow a private class for the brute force
and this is part of the critical code in the brute force
except for easiest puzzles immediatly solved
*/
struct ZH_1D_GLOBAL {
	BF128 *tsolw, t3; // belong to the caller
	int nsolw,lim;
	ZH_1D_GLOBAL() { t3.SetAll_1(); t3.bf.u32[3] = 0; }
	inline void Add(BF128 & s) {
		*tsolw++ = s & t3; // clear last 32 bits
		nsolw++;
	}
	int Go(BF128 & fde, BF128 *tsol,int limit=5000);
};

struct ZHOU;
/* ZH_GLOBAL2  and ZH_GLOBAL are both "static variables for the brute force
to have a better cache effect, ZH_GLOBAL is strictly limited to what is required 
for the basic brute force; 
data used in other brute force uses are in ZH_GLOBAL2
*/
struct ZH_GLOBAL2 {
	BF128 Digit_cell_Assigned[9];// init sequence
	GINT16 tgiven[81];
	int ngiven, digitsbf;// digitsbf to check minimum 8 digits
	char * zsol,		stdfirstsol[82];
	char puz[82]; // the solved puzzle 
	ZH_GLOBAL2() {
		zsol = 0; // no solution unless required buy the user
	}
};
struct ZH_GLOBAL { // global variables for the core brute force

	int nsol, lim,	single_applied,	go_back,	loop;
	BF128  pairs;

	inline void Init(int maxsols){
		nsol = go_back=0;
		lim = maxsols;
	}
	int Go_InitSudoku(char * ze);
	void ValidPuzzle(ZHOU * z);
};
/* 2 BF 128 per digit
	equivalent to F and Fcomp in Zhou
	Last 32 bits in FD[0] is a  bits field for unknown rows
	Last 32 bits in FD[1]] contain  digit mapped
*/
// class encapsulating the brute force 
struct ZHOU{// size 32 bytes 
	BF128 FD[9][2];
	BF128 cells_unsolved;
//________________________________________

	int FullUpdate();
	int ApplySingleOrEmptyCells();
	void Guess();
	void GuessGo(int dig, BF128& s);
	void GuessInCell();
	void GuessFullDigit();
	int GuessHiddenBivalue();
	void Assign(int digit, int cell, int xcell);
	int Update();


	void SetaCom(int digit, int cell, int xcell);
	inline void Seta_c(int digit, int cell){ SetaCom(digit, cell, C_To128[cell]); }

	inline int Unsolved_Count(){ return cells_unsolved.Count(); }
	inline void ComputeNext(){ 	if (FullUpdate())Guess();}

	int CheckValidityQuick(char *puzzle);
	int InitSudoku(GINT16 * t, int n);
	int InitSudoku(char * zpuz);
	char * SetKnown(char * zs);

 };
 
