/*In line small functions to put bits in table
 in many cases,  the bit fields have a small number of bits set to 1
 in the program it appeared easier to first create the table of such bits index 
 the bitscan family of intrinsic instructions is a good way to do this

 here below some small inline functions to do it
 The calling program parameters are
 int * t the pointer to the  index table
 int & nt the number of index already occupied (0 at the start)
 value the value 32 bits or 64 bits ot scan

 v0 is the relative index for the first bit in the value
    eg: v0 shoul be set to 27 if value is the second band 
	for a bitfields of 81 cells split in 3 bands

 */

inline void BitsInTable64(int * t, int & nt, uint64_t v, int v0=0) {
	register uint64_t R = v;
	uint32_t register res, R0 = v0;
	while (R) {
		bitscanforward64(res, R);
		t[nt++] = res + R0;
		register uint64_t bit = 1; bit <<= res;
		R ^= bit;
	}
}
inline void BitsInTable32(int * t, int & nt, uint32_t v, int v0=0) {
	register uint32_t R = v, res, R0 = v0;
	while (R) {
		bitscanforward(res, R);
		t[nt++] = res + R0;
		R ^= 1 << res;
	}
}
inline void BitsInTable64Reverse(int * t, int & nt, uint64_t v, int v0 = 0) {
	register uint64_t R = v;
	uint32_t register res, R0 = v0;
	while (R) {
		bitscanreverse64(res, R);
		t[nt++] = res + R0;
		register uint64_t bit = 1; bit <<= res;
		R ^= bit;
	}
}
inline void BitsInTable32Reverse(int * t, int & nt, uint32_t v, int v0 = 0) {
	register uint32_t R = v, res, R0 = v0;
	while (R) {
		bitscanreverse(res, R);
		t[nt++] = res + R0;
		R ^= 1 << res;
	}
}


/* 
BF128 a bit field of size 128 bits used  to work with 128 bits registers
      mostly used as 3x32   bit fields for a sudoku 81 bits field
	  this code is designed to work as much as possible in 64 bits mode
	  (but the main process is uses 32 bits fields)
*/
class BF128 {
public:
	T128 bf;
	BF128() {}
	BF128(const BF128 &v) { bf.u128 = v.bf.u128; }
	BF128(const __m128i &v) { bf.u128 = v; }
	BF128(const T128 &v) { bf.u128 = v.u128; }

	inline void clear() { bf.u64[0] = bf.u64[1] = 0; }
	inline void SetAll_0() { bf.u64[0] = bf.u64[1] = 0; };
	inline void SetAll_1() { bf.u64[0] = bf.u64[1] = BIT_SET_64; };

	inline BF128 operator| (const BF128 &r) const { BF128 w; w.bf.u128 = _mm_or_si128(bf.u128, r.bf.u128); return w; }
	inline BF128 operator| (const __m128i r) const { BF128 w; w.bf.u128 = _mm_or_si128(bf.u128, r); return w; }
	inline void operator|= (const BF128& r) { bf.u128 = _mm_or_si128(bf.u128, r.bf.u128); }
	inline void operator|= (const __m128i r) { bf.u128 = _mm_or_si128(bf.u128, r); }

	inline BF128 operator& (const BF128 &r) const { BF128 w; w.bf.u128 = _mm_and_si128(bf.u128, r.bf.u128); return w; };
	inline BF128 operator& (const __m128i r) const { BF128 w; w.bf.u128 = _mm_and_si128(bf.u128, r); return w; }
	inline void operator&= (const BF128& r) { bf.u128 = _mm_and_si128(bf.u128, r.bf.u128); }
	inline void operator&= (const __m128i r) { bf.u128 = _mm_and_si128(bf.u128, r); }

	inline BF128 operator^ (const BF128 &r) const { BF128 w; w.bf.u128 = _mm_xor_si128(bf.u128, r.bf.u128); return w; }
	inline BF128 operator^ (const __m128i r) const { BF128 w; w.bf.u128 = _mm_xor_si128(bf.u128, r); return w; }
	inline void operator^= (const BF128& r) { bf.u128 = _mm_xor_si128(bf.u128, r.bf.u128); }
	inline void operator^= (const __m128i r) { bf.u128 = _mm_xor_si128(bf.u128, r); };

