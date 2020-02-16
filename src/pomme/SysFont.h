namespace Pomme::SysFont {
	static const int firstCodepoint = 32;
	static const int lastCodepoint = 126;
	static const int charSpacing = 2;
	static const int rows = 14;
	static const int leftMargin = 2;
	static const int ascend = 12;
	static const int descend = 2;
	static const int widthBits = 16;

	static const struct Glyph {
		short width;
		unsigned short bits[14];
	} glyphs[95] = {
		{  2, {    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 } }, //space
		{  3, {    0,   0,   0,  24,  24,  24,  24,  24,  24,  24,   0,  24,   0,   0 } }, //! 
		{  4, {    0,   0,   0,  40,  40,  40,   0,   0,   0,   0,   0,   0,   0,   0 } }, //" 
		{  8, {    0,   0, 288, 288,1016, 144, 144, 144, 508,  72,  72,   0,   0,   0 } }, //# 
		{  7, {    0,   0,  32, 504,  44,  44,  44, 248, 416, 416, 416, 252,  32,   0 } }, //$ 
		{  9, {    0,   0,   0, 256, 280, 164, 164,  88, 832,1184,1184, 784,  16,   0 } }, //% 
		{  7, {    0,   0,   0, 120,  76,  12, 408, 204, 204, 204, 204, 120,   0,   0 } }, //& 
		{  2, {    0,   0,   0,  12,  12,   8,   4,   0,   0,   0,   0,   0,   0,   0 } }, //' 
		{  4, {    0,   0,  48,  24,  24,  12,  12,  12,  12,  12,  24,  24,  48,   0 } }, //( 
		{  4, {    0,   0,  12,  24,  24,  48,  48,  48,  48,  48,  24,  24,  12,   0 } }, //) 
		{  7, {    0,   0,   0,   0,   0, 216, 112, 508, 112, 216,   0,   0,   0,   0 } }, //* 
		{  6, {    0,   0,   0,   0,   0,  48,  48, 252, 252,  48,  48,   0,   0,   0 } }, //+ 
		{  2, {    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12,  12,   8,   4 } }, //, 
		{  4, {    0,   0,   0,   0,   0,   0,   0,  60,  60,   0,   0,   0,   0,   0 } }, //- 
		{  3, {    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  24,  24,   0,   0 } }, //. 
		{  6, {    0,   0,   0, 192, 192,  96,  96,  48,  48,  24,  24,  12,  12,   0 } }, /// 
		{  6, {    0,   0,   0, 120, 204, 204, 236, 220, 204, 204, 204, 120,   0,   0 } }, //0 
		{  6, {    0,   0,   0,  56,  48,  48,  48,  48,  48,  48,  48, 252,   0,   0 } }, //1 
		{  6, {    0,   0,   0, 124, 192, 192, 192,  96,  48,  24,  12, 252,   0,   0 } }, //2 
		{  6, {    0,   0,   0, 124, 192, 192, 112, 192, 192, 192, 192, 124,   0,   0 } }, //3 
		{  6, {    0,   0,   0, 192, 224, 208, 200, 204, 204, 252, 192, 192,   0,   0 } }, //4 
		{  6, {    0,   0,   0, 252,  12,  12, 124, 192, 192, 192, 192, 124,   0,   0 } }, //5 
		{  6, {    0,   0,   0, 248,  12,  12, 124, 204, 204, 204, 204, 120,   0,   0 } }, //6 
		{  6, {    0,   0,   0, 252, 192, 224, 112,  56,  24,  24,  24,  24,   0,   0 } }, //7 
		{  6, {    0,   0,   0, 120, 204, 204, 204, 120, 204, 204, 204, 120,   0,   0 } }, //8 
		{  6, {    0,   0,   0, 120, 204, 204, 204, 248, 192, 192, 192, 120,   0,   0 } }, //9 
		{  3, {    0,   0,   0,   0,   0,   0,  24,  24,   0,   0,  24,  24,   0,   0 } }, //: 
		{  3, {    0,   0,   0,   0,   0,   0,  24,  24,   0,   0,  24,  24,  16,   8 } }, //; 
		{  4, {    0,   0,   0,   0,   0,  32,  48,  24,  12,  24,  48,  32,   0,   0 } }, //< 
		{  5, {    0,   0,   0,   0,   0,   0,   0, 124,   0, 124,   0,   0,   0,   0 } }, //= 
		{  4, {    0,   0,   0,   0,   0,   4,  12,  24,  48,  24,  12,   4,   0,   0 } }, //> 
		{  7, {    0,   0,   0, 240, 408, 408, 448, 224,  96,  96,   0,  96,   0,   0 } }, //? 
		{  7, {    0,   0,   0,   0, 240, 264, 356, 340, 244,   4,  56,   0,   0,   0 } }, //@ 
		{  6, {    0,   0,   0, 120, 204, 204, 204, 252, 204, 204, 204, 204,   0,   0 } }, //A 
		{  6, {    0,   0,   0, 124, 204, 204, 204, 124, 204, 204, 204, 124,   0,   0 } }, //B 
		{  5, {    0,   0,   0, 120,  12,  12,  12,  12,  12,  12,  12, 120,   0,   0 } }, //C 
		{  6, {    0,   0,   0, 124, 204, 204, 204, 204, 204, 204, 204, 124,   0,   0 } }, //D 
		{  5, {    0,   0,   0, 124,  12,  12,  12,  60,  12,  12,  12, 124,   0,   0 } }, //E 
		{  5, {    0,   0,   0, 124,  12,  12,  12,  60,  12,  12,  12,  12,   0,   0 } }, //F 
		{  6, {    0,   0,   0, 248,  12,  12,  12, 204, 204, 204, 204, 184,   0,   0 } }, //G 
		{  6, {    0,   0,   0, 204, 204, 204, 204, 252, 204, 204, 204, 204,   0,   0 } }, //H 
		{  4, {    0,   0,   0,  60,  24,  24,  24,  24,  24,  24,  24,  60,   0,   0 } }, //I 
		{  6, {    0,   0,   0, 192, 192, 192, 192, 192, 192, 204, 204, 120,   0,   0 } }, //J 
		{  7, {    0,   0,   0, 396, 204, 108,  60,  28,  60, 108, 204, 396,   0,   0 } }, //K 
		{  5, {    0,   0,   0,  12,  12,  12,  12,  12,  12,  12,  12, 124,   0,   0 } }, //L 
		{ 10, {    0,   0,   0,2044,3276,3276,3276,3276,3276,3276,3276,3276,   0,   0 } }, //M 
		{  6, {    0,   0,   0, 124, 204, 204, 204, 204, 204, 204, 204, 204,   0,   0 } }, //N 
		{  6, {    0,   0,   0, 120, 204, 204, 204, 204, 204, 204, 204, 120,   0,   0 } }, //O 
		{  6, {    0,   0,   0, 124, 204, 204, 204, 124,  12,  12,  12,  12,   0,   0 } }, //P 
		{  6, {    0,   0,   0, 120, 204, 204, 204, 204, 204, 204, 204, 120, 224,   0 } }, //Q 
		{  6, {    0,   0,   0, 124, 204, 204, 204, 124, 204, 204, 204, 204,   0,   0 } }, //R 
		{  6, {    0,   0,   0, 248,  12,  12,  28, 120, 224, 192, 192, 124,   0,   0 } }, //S 
		{  6, {    0,   0,   0, 252,  48,  48,  48,  48,  48,  48,  48,  48,   0,   0 } }, //T 
		{  6, {    0,   0,   0, 204, 204, 204, 204, 204, 204, 204, 204, 120,   0,   0 } }, //U 
		{  6, {    0,   0,   0, 204, 204, 204, 204, 204, 204, 204,  76,  60,   0,   0 } }, //V 
		{ 10, {    0,   0,   0,3276,3276,3276,3276,3276,3276,3276,1228,1020,   0,   0 } }, //W 
		{  6, {    0,   0,   0, 204, 204, 204, 204, 120, 204, 204, 204, 204,   0,   0 } }, //X 
		{  6, {    0,   0,   0, 204, 204, 204, 204, 120,  48,  48,  48,  48,   0,   0 } }, //Y 
		{  6, {    0,   0,   0, 252, 192, 192,  96,  48,  24,  12,  12, 252,   0,   0 } }, //Z 
		{  3, {    0,   0,  28,  12,  12,  12,  12,  12,  12,  12,  12,  12,  28,   0 } }, //[ 
		{  6, {    0,   0,   0,  12,  12,  24,  24,  48,  48,  96,  96, 192, 192,   0 } }, //backslash
		{  3, {    0,   0,  28,  24,  24,  24,  24,  24,  24,  24,  24,  24,  28,   0 } }, //] 
		{  7, {    0,   0,   0,  32, 112, 216, 396,   0,   0,   0,   0,   0,   0,   0 } }, //^ 
		{  5, {    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 124,   0 } }, //_ 
		{  2, {    0,   0,   0,  12,  12,   4,   8,   0,   0,   0,   0,   0,   0,   0 } }, //` 
		{  6, {    0,   0,   0,   0,   0, 120, 192, 248, 204, 204, 204, 184,   0,   0 } }, //a 
		{  6, {    0,   0,   0,  12,  12, 124, 204, 204, 204, 204, 204, 116,   0,   0 } }, //b 
		{  5, {    0,   0,   0,   0,   0, 120,  12,  12,  12,  12,  12, 120,   0,   0 } }, //c 
		{  6, {    0,   0,   0, 192, 192, 248, 204, 204, 204, 204, 204, 184,   0,   0 } }, //d 
		{  6, {    0,   0,   0,   0,   0, 120, 204, 204, 252,  12,  12, 248,   0,   0 } }, //e 
		{  5, {    0,   0,   0, 112,  24,  60,  24,  24,  24,  24,  24,  24,   0,   0 } }, //f 
		{  6, {    0,   0,   0,   0,   0, 184, 204, 204, 204, 204, 204, 248, 192, 124 } }, //g 
		{  6, {    0,   0,   0,  12,  12, 108, 220, 204, 204, 204, 204, 204,   0,   0 } }, //h 
		{  2, {    0,   0,   0,  12,   0,  12,  12,  12,  12,  12,  12,  12,   0,   0 } }, //i 
		{  2, {    0,   0,   0,  12,   0,  12,  12,  12,  12,  12,  12,  12,  12,   6 } }, //j 
		{  6, {    0,   0,   0,  12,  12, 204, 108,  60,  28,  60, 108, 204,   0,   0 } }, //k 
		{  3, {    0,   0,   0,  12,  12,  12,  12,  12,  12,  12,  12,  24,   0,   0 } }, //l 
		{ 10, {    0,   0,   0,   0,   0,1908,3276,3276,3276,3276,3276,3276,   0,   0 } }, //m 
		{  6, {    0,   0,   0,   0,   0, 116, 204, 204, 204, 204, 204, 204,   0,   0 } }, //n 
		{  6, {    0,   0,   0,   0,   0, 120, 204, 204, 204, 204, 204, 120,   0,   0 } }, //o 
		{  6, {    0,   0,   0,   0,   0, 116, 204, 204, 204, 204, 204, 124,  12,  12 } }, //p 
		{  6, {    0,   0,   0,   0,   0, 184, 204, 204, 204, 204, 204, 248, 192, 192 } }, //q 
		{  5, {    0,   0,   0,   0,   0, 108,  28,  12,  12,  12,  12,  12,   0,   0 } }, //r 
		{  6, {    0,   0,   0,   0,   0, 248,  12,  12, 120, 192, 192, 124,   0,   0 } }, //s 
		{  4, {    0,   0,   0,  24,  24,  60,  24,  24,  24,  24,  24,  48,   0,   0 } }, //t 
		{  6, {    0,   0,   0,   0,   0, 204, 204, 204, 204, 204, 204, 184,   0,   0 } }, //u 
		{  6, {    0,   0,   0,   0,   0, 204, 204, 204, 204, 204,  76,  60,   0,   0 } }, //v 
		{ 10, {    0,   0,   0,   0,   0,3276,3276,3276,3276,3276,1228,1020,   0,   0 } }, //w 
		{  6, {    0,   0,   0,   0,   0, 204, 204, 204, 120, 204, 204, 204,   0,   0 } }, //x 
		{  6, {    0,   0,   0,   0,   0, 204, 204, 204, 204, 204, 204, 248, 192, 124 } }, //y 
		{  6, {    0,   0,   0,   0,   0, 252, 192,  96,  48,  24,  12, 252,   0,   0 } }, //z 
		{  4, {    0,   0,  48,  24,  24,  24,  24,  12,  24,  24,  24,  24,  48,   0 } }, //{ 
		{  2, {    0,   0,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12 } }, //| 
		{  4, {    0,   0,  12,  24,  24,  24,  24,  48,  24,  24,  24,  24,  12,   0 } }, //} 
	};

	static const Glyph& placeholder = glyphs['?' - firstCodepoint];

	static inline const Glyph& GetGlyph(char codePoint)
	{
		if (codePoint < firstCodepoint || codePoint > lastCodepoint)
			return placeholder;
		else
			return glyphs[codePoint - firstCodepoint];
	}
}