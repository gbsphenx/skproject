
#pragma once

struct CDDATUndoBase {
	// 
	virtual void Undo_Begin() = NULL;
	// 
	virtual bool Undo_End(bool fCommit) = NULL;
	// 
	virtual bool Undo_UndoOnce() = NULL;
	// 
	virtual void Undo_Clear() = NULL;
	// 
	virtual bool Undo_IsUndoable() = NULL;
	// 
	virtual void Undo_OnUndoStacked() = NULL;

};

class CDDATUndoListItem
{
public:
	// 
	WPARAM wp;
	// 
	LPARAM lp;
	// 
	CMemFile f;
};

class CDDATUndoListElem
{
public:
	// 
	CDDATUndoListItem *pItem;
	// 
	int iLevel;
	// 
	bool fMark;

	// 
	CDDATUndoListElem()
	{
		iLevel = -1;
		fMark = false;
	}
};

class CDDATUndoStableList
{
public:
	// 
	CList<CDDATUndoListItem *, CDDATUndoListItem *> listItem;

};

class CDDATUndoList : public CDDATUndoBase
{
	// 
	int iLevel;
	// 
	CList<CDDATUndoListElem, CDDATUndoListElem> listElem;
	// 
	CList<CDDATUndoStableList *, CDDATUndoStableList *> listStable;

	// 
	void RemoveElem(CDDATUndoListElem s)
	{
		delete s.pItem;
	}
	// 
	void RemoveStable(CDDATUndoStableList *p)
	{
		POSITION pos = p->listItem.GetHeadPosition();
		while (pos) {
			delete p->listItem.GetNext(pos);
		}
	}

public:
	// 
	CDDATUndoList()
	{
		iLevel = 0;
	}
	// 
	~CDDATUndoList()
	{
		ClearStack();
	}
	// 
	void AddStack()
	{
		iLevel++;
	}
	// 
	bool RemoveStack(bool fCommit)
	{
		if (iLevel == 0) return false;

		if (fCommit) {
			POSITION pos = listElem.GetTailPosition();
			while (pos) {
				CDDATUndoListElem &r = listElem.GetPrev(pos);
				if (r.iLevel < iLevel) break;
				r.fMark = true;
			}
		} else {
			while (!listElem.IsEmpty()) {
				CDDATUndoListElem &r = listElem.GetTail();
				if (r.iLevel < iLevel) break;
				Undo_OnUndo(r.pItem);
				RemoveElem(listElem.RemoveTail());
			}
		}

		iLevel--;

		if (iLevel == 0) {
			CDDATUndoStableList *p = new CDDATUndoStableList();
			POSITION pos = listElem.GetTailPosition();
			while (pos) {
				p->listItem.AddTail(listElem.GetPrev(pos).pItem);
			}
			listStable.AddTail(p);

			listElem.RemoveAll();

			Undo_OnUndoStacked();
		}
		return true;
	}
	// 
	void AddUndoItem(CDDATUndoListItem *pItem)
	{
		CDDATUndoListElem x;
		x.pItem = pItem;
		x.iLevel = iLevel;
		listElem.AddTail(x);
	}
	// 
	void ClearStack()
	{
		POSITION pos;

		pos = listElem.GetHeadPosition();
		while (pos) {
			RemoveElem(listElem.GetNext(pos));
		}
		listElem.RemoveAll();

		pos = listStable.GetHeadPosition();
		while (pos) {
			CDDATUndoStableList *p = listStable.GetNext(pos);
			RemoveStable(p);
			delete p;
		}
		listStable.RemoveAll();
	}
	// 
	bool UndoStack()
	{
		if (!listElem.IsEmpty()) return false;
		if (listStable.IsEmpty()) return false;

		CDDATUndoStableList *p = listStable.RemoveTail();

		POSITION pos = p->listItem.GetHeadPosition();
		while (pos) {
			Undo_OnUndo(p->listItem.GetNext(pos));
		}

		RemoveStable(p);
		delete p;

		return true;
	}
	// 
	virtual void Undo_OnUndo(CDDATUndoListItem *pItem) = NULL;

	// 
	virtual void Undo_Begin()
	{
		AddStack();
	}
	// 
	virtual bool Undo_End(bool fCommit)
	{
		return RemoveStack(fCommit);
	}
	// 
	virtual bool Undo_UndoOnce()
	{
		return UndoStack();
	}
	// 
	virtual void Undo_Clear()
	{
		ClearStack();
	}
	// 
	virtual bool Undo_IsUndoable()
	{
		return !listStable.IsEmpty();
	}
};

struct CDDATUndoLock
{
	// 
	CDDATUndoBase &m_rUndo;
	// 
	BOOL m_bLocked;

public:
	// 
	CDDATUndoLock(CDDATUndoBase &r, BOOL bLock): m_rUndo(r), m_bLocked(bLock)
	{
		if (m_bLocked) m_rUndo.Undo_Begin();
	}
	// 
	~CDDATUndoLock()
	{
		if (m_bLocked) m_rUndo.Undo_End(false);
	}
	// 
	void Lock()
	{
		_ASSERTE(!m_bLocked);
		m_rUndo.Undo_Begin();
		m_bLocked = TRUE;
	}
	// 
	void Unlock(bool bCommit)
	{
		_ASSERTE(m_bLocked);
		m_rUndo.Undo_End(bCommit);
		m_bLocked = FALSE;
	}

};
