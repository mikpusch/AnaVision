// Tree.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "Tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Tree

IMPLEMENT_DYNCREATE(Tree, CView)

Tree::Tree()
{
	firsttime = true;
	pm_treeView = NULL;
	pm_treeImageList = NULL;
	frame = (CMainFrame *)(AfxGetMainWnd( ));
	FirstFileField = LastFileField = CurrentFileField = NULL;
	treeid = 0;

}

Tree::~Tree()
{
	DeleteTreeView();
}
void Tree::DeleteTreeView(){
	if (pm_treeImageList) delete pm_treeImageList;
	if (pm_treeView) delete pm_treeView;
}
void Tree::CreateTreeView()

{
	pDoc = (CAnaVisionDoc *) GetDocument();
	if (!pDoc->dat.images){
		return;
	}

	pm_treeView = new 	CTreeCtrl;
	pm_treeImageList = new CImageList;

	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;



	// Create the Image List.
	m_treeImageList.Create(13, 13, false, 3, 0);
	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_ICON3));
	m_treeImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_ICON4));
	m_treeImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_ICON5));
	m_treeImageList.Add(hIcon);

//	CRect rect = CRect(10,1,150,340);
//	CRect rect = CRect(10,1,160,340);
//	CRect rect = CRect(10,1,250,340);
	CRect rect = CRect(10,100,250,540);


	// Create the Tree View control.
	m_treeView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER |
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
//		TVS_EDITLABELS, CRect(20, 260, 160, 360), this, 110);
		TVS_EDITLABELS, rect, this, 110);

//	m_treeView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER |
//		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
//		TVS_EDITLABELS, CRect(10, 1, 150, 340), this, 110);
	m_treeView.SetImageList(&m_treeImageList, TVSIL_NORMAL);


	CString rootname;
	if (true){
		rootname = pDoc->dat.FirstFileName;
	}
	if (rootname.GetLength()<1){
		rootname="root";
	}
	MakeRoot(rootname);

	DataFile & d = pDoc->dat;

	HTREEITEM item = NULL;

	for (UINT i=0; i<d.NImages; i++){
		item = AddSequence(d.images[i].Title, item, i, d.images[i].treeid);
		if (!item) return;
	}


}

void Tree::MakeRoot(CString itemtext){
	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;

	CMainFrame* f = (CMainFrame *)(AfxGetMainWnd( ));
	TV_ITEM tvItem1, tvItem2;
	TV_INSERTSTRUCT tvInsert1;
	

	tvItem1.mask =
//		TVIF_STATE | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvItem2.mask = tvItem1.mask;
//		TVIF_STATE | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	tvItem1.state = 0;
//	tvItem1.stateMask = 1 | 2 |4 |8 |16 |32 |64 |128;
	tvItem1.stateMask = 8;
	tvItem1.cchTextMax = 4;
	tvItem1.cchTextMax = 20;
	tvItem1.iImage = 0;
	tvItem1.iSelectedImage = 0;
//	tvItem1.lParam=ROOT_TREE_ID;
	tvItem1.lParam=treeid;
//	f->pGroup->TreeID = treeid;
//	f->pGroup->AddedInTree = TRUE;
	treeid++;

/*
	PF.RootText.MakeText(tvItem1.lParam, itemtext);
*/
//	itemtext = "Root";
	{
		const char * p= LPCTSTR (itemtext );
		tvItem1.pszText = (char * )( p );
	}

	tvInsert1.hParent = TVI_ROOT;
	tvInsert1.hInsertAfter = TVI_FIRST;
	tvInsert1.item = tvItem1;
	HTREEITEM hRoot = m_treeView.InsertItem(&tvInsert1);
}


