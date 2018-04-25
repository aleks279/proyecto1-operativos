#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 100

struct data_item {
  long data;
  long key;
};

struct data_item* hash_array[SIZE];
struct data_item* dummy_item;
struct data_item* item;

long hash_table_hash_code(long key) {
  return key % SIZE;
}

long hash_table_search(long key) {
  long hash_index = hash_table_hash_code(key);

  while(hash_array[hash_index] != NULL) {
    if(hash_array[hash_index]->key == key)
      return hash_array[hash_index]->data;

    ++hash_index;
    hash_index %= SIZE;
  }
  return -1;
}

void hash_table_insert(long key, long data) {
  struct data_item *item = (struct data_item*) malloc(sizeof(struct data_item));
  item->data = data;
  item->key = key;

  long hash_index = hash_table_hash_code(key);

  while(hash_array[hash_index] != NULL && hash_array[hash_index]->key != -1) {
    ++hash_index;

    hash_index %= SIZE;
  }

  hash_array[hash_index] = item;
}

void hash_table_delete(long key) {
  long hash_index = hash_table_hash_code(key);

  while(hash_array[hash_index] != NULL) {
    if(hash_array[hash_index]->key == key) {
      struct data_item* temp = hash_array[hash_index];
      hash_array[hash_index] = dummy_item;
    }

    ++hash_index;
    hash_index %= SIZE;
  }
}
