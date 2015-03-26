#pragma once


// CHintOfStatic

class CHintOfStatic : public CStatic
{
	// 
	char szVuff[3*300*100];

	// 
	DECLARE_DYNAMIC(CHintOfStatic)

public:
	// 
	int nCur, nMax;

	// 
	CHintOfStatic();
	// 
	virtual ~CHintOfStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void PreSubclassWindow();
};


