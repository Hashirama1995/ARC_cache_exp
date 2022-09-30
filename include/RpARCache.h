#ifndef __ARC_CACHE_H_INCLUDE
#define __ARC_CACHE_H_INCLUDE

#define MaxCapacity 1024

class RpList;
class RpMap;
struct Page;

class RpARCache {

private:
	RpList *t1, *t2, *b1, *b2;
	RpMap *ghosts, *pages;
	double p;
	int capacity;
	bool initialized;
	static RpARCache * mRpARCache;

public:
	unsigned long requests;
	unsigned long hits;

private:
	RpARCache();

public:
	static RpARCache* GetARCCacheInstance();
	int Initialize(int capacity = MaxCapacity);
	int DeInitilize();
	void CacheStats();
	int RequestInCache(int);
	int CheckInCache(int);
	void UpdateLRUPageListAndMap(int, Page *);
	virtual ~RpARCache();

};

#endif // __ARC_CACHE_H_INCLUDE