HTREEITEM Tree::AddSequence(CString itemtext, HTREEITEM Item, int index, int & settreeid){

	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;


	TV_ITEM tvItem1;
	TV_INSERTSTRUCT tvInsert1;
//	CString itemtext;

	
	HTREEITEM root = m_treeView.GetRootItem();

	tvItem1.mask =
//		TVIF_STATE | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	tvItem1.cchTextMax = 20;
	tvItem1.iImage = 1;
	tvItem1.iSelectedImage = 1;
	tvInsert1.hParent = root;
//	tvItem1.lParam=-is;
	tvItem1.lParam=treeid;
	settreeid = treeid;
//	series->AddedInTree = TRUE;
//	series->TreeID = treeid;
	treeid++;
//		PF.series[i].SeriesText.MakeText(tvItem1.lParam, itemtext);
	char ss[20];
	_itoa(index+1, ss, 10);
//	itemtext = CString("GeSeries ")+CString(ss);
//	itemtext = CString("")+CString(ss)+CString(" - ")+series->Stim->EntryName;
	CString itext = CString("")+CString(ss)+CString(" - ")+itemtext;
	{
		const char * p= LPCTSTR (itext );
		tvItem1.pszText = (char * )( p );
	}

	tvItem1.state = 0;

	if (!Item){//indedx==0){
	   tvInsert1.hInsertAfter = TVI_FIRST;
		m_treeView.Expand( root, TVE_EXPAND);
	}
	else {
	   tvInsert1.hInsertAfter = Item;
	}

	tvInsert1.item = tvItem1;
	HTREEITEM toreturn = m_treeView.InsertItem(&tvInsert1);

	if (!Item){//indedx==0){
		m_treeView.Expand( root, TVE_EXPAND);
	}
	return toreturn;

}


BEGIN_MESSAGE_MAP(Tree, CView)
	//{{AFX_MSG_MAP(Tree)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS( ID_FILENO_FIELD, OnCurrentFileNoChanged)
	ON_EN_KILLFOCUS( ID_DELTAIMAGE_FIELD, OnDeltaImageChanged)

	ON_BN_CLICKED( ID_PREVIOUSFILE_BUTTON, PreviousFileClicked)
	ON_BN_CLICKED( ID_NEXTFILE_BUTTON, NextFileClicked)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Tree drawing

void Tree::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if (firsttime){
		firsttime = false;
		pDoc = (CAnaVisionDoc *) GetDocument();
		CreateTreeView();
//		Beep(100,10);
		CreateButtons();
//		if (pDoc->dat){

	}
	if (pDoc->IsLoadingFile){
		return;
	}
	MoveButtons(pDC);

	pDC->TextOut(1,200, pDoc->GetPathName());
}


void Tree::CreateFileNumberButtons(){
	CRect r;
	if (FirstFileField) delete FirstFileField;
	if (LastFileField) delete LastFileField;
	if (CurrentFileField) delete CurrentFileField;

	DataFile & d = pDoc->dat;
	int first=0;
	int last=0;
	int current=0;
	if (d.NImages>0){
		ImageSequence & IS = d.images[d.ImageIndex];
		first = IS.FirstIndex;
		last = IS.LastIndex;
		current = IS.CurrentIndex;
	}
	FirstFileField = new Text(ID_FIRSTFILE_FIELD, first, 0, first, false, r, this);
	LastFileField = new Text(ID_LASTFILE_FIELD, last, 0, last, false, r, this);
	CurrentFileField = new Text(ID_FILENO_FIELD, current, first, last, true, r, this);
	
}

