#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/

// Pair structure used to store the key and value
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/

// stores capacity
typedef struct BasicHashTable {
  int capacity;
  Pair **storage; //pointer pointing to storage array filled with pointers pointing to pairs
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  char *new_key = malloc(sizeof(char)*strlen(key)+1);
  char *new_value = malloc(sizeof(char)*strlen(value)+1);
  strcpy(new_key, key);
  strcpy(new_value, value);
  pair->key = new_key;
  pair->value = new_value;
  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity; // equivalent to (*ht).capacity = capacity; //here we're dereferencing the pointer and accessing the variable stored in the struct called capacity
  ht->storage = calloc(capacity, sizeof(Pair *)); //calloc same as malloc except it initializes all value to null

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);
  Pair *pair = create_pair(key, value);

  Pair *stored_pair = ht->storage[index];
  if (stored_pair != NULL) {  //a collision is when we're looking in our buckets and the key is different. if the key is the same, then we're just overwriting the key with the same key. 
    if (strcmp(key,stored_pair->key) != 0) {
      printf("WARNING: Overwriting value: %s, %s with %s, %s\n", 
        stored_pair->key, stored_pair->value, key, value);
    }
    destroy_pair(stored_pair);
  }
  // ht->storage[index] = pair; 
  ht->storage[index] = create_pair(key,value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL) {
    printf("Unable to remove entry with key: %s\n", key);
  } else {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
  }

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index] == NULL) {
    printf("Unable to retrieve entry with key: %s", key );
    return NULL;
  }
  return ht->storage[index]->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      destroy_pair(ht->storage[i]);
      
    }
  }
  free(ht->storage);
  free(ht);

}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
