#include <stdbool.h>

struct node {
  void* data;
  struct node *next;
};

struct linked_list {
  struct node *head;
};

struct node *current_node = NULL;

struct node* search_node(int index, struct linked_list* list) {
  int i = 0;
  current_node = list->head;
  while(i != index) {
    current_node = current_node->next;
    i++;
  }
  return current_node;
};

struct linked_list* new_linked_list() {
  struct linked_list* list = malloc(sizeof(struct linked_list));
  list->head = NULL;
  return list;
};

int linked_list_length(struct linked_list* list) {
  int length = 1;
  
  if(list->head == NULL) {
    return 0;
  }

  current_node = list->head->next;
  while(current_node != list->head) {
    length++;
    current_node = current_node->next;
  }

  return length;
}

void linked_list_insert(void* data, struct linked_list* list) {
  struct node *temp = (struct node*) malloc(sizeof(struct node));
  temp->data = data;

  if(list->head == NULL) {
    list->head = temp;
    temp->next = temp;
  } else {
    current_node = list->head;
    while(current_node->next != list->head) {
      current_node = current_node->next;
    }
    temp->next = list->head;
    current_node->next = temp;
  }
}

int linked_list_get_index(void* data, struct linked_list* list) {
  int i = 0;
  int length = linked_list_length(list);
  current_node = list->head;

  while(i < length) {
    if(current_node->data == data) break;
    current_node = current_node->next;
    i++;
  }
  return i;
}

void linked_list_delete_index(int index, struct linked_list* list) {
  int length = linked_list_length(list);
  
  if(index > length-1) return;

  if(linked_list_length(list) == 1) {
    free(list->head);
    list->head == NULL;
    return;
  }

  if(index == 0) {
    struct node* node = list->head->next;
    search_node(length-1, list)->next = node;
    free(list->head);
    list->head = node;
    return;
  }

  struct node* node = search_node(index, list);
  search_node(index-1, list)->next = node->next;
  free(node);
  return;
}

void linked_list_delete_data(void* data, struct linked_list* list) {
  linked_list_delete_index(linked_list_get_index(data, list), list);
  return;
}

void* linked_list_search(int index, struct linked_list* list) {
  int i = 0;
  current_node = list->head;

  while(i < index) {
    current_node = current_node->next;
    i++;
  }

  return current_node->data;
}

void linked_list_shift_left(struct linked_list* list) {
  list->head = list->head->next;
}
