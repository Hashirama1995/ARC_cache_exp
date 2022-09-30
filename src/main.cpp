#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include "../include/RpARCache.h"

using namespace std;

#define CACHE_SIZE 10240 
#define STARTING_PAGE_ADDRESS 1000
#define END_PAGE_ADDRESS 100000

int main(int argc, char *agrv[]){
	
	RpARCache *ARC = RpARCache::GetARCCacheInstance();
	if(NULL == ARC) {
		cout<<"Cache couldn't created"<<endl;
	}
	if(0 != ARC->Initialize(CACHE_SIZE)){
		cout<<"Cache couldn't Initialized"<<endl;
	}
	
	int pageAddress = 0, count = 0;
	while(count < END_PAGE_ADDRESS) {

		struct timeval time_;
		gettimeofday(&time_,NULL);
		pageAddress = time_.tv_usec/10;
		//cout<<"pageAddress = "<<(time_.tv_usec/10)<<endl;
		ARC->RequestInCache(pageAddress);
		if(count%100 == 0) {
			ARC->CacheStats();
		}
		//usleep(pageAddress/100);	
		count++;
	}	
	ARC->DeInitilize();
	return 0;
}
