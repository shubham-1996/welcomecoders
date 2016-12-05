/*TLB CALCULATOR*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
long long modPowerOf2(long long a, long long b);
long long log2(long long n);
long long pageTable[10000000];

int main(int argc, char *args[])
{
	FILE *fp;
   	long long virtualAddress;
	long long wordSize, cacheSize, cacheWays, cacheBlockSize, pageSize;
	long long cacheFrames, cacheSets, virtualPages;
	long long bCacheOffset, bIndex, bTag;
	long long bPageOffset, bNumber;
	fp = fopen (args[1], "r");
	virtualAddress = atoi(args[2]);
	
	fscanf(fp,"%lld %lld %lld %lld %lld",&wordSize, &cacheSize, &cacheWays, &cacheBlockSize, &pageSize);
	
	printf("%-40s %lld\n","Word size:",wordSize);
	printf("%-40s %lld\n","Cache capacity:",cacheSize);
	printf("%-40s %lld\n","Cache associativity:",cacheWays);
	printf("%-40s %lld\n","Cache block size:",cacheBlockSize);
	printf("%-40s %lld\n","Page size:",pageSize);

	cacheFrames = cacheSize/cacheBlockSize;
	cacheSets = cacheFrames/cacheWays;
	virtualPages = (1LL<<wordSize)/pageSize;

	printf("\n");
	printf("%-40s %lld\n","Cache frames:",cacheFrames);
	printf("%-40s %lld\n","Cache sets:",cacheSets);
	printf("%-40s %lld\n","Virtual pages:",virtualPages);

	bCacheOffset = log2(cacheBlockSize);
	bIndex = log2(cacheSets);
	bTag = wordSize - bCacheOffset - bIndex;

	bPageOffset = log2(pageSize);
	bNumber = wordSize - bPageOffset;

	printf("\n");
	printf("%-40s %lld\n","Bits for cache block offset:",bCacheOffset);
	printf("%-40s %lld\n","Bits for cache index:",bIndex);
	printf("%-40s %lld\n","Bits for cache tag:",bTag);
	printf("%-40s %lld\n","Bits for page offset:",bPageOffset);
	printf("%-40s %lld\n","Bits for page number:",bNumber);
	printf("\n");
	printf("%-40s %lld\n","Virtual address:",virtualAddress);
	printf("\n");

	long long cache[cacheSets][cacheWays];
	long long i,j;

	for(i = 0; i < cacheSets; i++)
		for(j = 0; j < cacheWays; j++)
			fscanf(fp,"%lld",&cache[i][j]);

	for(i = 0; i < virtualPages; i++)
	 	fscanf(fp,"%lld",&pageTable[i]);

	long long addPageOffset, addPageNumber, addPhysicalPage, physicalAddress;	

	addPageOffset = modPowerOf2(virtualAddress, pageSize);
	addPageNumber = virtualAddress / pageSize;
	addPhysicalPage = pageTable[addPageNumber]; 

	printf("%-40s %lld\n","Address page offset:",addPageOffset);
	printf("%-40s %lld\n","Address virtual page number:",addPageNumber);

	 if(addPhysicalPage == -1LL)
	 {
	 	printf("%-40s fault\n","Address hit/miss/fault:");
	 	return 0;
	 }

	 physicalAddress = addPhysicalPage * pageSize + addPageOffset;

	 printf("%-40s %lld\n","Address physical page number:",addPhysicalPage);
	 printf("%-40s %lld\n","Physical address:",physicalAddress);

	 long long cacheBlockOffset, cacheIndex, cacheTag;

	 cacheBlockOffset = modPowerOf2(physicalAddress, 1<<bCacheOffset);
	 physicalAddress = physicalAddress>>bCacheOffset;
	 cacheIndex = modPowerOf2(physicalAddress, 1<<bIndex);
	 physicalAddress = physicalAddress>>bIndex;
	 cacheTag = physicalAddress;

	 printf("\n");
	 printf("%-40s %lld\n","Address cache block offset:",cacheBlockOffset);
	 printf("%-40s %lld\n","Address cache index:",cacheIndex);
	 printf("%-40s %lld\n","Address cache tag:",cacheTag);

	 long long hit = 0;
	 for(i = 0; i < cacheWays; i++)
	 	if(cacheTag == cache[cacheIndex][i])
	 	{
	 		hit=1;
	 		break;
	 	}
	if(hit)
	printf("%-40s hit\n","Address hit/miss/fault:");	
	else
	printf("%-40s miss\n","Address hit/miss/fault:");
	return 0;
}


long long log2(long long n) {
	long long r=0;
	while (n>>=1LL) r++;
	return r;
}
long long modPowerOf2(long long a, long long b)
{
	return a & ((1LL<<log2(b))-1);
}