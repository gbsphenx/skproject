
#ifndef __DM2SCROLLREC_H__
#define __DM2SCROLLREC_H__

class DM2ScrollRec_t
{
public:
	// 
	bool fExt;
	// 
	WORD index;
	// 
	CString text1;

	// 
	void Decode(WORD w2);

};

#endif // __DM2SCROLLREC_H__
