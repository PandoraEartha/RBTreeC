#include <map>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>
#include "PCG32.h"
#include "RBTree.h"

#define TEST_CASE (1<<29)
#define THREAD_NUMBER 32

static inline void swap(unsigned* array,unsigned index0,unsigned index1){
	unsigned temporary=array[index1];
	array[index1]=array[index0];
	array[index0]=temporary;
}

int main(int argc, char const *argv[]){
	struct timeval start,end;
	unsigned insertTime[2];
	unsigned searchTime[2];
	unsigned eraseTime[2];
	unsigned clearTime[2];

	unsigned* keys=(unsigned*)malloc(TEST_CASE*sizeof(unsigned));
	unsigned* values=(unsigned*)malloc(TEST_CASE*sizeof(unsigned));

	PCG32Struct PCGStatus;
	PCGStatus.seed=time(NULL);
	PCG32Init(&PCGStatus);
	for(unsigned index=0;index<TEST_CASE;index++){
		keys[index]=PCG32(&PCGStatus);
		values[index]=index;
	}
	std::map<long long unsigned int,long long unsigned int> stdMap;
	Head* RBTree=RBTreeCreate();

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeInsert(RBTree,keys[index],values[index]);
	}
	gettimeofday(&end,NULL);
	insertTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		long long unsigned int value;
		RBTreeSearch(RBTree,keys[index],&value);
	}
	gettimeofday(&end,NULL);
	searchTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeDelete(RBTree,keys[index]);
	}
	gettimeofday(&end,NULL);
	eraseTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeInsert(RBTree,keys[index],values[index]);
	}
	gettimeofday(&start,NULL);
	RBTreeClear(RBTree);
	gettimeofday(&end,NULL);
	clearTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		stdMap[keys[index]]=values[index];
	}
	gettimeofday(&end,NULL);
	insertTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	for(unsigned index=0;index<TEST_CASE;index++){
		swap(keys,index,PCG32(&PCGStatus)&(TEST_CASE-1));
	}
	
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		stdMap.find(keys[index]);
	}
	gettimeofday(&end,NULL);
	searchTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		stdMap.erase(keys[index]);
	}
	gettimeofday(&end,NULL);
	eraseTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	for(unsigned index=0;index<TEST_CASE;index++){
		stdMap[keys[index]]=values[index];
	}

	gettimeofday(&start,NULL);
	stdMap.clear();
	gettimeofday(&end,NULL);
	clearTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	printf("RBTreeInsert(): %u milliseconds\n",insertTime[0]);
	printf("std::map overload []: %u milliseconds\n\n",insertTime[1]);
	printf("RBTreeSearch(): %u milliseconds\n",searchTime[0]);
	printf("std::map::find(): %u milliseconds\n\n",searchTime[1]);
	printf("RBTreeDelete(): %u milliseconds\n",eraseTime[0]);
	printf("std::map::erase(): %u milliseconds\n\n",eraseTime[1]);
	printf("RBTreeClear(): %u milliseconds\n",clearTime[0]);
	printf("std::map::clear(): %u milliseconds\n\n",clearTime[1]);
	return 0;
}