#ifndef __LIST_H_INCLUDE
#define __LIST_H_INCLUDE

//Forward declration also called generic declartion
class RpList;

struct Page {
  Page *nextPage;// Using struct keyword is not mandatory in c++
  Page *prevPage;
  RpList *listOfPage; 
  int addrOfPage;

	Page(int address) {
		nextPage = NULL;
		prevPage = NULL;
		listOfPage = NULL;
		addrOfPage = address;
	};
	~Page(){};
};

class RpList {

public:
	Page *headOfList;
	Page *tailOfList;
	int sizeOfList;

	RpList();
	virtual ~RpList();
	int FreeList();
	Page *ListRemove(Page *);
	Page *ListPopBack();
	void ListPushFront(Page *); 

};

#endif // __LIST_H_INCLUDE