void Tree::CreateButtons(){
	pDoc = (CAnaVisionDoc *) GetDocument();
	if (!pDoc->dat.images){
		//return;
	}

	CRect r;
	CreateFileNumberButtons();

	
/*
//	FirstFileField = new Text(ID_FIRSTFILE_FIELD, pDoc->dat.FirstIndex, 0, pDoc->dat.FirstIndex, FALSE, r, this);
	FirstFileField = new Text(ID_FIRSTFILE_FIELD, IS.FirstIndex, 0, IS.FirstIndex, FALSE, r, this);
//	LastFileField = new Text(ID_LASTFILE_FIELD, pDoc->dat.LastIndex, 0, pDoc->dat.LastIndex, FALSE, r, this);
	LastFileField = new Text(ID_LASTFILE_FIELD, IS.LastIndex, 0, IS.LastIndex, FALSE, r, this);
	CurrentFileField = new Text(ID_FILENO_FIELD, IS.CurrentIndex,
						IS.FirstIndex, IS.LastIndex, TRUE, r, this);
*/

	NextFileButton = new Button(ID_NEXTFILE_BUTTON, false, CString("->"), r, this);
	PreviousFileButton = new Button(ID_PREVIOUSFILE_BUTTON, false, CString("<-"), r, this);

	TextDeltaImageField = new Text(ID_TEXT_DELTAIMAGE_FIELD, "Step", false, r, this);
//	TextFiltBox = new Text(ID_TEXT_FILTER_BOX, "Filt.", FALSE, r, this);

//	DeltaImageField = new Text(ID_DELTAIMAGE_FIELD, frame->p.DeltaImage, 1, 30000, TRUE, r, this);
	DeltaImageField = new Text(ID_DELTAIMAGE_FIELD, pDoc->dat.p.DeltaImage, 1, 30000, true, r, this);


}
void Tree::MoveButtons(CDC * pDC){

	CRect r;
	GetClientRect(&r);

	DeltaImageField->SetInt(pDoc->dat.p.DeltaImage);

	r.left = 2;

	r.right = r.left + 60;

	r.top = 10;
	r.bottom = r.top + 20;

	FirstFileField->MoveWindow(&r);

	r.left = r.right + 1;
	r.right = r.left + 60;
	LastFileField->MoveWindow(&r);

	r.top = r.bottom + 5;
	r.bottom = r.top + 20;

	r.left = 2;
	r.right = r.left + 20;

	PreviousFileButton->MoveWindow(&r);

	r.left = r.right;
	r.right = r.left + 60;
	CurrentFileField->MoveWindow(&r);
	r.left = r.right;
	r.right = r.left + 20;
	NextFileButton->MoveWindow(&r);


	r.top = r.bottom + 20;
	r.bottom = r.top + 20;

	r.left = 2;
	r.right = r.left + 50;

	TextDeltaImageField->MoveWindow(&r);
	r.left = r.right;
	r.right = r.left + 60;
	DeltaImageField->MoveWindow(&r);
}

/////////////////////////////////////////////////////////////////////////////
// Tree diagnostics

#ifdef _DEBUG
void Tree::AssertValid() const
{
	CView::AssertValid();
}

void Tree::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Tree message handlers


void Tree::OnCurrentFileNoChanged(){
	DoSetFileNoField(CurrentFileField->GetInt());
}
void Tree::PreviousFileClicked(){
	DoSetFileNoField(CurrentFileField->GetInt()-pDoc->dat.p.DeltaImage);
}
void Tree::NextFileClicked(){
	DoSetFileNoField(CurrentFileField->GetInt()+pDoc->dat.p.DeltaImage);
}

void Tree::DoSetFileNoField(int index){
	CWaitCursor dummy;

	DataFile & d = pDoc->dat;
	if (d.NImages<1) return;
	ImageSequence & IS = d.images[d.ImageIndex];
//	ShowFloat(index, "index before");
//	ShowFloat(IS.LastIndex, "Last");

	if (index<IS.FirstIndex){
		index = IS.FirstIndex;
	}
	if (index>IS.LastIndex){
		index = IS.LastIndex;
	}

//	ShowFloat(index, "index after");
	CurrentFileField->SetInt(index);

	if (IS.CurrentIndex == index){
		return;
	}

	if (!pDoc->ReadFile(index)){
//		Alert("error");
		index = IS.CurrentIndex;

		pDoc->ReadFile(index);
		CurrentFileField->SetInt(index);
		return;
	}
	CChildFrame * child = (CChildFrame *)(GetParent()->GetParent());
	child->OnAnalyzeThisimage();

	IS.CurrentIndex = index;
//	ShowFloat(index, "index");


	pDoc->UpdateAllViews(NULL);


}

