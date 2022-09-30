#include <iostream>
#include "../include/RpARCache.h"
#include "../include/RpMap.h"
#include "../include/RpList.h"

using namespace std;

//Since it is a static member variable so defination required
//Its also don't involve in the size of class object
RpARCache* RpARCache::mRpARCache = NULL;

#define min(a,b) ((a < b)? a : b)
#define max(a,b) ((a > b)? a : b)

#define sigma1(b1,b2) ((b1->sizeOfList >= b2->sizeOfList)? 1 : b2->sizeOfList/(double)b1->sizeOfList)
#define sigma2(b1,b2) ((b2->sizeOfList >= b1->sizeOfList)? 1 : b1->sizeOfList/(double)b2->sizeOfList)

/*For Knowledge: Why/When we use initializer list?
*	1) For initialization of non-static const data members:
*	2) For initialization of reference members:
*	3) For initialization of member objects which do not have default constructor:
*	4) For initialization of base class members : 
*	5) When constructor’s parameter name is same as data member
*	6) For Performance reasons:
*/
RpARCache::RpARCache():
t1(NULL),
t2(NULL),
b1(NULL),
b2(NULL),
ghosts(NULL),
pages(NULL),
capacity(0),
initialized(false),
requests(0),
hits(0)
{

}

RpARCache::~RpARCache(){

}

RpARCache* RpARCache::GetARCCacheInstance(){

	if(NULL == mRpARCache){
		mRpARCache = new RpARCache();
		if(NULL == mRpARCache){
			cout<<"RpARCache::GetARCCahceInstance; RpARCache is null, object not created"<<endl;
		}
	}
	return mRpARCache;
}

int RpARCache::Initialize(int capacity_){

	if(initialized){
		cout<<"RpARCache::Initializ; Cache is already initialized"<<endl;
		return 0;
	}
	t1 = new RpList();
	if(NULL == t1) {
		cout<<"RpARCache::Initialize; t1 List could not created"<<endl;
		DeInitilize();
		return -1;
	}
	t2 = new RpList();
  if(NULL == t2) {
    cout<<"RpARCache::Initialize; t2 List could not created"<<endl;
		DeInitilize();
    return -1; 
  }
  b1 = new RpList();
  if(NULL == b1) {
    cout<<"RpARCache::Initialize; b1 List could not created"<<endl;
		DeInitilize();
    return -1; 
  }
  b2 = new RpList();
  if(NULL == t1) {
    cout<<"RpARCache::Initialize; b2 List could not created"<<endl;
		DeInitilize();
    return -1; 
  }
	pages = new RpMap();
	if(NULL == pages) {
    cout<<"RpARCache::Initialize; pages map could not created"<<endl;
		DeInitilize();
    return -1;
  }
  if(-1 == pages->Initialize(capacity_)) {
    cout<<"RpARCache::Initialize; pages map could not initialised successfully"<<endl;
		DeInitilize();
    return -1; 
  }
  ghosts = new RpMap();
  if(NULL == ghosts) {
    cout<<"RpARCache::Initialize; ghosts map could not created"<<endl;
		DeInitilize();
    return -1;
  }
  if(-1 == ghosts->Initialize(capacity_)) {
    cout<<"RpARCache::Initialize; ghosts` map could not initialised successfully"<<endl;
		DeInitilize();
    return -1;
  }
	capacity = capacity_;
	initialized = true;
	
	return 0;
}

int RpARCache::DeInitilize(){
	// delete all, deleting NULL pointer is no-prob, it doesn't causes error
	delete t1;
	delete t2;
	delete b1;
	delete b2;
	delete pages;
	delete ghosts;
	initialized = false;
	return 0;
}

void RpARCache::CacheStats(){
	cout<<\
    " Requests: "<<requests<<\
    " Hits: "<<hits<<\
    " Hit Ratio: "<<(hits*100/(float)requests)<<endl;
}

int RpARCache::RequestInCache(int address) {
	requests++;

	if(CheckInCache(address)) {
		hits++;
	}
	return 0;
}

