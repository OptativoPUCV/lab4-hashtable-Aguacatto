#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) 
{
  long pos = hash(key,map->capacity);
  Pair* newPair = createPair(key,value);
  
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key) == 0)
  {
    pos = (pos + 1) % map->capacity;
  }
  if(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL)
  {
    map->buckets[pos] = newPair;
    map->size++;
    map->current = pos;
  }
}

void enlarge(HashMap * map)
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** oldBuckets = map->buckets;
  map->capacity *= 2;
  map->buckets = (Pair**)malloc(sizeof(Pair*) * map->capacity);
  for(int i = 0; i < map->capacity; i++)
  {
    map->buckets[i] = NULL;
  }
  map->size = 0;
  for(long i = 0; i < map->capacity / 2; i++)
  {
    if(oldBuckets[i] != NULL && oldBuckets[i]->key != NULL)
    {
      insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
    }
  }
  free(oldBuckets); 
}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)malloc(sizeof(Pair*) * capacity);
  for(int i = 0; i < capacity; i++)
  {
    map->buckets[i] = NULL;
  }
  
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
  ;
}

void eraseMap(HashMap * map,  char * key)
{
  Pair* par = searchMap(map, key);
  if(par != NULL)
  {
    par->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key)
{
  long pos = hash(key,map->capacity);
  while(map->buckets[pos] != NULL)
  {
    if(is_equal(map->buckets[pos]->key, key) == 1)
    {
      map->current = pos;
      return map->buckets[pos];
    }
    pos = (pos + 1) % map->capacity;
  }
  return NULL;
}

Pair * firstMap(HashMap * map)
{
  for(long i = 0; i < map->capacity; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map)
{
  for(int i = map->current; i < map->capacity; i++)
  {
    if(map->buckets[i+1] != NULL && map->buckets[i+1]->key != NULL)
    {
      map->current = i+1;
      return map->buckets[i+1];
    }
  }
  return NULL;
}