void Tree::OnDeltaImageChanged(){
	pDoc->dat.p.DeltaImage = DeltaImageField->GetInt();
//	Invalidate();

}

void Tree::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
//	frame->p = pDoc->dat.p;
//	Beep(2000,10);
	
}

HTREEITEM Tree::GetSelected(BOOL & RootSelected, BOOL & SeriesSelected, BOOL & SweepSelected,
							int & indexseries, int & indexsweep){
	CMainFrame* f = (CMainFrame *)(AfxGetMainWnd( ));
	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;

	RootSelected = SeriesSelected = SweepSelected = false;
	indexseries = 0;

	HTREEITEM SelectedItem = m_treeView.GetSelectedItem();
	if (!SelectedItem) return NULL;
	if (SelectedItem == m_treeView.GetRootItem( )){
		RootSelected = true;
		return SelectedItem;
	}
	int treeid = m_treeView.GetItemData(SelectedItem);


//	if (m_treeView.ItemHasChildren(SelectedItem)){ // i.e. is series
		SeriesSelected = true;

		while (indexseries<int(pDoc->dat.NImages)){
			ImageSequence & d = pDoc->dat.images[indexseries];
			if (d.treeid==treeid){
//				Alert0("found");
				return SelectedItem;
			}
			indexseries++;
		}
		Alert0("error in get item: series not found");
		return NULL;
//	}
	// so must be a sweep;
/*
	SweepSelected = TRUE;
	GeSeries * s = f->pGroup->series;
	indexseries = 0;
	while (s){
		Sweep * sweep = s->sweeps;
		indexsweep = 0;
		while(sweep){
			if (sweep->TreeID==treeid){
//				Alert0("found");
				return SelectedItem;
			}
			sweep = (Sweep *)sweep->Next;
			indexsweep++;
		}
		s = (GeSeries*)s->Next;
		indexseries++;
	}
	Alert0("error in get item: sweep not found");
	return NULL;
*/
}

void Tree::UpdateActiveSequence(){
	BOOL RootSelected, SeriesSelected, SweepSelected;
	int indexseries,  indexsweep;
	HTREEITEM SelectedItem = GetSelected(RootSelected, SeriesSelected,
								SweepSelected, indexseries, indexsweep);
	if (!SelectedItem) return;
	if (RootSelected) return;
	if (SweepSelected) return;
	if (indexseries==pDoc->dat.ImageIndex){
		return;
	}
	pDoc->dat.ImageIndex = indexseries;
	CreateFileNumberButtons();
}

void Tree::MyInvalidate(){
//	frame->p = pDoc->dat.p;
	Invalidate();
}

BOOL Tree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;

	NMHDR * phdr = (NMHDR * ) lParam;
	if (CWnd::FromHandle(phdr->hwndFrom)==&(m_treeView)){
		UpdateActiveSequence();
/*
		if ( (phdr->code == NM_CLICK ) ||   
			 (phdr->code == NM_DBLCLK ) ||
			 (phdr->code == NM_RETURN ) ){
			Beep(1000,10);
		}
*/
	}
	return CView::OnNotify(wParam, lParam, pResult);
}


void Tree::OnModifydataSubtract(){
	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;

	BOOL RootSelected, SeriesSelected, SweepSelected;
	int indexseries,  indexsweep;
	HTREEITEM SelectedItem = GetSelected(RootSelected, SeriesSelected,
								SweepSelected, indexseries, indexsweep);
	if (!SelectedItem) return;
	if (RootSelected) return;
	if (SweepSelected) return;
//	ShowFloat(indexseries, "index");
	CWaitCursor dummy;
	DataFile & d = pDoc->dat; 
	UINT oldn=d.NImages;
	if (!d.CopyImage(indexseries)){
		return;
	}
	if (d.NImages>oldn){
		//Beep(1000,100);
		HTREEITEM item = m_treeView.GetChildItem(m_treeView.GetRootItem());
		if (item==NULL) return;
		while (m_treeView.GetNextSiblingItem(item)){
			item = m_treeView.GetNextSiblingItem(item);
		}
		item = AddSequence(d.images[oldn].Title, item, oldn, d.images[oldn].treeid);
		if (!item) return;
		pDoc->SetModifiedFlag();
		Invalidate();
	}
}