int RpARCache::CheckInCache(int address){
	// Case I: When requested pages is already in cache (also called hit), means either it will be in t1 or t2.
	// So we need to do following action:
	// 1)	move page to top of t2

	// Случай I: когда запрошенные страницы уже находятся в кеше (также называемом попаданием), это означает, что они будут либо в t1, либо в t2.
	// Итак, нам нужно сделать следующее действие:
	// 1) переместить страницу в начало t2

	if(pages->HasMap(address)){
		Page *p = pages->MapGet(address);
	 	p->listOfPage->ListRemove(p);
	 	t2->ListPushFront(p);
		return 1;		
	}
	
	// Case II-III: When request page is not in cache but it is in ghost map, means either in b1 or b2.
	// So will have to do following activities:
	// 1)	move page to top of t2
	// 2)	place page in cache	
	// 3) update p

	// Случай II-III: когда страница запроса находится не в кеше, а в карте-призраке, значит либо в b1, либо в b2.
// Поэтому придется выполнить следующие действия:
// 1) переместить страницу в начало t2
// 2) поместить страницу в кеш
// 3) обновить p

	if(ghosts->HasMap(address))	{
		Page *pg = ghosts->MapGet(address);
		//if page is in b1
		if(pg->listOfPage == b1) {
			p = min((p + sigma1(b1, b2)), capacity);
		} else { // if page is in b2
			p = max((p - sigma2(b1, b2)), 0);
		}

		pg->listOfPage->ListRemove(pg);//remove the page from list where it is currently either in b1 or b2
	 	t2->ListPushFront(pg); // coping the page to top of t2
		ghosts->MapUnset(address); // removing the page from ghosts map as it should be in cache now means pages map 
		pages->MapSet(address, pg); // put the page in pages map means cache
		UpdateLRUPageListAndMap(address, pg);
		return 0;
	}

	// Case IV: When requested page is not in cache and not in ghost page
	// Случай IV: когда запрошенная страница не в кеше и не на странице-призраке
	if(t1->sizeOfList + b1->sizeOfList == capacity) {
		if(t1->sizeOfList < capacity) {
			Page *pg = b1->ListPopBack();
			if(NULL != pg) {
				ghosts->MapUnset(pg->addrOfPage);
				delete pg;
			}
			Page *ghostPage = ghosts->MapGet(address);
			UpdateLRUPageListAndMap(address, ghostPage);
		} else {
			Page *pg = t1->ListPopBack();
			if(NULL != pg){
				pages->MapUnset(pg->addrOfPage);
				delete pg;
			}
		}
	} else {
		int len = t1->sizeOfList + t2->sizeOfList + b1->sizeOfList + b2->sizeOfList;

		if(len >= capacity) {
			if(len == 2*capacity) {
				Page *pg = b2->ListPopBack();
				if(NULL != pg) {
					ghosts->MapUnset(pg->addrOfPage);
					delete pg;
				}
			}
			Page *ghostPage = ghosts->MapGet(address);
			UpdateLRUPageListAndMap(address, ghostPage);
		}
	}

	Page *pg = new Page(address);
	t1->ListPushFront(pg);
	pages->MapSet(address, pg); 
	return 0;
}

void RpARCache::UpdateLRUPageListAndMap(int address, Page *ghostPage) {

  if (t1->sizeOfList != 0 &&             										// |t1| is not empty
       (t1->sizeOfList > p ||                 							// |t1| exceeds target p
         (ghosts != NULL && ghostPage->listOfPage == b2 && 	// x_t is in b2
            t1->sizeOfList == p))) {          										// |t1| = p
    // delete LRU in t1
		Page *pg = t1->ListPopBack();
    if (pg != NULL) {
      // remove LRU from cache
			pages->MapUnset(address);
      
			// move lru into mru of b1
			b1->ListPushFront(pg);
			ghosts->MapSet(address, pg);
    }
  } else {
    // delete LRU in t2
		Page *pg = t2->ListPopBack();
    if (pg != NULL) {
      // remove LRU from cache
			pages->MapUnset(address);

      // move lru into mru of b2
			b2->ListPushFront(pg);
			ghosts->MapSet(address, pg);
    }
  }
}