	inline BF128 operator- (const BF128 &r) const { BF128 w; w.bf.u128 = _mm_andnot_si128(r.bf.u128, bf.u128); return w; }
	inline BF128 operator- (const __m128i r) const { BF128 w; w.bf.u128 = _mm_andnot_si128(r, bf.u128); return w; }
	inline void operator-= (const BF128& r) { bf.u128 = _mm_andnot_si128(r.bf.u128, bf.u128); }
	inline void clearBits(const BF128& r) { bf.u128 = _mm_andnot_si128(r.bf.u128, bf.u128); }
	inline void operator-= (const __m128i r) { bf.u128 = _mm_andnot_si128(r, bf.u128); };

	inline void operator= (const BF128 &r) { bf.u128 = r.bf.u128; }
	inline void operator= (const void *p) { bf.u128 = _mm_loadu_si128((const __m128i*)p); }

	inline bool operator== (const BF128& r) const { return(bf.u64[0] == r.bf.u64[0] && bf.u64[1] == r.bf.u64[1]); }
	inline bool operator!= (const BF128 &r) const { return(bf.u64[0] != r.bf.u64[0] || bf.u64[1] != r.bf.u64[1]); };
	inline void MaskToBit(const int theBit) {
		register int R = theBit;		if (R >= 128)SetAll_1();
		else if (R <= 0)SetAll_0();
		else if (R < 64) {
			bf.u64[0] = 0;
			bf.u64[0] = (1 << R) - 1;
		}
		else {
			bf.u64[0] = 0;
			bf.u64[0] = (1 << (R - 64)) - 1;
		}
	}
	inline void Mask(const int theBit) { BF128 w; w.MaskToBit(theBit); *this &= w; }

#ifndef _MSC_VER
	inline void Set(const int theBit) {
		if (theBit < 64) {
			bf.u64[0] |= (uint64_t)1 << theBit;
		}
		else {
			bf.u64[1] |= (uint64_t)1 << (theBit - 64);
		}
	}
	inline void SetToBit(const int theBit) { 
		if (theBit < 64) {
			bf.u64[1] = (uint64_t)0;
			bf.u64[0]=(uint64_t) 1<< theBit;
		}
		else {
			bf.u64[0] = (uint64_t)1;
			bf.u64[1] = (uint64_t)1 << (theBit-64);
		}
	}
	inline int On(const int theBit) const { 
		if (theBit<64)	return  ((uint64_t)bf.u64[0] >> theBit) & 1;
		else return  ((uint64_t)bf.u64[1] >> (theBit-64)) & 1;
	}

	inline void Clear(const int theBit) {
		if (theBit < 64) {
			bf.u64[0] &= ~((uint64_t)1 << theBit)	;
		}
		else {
			bf.u64[1] &=~( (uint64_t)1 << (theBit - 64));
		}
	}
#else
	inline void Set(const int theBit) { _bittestandset64((long long*)&bf.u64[0], theBit); }
	inline void SetToBit(const int theBit) { clear(); _bittestandset64((long long*)&bf.u64[0], theBit); }
	inline unsigned char On(const int theBit) const { return  _bittest64((long long*)&bf.u64[0], theBit); }
	inline void Clear(const int theBit) { _bittestandreset64((long long*)&bf.u64[0], theBit); }
#endif


	inline int Off(const int theBit) const { return !On(theBit); }
	inline int isBitSet(const int theBit) const { return !On(theBit); }// double definition to clear
	inline void setBit(const int theBit) { Set(theBit); }// double definition to clear
	inline void clearBit(const int theBit) { Clear(theBit); }// double definition to clear

	//  code to use in a 3 bands pattern calling using a cell 0-80
	inline int On_c(const int cell) const { return On(C_To128[cell]); }
	inline int Off_c(const int cell) const { return Off(C_To128[cell]); }
	inline void Set_c(const int cell) { Set(C_To128[cell]); }
	inline void Clear_c(const int cell) { Clear(C_To128[cell]); }
	inline void SetDiagX(const int theBit){		Set_c(C_transpose_d[From_128_To_81[theBit]]);	}
	void ClearDiag(int clear, int stack);
	void ClearRow(int clear, int row);
	void ClearCol(int clear, int col);

	inline bool isZero() const { return bf.u64[0] == 0 && bf.u64[1] == 0; }
	inline bool isEmpty() const { return bf.u64[0]==0 && bf.u64[1] == 0; }
	inline bool isNotEmpty() const { return bf.u64[0] != 0 || bf.u64[1] != 0; }

	inline int Count(){ return (int)(_popcnt64(bf.u64[0]) + _popcnt64(bf.u64[1])); }
	inline int Count96(){ return (int)(_popcnt64(bf.u64[0]) + _popcnt32(bf.u32[2])); }