void Tree::OnModifydataRatio(){
	CTreeCtrl & m_treeView = *pm_treeView;
	CImageList & m_treeImageList = *pm_treeImageList;

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	int indexa, indexb, indexc;
	CString newtitle;

	if (!pDoc->id.data){
		return;
	}

	BOOL RootSelected, SeriesSelected, SweepSelected;
	int indexseries,  indexsweep;
	HTREEITEM SelectedItem = GetSelected(RootSelected, SeriesSelected,
								SweepSelected, indexseries, indexsweep);
	if (!SelectedItem) return;
	if (RootSelected) return;
	if (SweepSelected) return;

	DataFile & d = pDoc->dat; 

	RatioDiag diag;

	diag.NStringsAB = d.NImages;

	CString * StringsAB = new CString[diag.NStringsAB];
	diag.StringsAB = StringsAB;

	for (int i=0; i<diag.NStringsAB; i++){
		StringsAB[i] = d.images[i].Title;
	}


	diag.indexa = indexseries;

	diag.indexb = indexseries;
	diag.indexc = indexseries+1;


	if (diag.indexc>=diag.NStringsAB){
		diag.indexc = -1;
	}


	diag.m_BackgroundA = d.p.BackgroundA;
	diag.m_BackgroundB = d.p.BackgroundB;

	diag.m_NSurroundForAverage = d.p.NSurroundForAverage;

	UINT oldn=d.NImages;

	if (diag.DoModal()!=IDOK){
		goto EndModifyRatio;
	}
	newtitle = diag.NewResultString;

//	Alert(newtitle);

	if (diag.InvalidSelection){
		goto EndModifyRatio;
	}

	indexa = diag.indexa;
	indexb = diag.indexb;
	indexc = diag.indexc;
	if (indexa == indexb){
		Alert("A = B");
		goto EndModifyRatio;
	}
	if (indexa == indexc){
		Alert("A = C");
		goto EndModifyRatio;
	}
	if (indexb == indexc){
		Alert("C = B");
		goto EndModifyRatio;
	}

	if (d.images[indexa].Sizex != d.images[indexb].Sizex){
		Alert("images have different xsize");
		goto EndModifyRatio;
	}
	if (d.images[indexa].Sizey != d.images[indexb].Sizey){
		Alert("images have different ysize");
		goto EndModifyRatio;
	}
	
	//////  CHECK COMPATIBLE IMAGE FORMATS AND NUMBERS


	if (diag.indexc>=0){
		CString c="Are you sure to overwrite the data \"";
		c += StringsAB[diag.indexc];
		c+= "\" ?";
		if (!Confirm(c)){
			goto EndModifyRatio;
		}
		CString cc = "Last chance: " +c;
		if (!Confirm(cc)){
			goto EndModifyRatio;
		}


	}

	d.p.BackgroundA = diag.m_BackgroundA;
	d.p.BackgroundB = diag.m_BackgroundB;
	d.p.NSurroundForAverage = diag.m_NSurroundForAverage;




	d.DoAverage(indexa, indexb, indexc, newtitle);

EndModifyRatio:

	delete [] StringsAB;


	if (d.NImages>oldn){
		//Beep(1000,100);
		HTREEITEM item = m_treeView.GetChildItem(m_treeView.GetRootItem());
		if (item==NULL) return;
		while (m_treeView.GetNextSiblingItem(item)){
			item = m_treeView.GetNextSiblingItem(item);
		}
		item = AddSequence(d.images[oldn].Title, item, oldn, d.images[oldn].treeid);
		if (!item) return;
		pDoc->SetModifiedFlag();
		Invalidate();
	}

}

