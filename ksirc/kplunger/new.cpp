#ifdef MEM_DEBUG

#include <stddef.h>
#include <stdlib.h>
#include <iostream.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include <hash_map>

#include "kplunger.h"

typedef struct Smem_info {
  char *id;
  size_t size;
  time_t when;
} mem_info;

void stats(int);

typedef hash_map<unsigned int, const mem_info*, hash<unsigned int> > aMap;
static aMap mapAllocs;
static bool delay_stats = false;

void* operator new(size_t sizeofX){
    return operator new(sizeofX, "Unkown");
}

void* operator new(size_t sizeofX, char *id){
    mem_info *mi = 0x0;
    void *p = 0x0;

    delay_stats = true;
    
    static int set = 0;
    
    if(set == 0){
        set = 1;
        signal(SIGALRM, stats);
        alarm(10);
    }
//    printf("Did Malloc() ID: %s Bytes: %d\n", id, sizeofX);

    p = malloc(sizeofX);
    if(p == 0x0){
        fprintf(stderr, "new call failed for size: %d for id: %s", sizeofX, id);
    }
    mi = (mem_info *) malloc(sizeof(mem_info));
    if(mi == 0x0){
        fprintf(stderr, "new call failed for mem_info: %d for id: %s", sizeof(mem_info), id);
    }
    mi->id = id;
    mi->size = sizeofX;
    mi->when = time(NULL);
    
    mapAllocs[(unsigned int) p] = mi;

    delay_stats = false;
    
    return p;
}

void* operator new[](size_t sizeofX){
    void *p = operator new(sizeofX, "Unkown[]");
//    printf("Did new[] ID: Unkown[] Bytes: %d P: %x\n", sizeofX, p);
    return p;
}

void* operator new[](size_t sizeofX, char *id){
    void *p = operator new(sizeofX, id);
//    printf("Did Malloc() ID: %s Bytes: %d P: %x\n", id, sizeofX, p);
    return p;
}

void operator delete(void *p){
    //    cerr << "Did free() ID: " << mapAllocs[(unsigned int) p] << "\n";
    /*
     * mem info lso freed since it's part of p
     */
    const mem_info *mi = mapAllocs[(unsigned int) p];
    mapAllocs.erase((unsigned int) p);
    free(p);
    free((void *)mi);
}

void operator delete[](void *p){
//  printf("Did delete[] ID: %s P: %x\n", mapAllocs[(unsigned int) p]->id, p);
    operator delete(p);
}


void stats(int){
    if(delay_stats == true){
        signal(SIGALRM, stats);
        alarm(1);
        return;
    }
    int bytes_allocated = 0;
    const mem_info *mi = 0x0;
    hash_map<const char*, int, hash<const char*> > numAllocs;
    aMap::const_iterator it = mapAllocs.begin();
    for(; it != mapAllocs.end(); ++it){
        mi = (*it).second;
        bytes_allocated += mi->size;
        numAllocs[mi->id] = numAllocs[mi->id]+1;
    }
    hash_map<const char *, int, hash<const char *> >::const_iterator it2 = numAllocs.begin();
    for(; it2 != numAllocs.end(); ++it2){
        cerr << time(NULL) << " " << (*it2).first << ": " << (*it2).second << endl;
    }
    fprintf(stderr, "Size: %d\n", mapAllocs.size());
    fprintf(stderr, "Total Memory Used: %d\n",  bytes_allocated);
    signal(SIGALRM, stats);
    alarm(10);
}

#endif
