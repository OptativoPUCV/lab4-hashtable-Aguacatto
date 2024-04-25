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

  for(long i = 0; i < map->capacity; i++)
  {
    long currentIndex = (pos + i) % map->capacity;
    if(map->buckets[currentIndex] != NULL && is_equal(map->buckets[currentIndex]->key, key) == 1)
    {
      return;
    }
  }
  
  if(map->buckets[pos]== NULL)
  {
    Pair* newPair = malloc(sizeof(Pair));
    newPair->key = key;
    newPair->value = value;
    map->buckets[pos] = newPair;
    map->current = pos;
    map->size++;
  }
  else
  {
    long newPos = pos;
    while(map->buckets[newPos] != NULL && map->buckets[newPos]->key != NULL)
    {
      newPos = (newPos + 1) % map->capacity;
    }
    Pair* newPair = malloc(sizeof(Pair));
    newPair->key = key;
    newPair->value = value;
    map->buckets[newPos] = newPair;
    map->current = newPos;
    map->size++;
  }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
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
  if(is_equal(map->buckets[pos]->key, key) == 1)
  {
    map->current = pos;
    return map->buckets[pos];
  }
  else
  {
    long newPos = pos;
    while(map->buckets[newPos] != NULL)
    {
      if(is_equal(map->buckets[newPos]->key, key) == 1)
      {
        map->current = newPos;
        return map->buckets[newPos];
      }
      newPos = (newPos + 1) % map->capacity;
    }
    return NULL;
  }
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

Pair * nextMap(HashMap * map) {

    return NULL;
}
