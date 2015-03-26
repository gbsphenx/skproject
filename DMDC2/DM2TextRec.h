
#ifndef __DM2TEXTREC_H__
#define __DM2TEXTREC_H__

enum {
	trptNo,
	trptOrnate,
	trptCreature,
	trptTextg, // text in GRAPHICS.dat
	trptTexti, // text internal (in DUNGEON.dat)
};

class DM2TextRec_t
{
public:
	// 
	bool complex;
	// 
	int trpt;
	// 
	CString text1;
	// 
	WORD index;

	// 
	void Decode(WORD w2);

};

#endif // __DM2TEXTREC_H__