	inline int isSubsetOf(const BF128 &s) const { return  _mm_testc_si128(s.bf.u128, bf.u128); }
	inline int isDisjoint(const BF128& r) const { return _mm_test_all_zeros(r.bf.u128, bf.u128); }
	inline int Disjoint(const BF128& r) const { return _mm_test_all_zeros(r.bf.u128, bf.u128); }
	inline int SupersetOf(const BF128 &s) const { return  _mm_testc_si128(bf.u128, s.bf.u128); }

	inline int SupersetOf81(const BF128 &s) const { BF128 w = s; w.Mask(81); return  SupersetOf(w); }
	inline int SupersetOf96(const BF128 &s) const { BF128 w = s; w.Mask(96); return  SupersetOf(w); }
	inline bool operator< (const BF128 &rhs) const {
		if (bf.u64[1] < rhs.bf.u64[1]) return true;
		if (bf.u64[1] > rhs.bf.u64[1]) return false;
		return bf.u64[0] < rhs.bf.u64[0];
	}
	inline int Compare(const BF128 &r) const {
		if (*this == r) return 0;
		if (bf.u64[1] == r.bf.u64[1]){
			if (bf.u64[0] < r.bf.u64[0]) return -1;
			else return 1;
		}
		if (bf.u64[1] < r.bf.u64[1]) return -1;
		else return 1;
	}

	inline void Convert3X27to81(const BF128 & rhs){
		register uint64_t R0 = rhs.bf.u32[0],
			R1 = rhs.bf.u32[1],
			R2 = rhs.bf.u32[2];
		R1 <<= 27; R0 |= R1;
		R1 = R2; R1 <<= 54;
		bf.u64[0] = R0 | R1;
		bf.u64[1] = R2 >> 10;
	}
	inline void Convert81to3X27(const T128 & rhs){
		register uint64_t R0 = rhs.u64[0],
			R1 = rhs.u64[1];
		bf.u32[0] = R0 & BIT_SET_27;
		R0 >>= 27;
		bf.u32[1] = R0 & BIT_SET_27;
		R0 >>= 27;
		R1 <<= 10;
		R0 |= R1;
		bf.u32[2] = R0 & BIT_SET_27;
		bf.u32[3] = 0;
	}
	
	inline int mask8() const { return _mm_movemask_epi8(bf.u128); }
	inline int getFirst96() const {
		uint32_t res;
		if (bf.u64[0]) {
			bitscanforward64(res, bf.u64[0]);
			return res;
		}
		if (bf.u32[2]) {
			bitscanforward(res, bf.u32[2]);
			return 64 + res;
		}
		return -1;
	}
	inline int getLastCell() const {
		uint32_t res;
		if (bf.u32[2]) {
			bitscanreverse(res, bf.u32[2]);
			return 54 + res;
		}
		if (bf.u32[1]) {
			bitscanreverse(res, bf.u32[1]);
			return 27 + res;
		}
		if (bf.u32[0]) {
			bitscanreverse(res, bf.u32[0]);
			return res;
		}
		return -1;
	}
	inline int getFirst128() const {
		uint32_t res;
		if (bf.u64[0]) {
			bitscanforward64(res, bf.u64[0]);
			return res;
		}
		if (bf.u64[1]) {
			bitscanforward64(res, bf.u64[1]);
			return res+64;
		}
		return -1;
	}
	inline int getFirstCell() const {
		uint32_t res;
		if (bf.u32[0]) {
			bitscanforward(res, bf.u32[0]);
			return res;
		}
		if (bf.u32[1]) {
			bitscanforward(res, bf.u32[1]);
			return 27 + res;
		}
		if (bf.u32[2]) {
			bitscanforward(res, bf.u32[2]);
			return 54 + res;
		}
		return -1;
	}
	inline int getLast128() const {
		uint32_t res;
		if (bf.u64[1]) {
			bitscanreverse64(res, bf.u64[1]);
			return res + 64;
		}
		if (bf.u64[0]) {
			bitscanreverse64(res, bf.u64[0]);
			return res;
		}
		return -1;
	}
	void Diag3x27(BF128 & r);
	inline void Store(USHORT * tstore){ memcpy(tstore, this, 16); }
	inline void Re_Load(USHORT * tstore){ memcpy(this, tstore, 16); }

	int Table3X27(int * r);// cell 0_80 as output
	int Table128(int * r);// cell 0_127 as output
	int Table64_0(int * r);//  0_63 as output
	int Table64_1(int * r);//  64_127 as output


	char * String3X(char * ws);
	char * String3X_Rev(char * ws);
	char * String128(char * ws);

};

