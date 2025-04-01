#pragma once

//#ifndef LIST_OBJECT_MIK
//#define LIST_OBJECT_MIK


class Rectangle{
};

class ListObject { 

public :
	ListObject();
	~ListObject();

	ListObject * Next;
	ListObject * Previous;

	static int LengthList(ListObject * s);
	static ListObject * First(ListObject * l);
	static ListObject * Last(ListObject * l);
	virtual ListObject * VirtualLast(ListObject * l);
	static ListObject * AddListObjectAtEnd(ListObject * list, ListObject * ToAdd);
	static ListObject * Remove(ListObject * list, int index);

	static	int GetIndex(ListObject * list, ListObject * entry); //
				//returns -1 if the entry is not found  or if either input is NULL

	static ListObject * nth(ListObject * list, int index);

	static ListObject *  InsertInMiddleBefore(ListObject * list, ListObject * ToInsert, int index);

	static ListObject *	MoveEntry(ListObject * list, int fromindex, int toindex);

};

template <class T>
class BinaryTreeObject{
public :
	BinaryTreeObject();
	~BinaryTreeObject();

	static BinaryTreeObject * AddLeft(BinaryTreeObject * t, BinaryTreeObject * left);
	static BinaryTreeObject * AddRight(BinaryTreeObject * t, BinaryTreeObject * right);

	
	T * object;
	T * Left;
	T * Right;

	T * Parent;

};

template <class T>
BinaryTreeObject<T>::BinaryTreeObject(){
	object=Left=Right=Parent=NULL;
}

template <class T>
BinaryTreeObject<T>::~BinaryTreeObject(){
	if (Left) delete Left;
	if (Right) delete Right;
	delete Object;
}

template <class T>
BinaryTreeObject<T> * BinaryTreeObject<T>::AddLeft(BinaryTreeObject * t, BinaryTreeObject * left){
	t->Left = left:
	if (left){
		left->Parent = t;
	}
	return t;
}

template <class T>
BinaryTreeObject<T> * BinaryTreeObject<T>::AddRight(BinaryTreeObject * t, BinaryTreeObject * right){
	t->Right = right;
	if (right){
		right->Parent = t;
	}
	return t;
}


//#endif