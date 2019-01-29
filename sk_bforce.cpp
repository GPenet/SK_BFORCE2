
#define _CRT_SECURE_NO_DEPRECATE
//#define ZHOU_OLD
#include <sys/timeb.h>
#include "main.h"  // tab0  

uint64_t p_cptg[40]; 

extern ZHOU    zhou[50];// , zhou_i, zhou_solve;
extern ZH_GLOBAL zh_g;
extern ZH_GLOBAL2 zh_g2;
extern SGO sgo;

ofstream  fout1,fout2;
FINPUT finput;

//========================================
//const char * zh_g_cpt[10] = { "npuz", "guess", "close_d ", "upd1 ", "upd2 ",
//"fupd ", "hpair ", "htripl ", " ", " " };

void Go_c10() { // extract valid puzzles from entry
	int filter = sgo.bfx[0];
	if (!sgo.finput_name) return;
	cout << "brute force Go_10 entry " << sgo.finput_name << " input" << endl;
	finput.open(sgo.finput_name);
	if (!finput.is_open()) {
		cerr << "error open file " << sgo.finput_name << endl;
		return;
	}
	char ze[200]; ze[81] = 0;
	while (finput.GetPuzzle(ze)) {
		int ir = zhou[0].CheckValidityQuick(ze);
		if ((!filter) && ir == 1)	fout1 << ze << endl;
		else if((filter &1) && ir==0)fout1 << ze << endl;
		else if (ir > 1)fout1 << ze << endl;
	}

}
void Go_c11() { // count valid puzzles in entry
	if (!sgo.finput_name) return;
	cout << "brute force Go_10 entry " << sgo.finput_name << " input" << endl;
	finput.open(sgo.finput_name);
	if (!finput.is_open()) {
		cerr << "error open file " << sgo.finput_name << endl;
		return;
	}
	uint64_t cc[3] = { 0,0,0 };
	char ze[200]; ze[81] = 0;
	while (finput.GetPuzzle(ze)) {
		int ir=zhou[0].CheckValidityQuick(ze);
		if (ir < 0 || ir>2) ir = 0;
		cc[ir]++;
	}
	cout << "count " << cc[0] << ";" << cc[1] << ";" << cc[2] << endl;
}

void Go_0() {
	// open  outputs files 1.txt
	if (sgo.foutput_name) {
		char zn[200];
		strcpy(zn, sgo.foutput_name);
		int ll = (int)strlen(zn);
		strcpy(&zn[ll], "_file1.txt");
		fout1.open(zn);
	}
	switch (sgo.command) {
	case 10: Go_c10(); break; // extract valid puzzles from entry		
	case 11:Go_c11(); break; // count valid puzzles in entry
	}
	cerr << "go_0 return" << endl;
}

