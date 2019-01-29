


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

int ZHOU::GetAllDigits(int cell){
	int ir = 0, xcell = C_To128[cell];;
	for (int i = 0; i < 9; i++) if (FD[i][0].On(xcell))ir |= (1 << i);
	return ir;
}
void ZHOU::ImageCandidats() {// only active digits ??
	int dig_cells[81]; 
	for (int i = 0; i < 81; i++) 
		dig_cells[i] = GetAllDigits(i);
	int i, j, l, lcol[9], tcol = 0,ncand=0;
	cout <<"PM map "<<endl<<endl;
	for(i = 0; i < 9; i++) {  // attention ici i indice colonne
		lcol[i] = 2;    // 2  mini tous chiffres impos�s
		for(j = 0; j < 9; j++) {
			l = _popcnt32(dig_cells[9 * j + i]);
			if(l > lcol[i])       lcol[i] = l;
		}
		tcol += lcol[i];
	}
	for(i = 0; i < 9; i++) {
		if((i == 3) || (i == 6))cout <<"|";
		cout <<(char)('A' + i)<<Blancs(lcol[i], 1);
	}
	cout << endl;
	for(i = 0; i < 9; i++) { // maintenant indice ligne
		if((i == 3) || (i == 6)) {
			for(int ix = 0; ix < (tcol + 10); ix++)       cout <<(char)'-';
			cout << endl;
		}
		for(j = 0; j < 9; j++) {
			if ((j == 3) || (j == 6))cout << "|";
			int cell = 9 * i + j, digs = dig_cells[cell], ndigs = _popcnt32(digs);
			ncand += ndigs;
			for (int id = 0; id < 9; id++)if (digs & (1 << id))
				cout << id + 1;
			cout <<Blancs(lcol[j] + 1 - ndigs, 1);
		} // end for j
		cout << endl;
	} // end for i
	cout << endl;

}

