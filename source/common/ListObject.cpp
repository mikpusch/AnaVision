#include "stdafx.h"
#include "../common/Listobject.h"

ListObject::ListObject(){

	Next = Previous = NULL;
}

ListObject::~ListObject(){
}

int ListObject::LengthList(ListObject * s){
	int n = 0;
	while (s){
		n++;
		s = s->Next;
	}
	return n;
}


ListObject * ListObject::Last(ListObject * s){
	if (!s) return NULL;
	while (s->Next){
		s= s->Next;
	}
	return s;
}


ListObject * ListObject::First(ListObject * s){
	if (!s) return NULL;
	while (s->Previous){
		s= s->Previous;
	}
	return s;

}

ListObject * ListObject::AddListObjectAtEnd(ListObject * list, ListObject * ToAdd){
	// this procedure connects only. It does not copy the contents of "ToAdd"
	if (!ToAdd) return list;
	if (!list) return ToAdd;
	ListObject * l=Last(list);
	ToAdd->Previous=l;
	l->Next=ToAdd;
	return list;
}



int ListObject::GetIndex(ListObject * list, ListObject * entry){
				//returns -1 if the entry is not found  or if either input is NULL

	if (!list) return -1;
	if (!entry) return -1;
	int result=0;
	while (list){
		if (list==entry){
			return result;
		}
		result ++;
		list = list->Next;
	}
	return -1;
}


ListObject * ListObject::nth(ListObject * list, int index){
	if (index<0) return NULL;
	int i=0;
	while (list){
		if (i==index) return list;
		i++;
		list = list->Next;
	}
	return NULL;
}
		


ListObject *  ListObject::InsertInMiddleBefore(ListObject * list, ListObject * ToInsert, int index){

	if (index<0){
		return list;
	}

	if (!ToInsert) return list;
	if (!list) return ToInsert;
	
	if (index>=(list->LengthList(list))){
		return AddListObjectAtEnd(list, ToInsert);
	}

	if (index==0){
		return AddListObjectAtEnd(ToInsert, list);
	}

	ListObject * LastPart = nth(list, index);

	ListObject * LastOfFirstPart = nth(list, index-1);

	LastOfFirstPart->Next = ToInsert;
	ToInsert->Previous = LastOfFirstPart;


	ListObject * LastOfInsert = Last(ToInsert);

	LastOfInsert->Next = LastPart;
	LastPart->Previous = LastOfInsert;

	return list;
}

ListObject * ListObject::MoveEntry(ListObject * list, int fromindex, int toindex){
	if (fromindex<0) return list;
	if (toindex<0) return list;
	if (fromindex==toindex) return list;
	int length = LengthList(list);
	if (fromindex>=length) return list;
	if (toindex>=length) return list;

	ListObject * from = nth(list, fromindex);
	ListObject * to = nth(list, toindex);

	ListObject * toprevious = to->Previous;
	ListObject * tonext = to->Next;

	ListObject * fromprevious = from->Previous;
	ListObject * fromnext = from->Next;


	if (toindex<fromindex){

		to->Previous = from;
		from->Next = to;
		from->Previous = toprevious;
	
		if (toprevious){
			toprevious->Next = from;
		}
		if (fromnext){
			fromnext->Previous = fromprevious;
		}
		if (fromprevious){
			fromprevious->Next = fromnext;
		}
	}
	else{
		if (fromprevious){
			fromprevious->Next = fromnext;
		}
		if (fromnext){
			fromnext->Previous = fromprevious;
		}
		from->Previous = to;
		to->Next = from;
		from->Next = tonext;
		if (tonext){
			tonext->Previous = from;
		}
	}

	if (toindex==0){
		return from;
	}
	if (fromindex==0){
		return fromnext;
	}
	return list;
}


ListObject * ListObject::VirtualLast(ListObject * s){
	if (!s) return NULL;
	while (s->Next){
		s= s->Next;
	}
	return s;
}
