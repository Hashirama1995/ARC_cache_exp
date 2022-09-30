#include <iostream>
#include "../include/RpMap.h"

using namespace std;

RpMap::RpMap():
pBucket(NULL),
sizeOfMap(0),
currSizeOfMap(0)
{

}

RpMap::~RpMap(){

}

int RpMap::HashKey(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

int RpMap::Initialize(int size) {
	sizeOfMap = size;
	currSizeOfMap = 0;
	pBucket = new Bucket*[size];
	if(NULL == pBucket){
		cout<<"RpMap::Initialize; Array of bucket could not created"<<endl;
		return -1;
	}
	for(unsigned int i = 0; i < size; i++){
		pBucket[i] = NULL;
	}
	return 0;
}

void RpMap::FreeMap() {
	for(unsigned int i = 0; i < sizeOfMap; i++) {
		Bucket *temp = pBucket[i], *next = NULL;
		while(NULL != temp){
			next = temp->nextBucket;
			delete temp;
			temp = next;
		}
	}
	delete pBucket;
}

void RpMap::MapSet(int key, Page *page){

	int i = HashKey(key, sizeOfMap);
	Bucket *temp = pBucket[i], *newBucket = NULL;
	while(NULL != temp){
		if(key == temp->key) {
			temp->page = page;
			return;
		}
		temp = temp->nextBucket;
	}
	newBucket = new Bucket();
	if(NULL == newBucket) {
		cout<<"RpMap::MapSet; new bucket could not created"<<endl;
		return;
	}
	newBucket->page = page;
	newBucket->key = key;
	newBucket->nextBucket = pBucket[i];
	pBucket[i] = newBucket;
	currSizeOfMap++;

}

void RpMap::MapUnset(int key){
	int i = HashKey(sizeOfMap, key);
	Bucket *temp = pBucket[i], *prev = temp;
	while(NULL != temp)	{
		if(key == temp->key) {
			if(pBucket[i] == temp){
				pBucket[i] = temp->nextBucket;
			} else {
				prev->nextBucket = temp->nextBucket;
			}
			delete temp;
			sizeOfMap--;
			return;
		}
		prev = temp;
		temp = temp->nextBucket;
	}
}

Page *RpMap::MapGet(int key) {
  int i = HashKey(key, sizeOfMap);
  Bucket *temp = pBucket[i];
  while (NULL != temp) {
    if (temp->key == key) {
      return temp->page;
    }   
    temp = temp->nextBucket;
  }
  return NULL;
}


int RpMap::HasMap(int key){
	int i = HashKey(key, sizeOfMap);
  Bucket *temp = pBucket[i];
  while (NULL != temp) {
    if (temp->key == key) {
      return 1; // true
    }
    temp = temp->nextBucket;
  }
  return 0; // false
}
