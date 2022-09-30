#include <iostream>
#include "../include/RpList.h"

RpList::RpList():
headOfList(NULL),
tailOfList(NULL),
sizeOfList(0)
{

}

RpList::~RpList(){

}

int RpList::FreeList(){
	Page *temp = headOfList, *next = NULL;
	while (NULL != temp) {
		next = temp->nextPage;
		delete temp;
		temp = next;
	}
}

Page * RpList::ListRemove(Page * page){
  // remove page from list
  if (sizeOfList == 1) {
    headOfList = NULL;
    tailOfList = NULL;
  } else {
    if (tailOfList == page) {
      tailOfList = page->prevPage;
      tailOfList->nextPage = NULL;
    } else if (headOfList == page) {
      headOfList = page->nextPage;
      headOfList->prevPage = NULL;
    } else {
      page->prevPage->nextPage = page->nextPage;
      page->nextPage->prevPage = page->prevPage;
    }
  }

  page->listOfPage = NULL;

  sizeOfList--;

  return page;
}

Page * RpList::ListPopBack(){
  if (0 == sizeOfList) {
    return NULL;
  } else {
    return ListRemove(tailOfList);
  }
}

void RpList::ListPushFront(Page *page){
  // insert at front
  if (0 == sizeOfList) {
    headOfList = page;
    tailOfList = page;
  } else {
    page->nextPage = headOfList;
    headOfList->prevPage = page;
    headOfList = page;
    page->prevPage = NULL;
  }

  page->listOfPage = this;

  sizeOfList++;
}
