#if !defined(AFX_TREE_H__1598D8AF_E1EE_49F3_AFA6_5C6F58B6C52D__INCLUDED_)
#define AFX_TREE_H__1598D8AF_E1EE_49F3_AFA6_5C6F58B6C52D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Tree view

class CMainFrame;
class  CAnaVisionDoc;


class Tree : public CView
{
protected:
	Tree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(Tree)

// Attributes
public:

	CTreeCtrl * pm_treeView;
	CImageList * pm_treeImageList;

	void CreateTreeView();
	void DeleteTreeView();
	void MakeRoot(CString itemtext);
	int treeid;
	HTREEITEM AddSequence(CString itemtext, HTREEITEM Item, int index, int & settreeid);
	HTREEITEM GetSelected(BOOL & RootSelected, BOOL & SeriesSelected,
		BOOL & SweepSelected, int & indexseries, int & indexsweep);

	void UpdateActiveSequence();

	bool firsttime;

	CMainFrame * frame;
	CAnaVisionDoc* pDoc;

	Text * FirstFileField;
	Text * LastFileField;
	Text * CurrentFileField;
	Text * DeltaImageField;
	Text * TextDeltaImageField;

	Button * NextFileButton;
	Button * PreviousFileButton;


// Operations
public:
	void MyInvalidate();
	void CreateButtons();
	void MoveButtons(CDC * pDC);
	void DoSetFileNoField(int index);
	void CreateFileNumberButtons();

	void OnModifydataSubtract();

	void OnModifydataRatio();



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Tree)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~Tree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(Tree)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg	void OnCurrentFileNoChanged();
	afx_msg	void PreviousFileClicked();
	afx_msg	void NextFileClicked();
	afx_msg	void OnDeltaImageChanged();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREE_H__1598D8AF_E1EE_49F3_AFA6_5C6F58B6C52D__INCLUDED_)
