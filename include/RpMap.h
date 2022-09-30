#ifndef __MAP_H_INCLUDE
#define __MAP_H_INCLUDE

class Page;

struct Bucket {
	int key;
	Page *page;
	Bucket *nextBucket;
};

class RpMap {

public:
	Bucket **pBucket;
	int sizeOfMap;
	int currSizeOfMap;

	RpMap();
	virtual ~RpMap();
	int Initialize(int);
	void MapSet(int, Page *);
	void FreeMap();
	void MapUnset(int);
	Page * MapGet(int);
	int HasMap(int);
	int HashKey(int, int);
};

#endif // __MAP_H_INCLUDE
