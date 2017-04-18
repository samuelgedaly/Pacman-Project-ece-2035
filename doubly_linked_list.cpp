#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->current = NULL;
    newList->size = 0;
    return newList;
}

LLNode* create_llnode(void* data) {
    
  //Your code here.
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->data = data;
    newNode->previous = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertHead(DLinkedList* dLinkedList, void* data){
  LLNode* newNode = create_llnode(data);
  if(dLinkedList->head == NULL){
    dLinkedList->size++;
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
  }else{
    dLinkedList->size++;
    newNode->next = dLinkedList->head;
    (dLinkedList->head)->previous = newNode;
    dLinkedList->head = newNode;
  }
  return;
}


void insertTail(DLinkedList* dLinkedList, void* data){
  
  //Your code here.
    LLNode* newNode = create_llnode(data);
    if(dLinkedList->head == NULL){
        dLinkedList->size++;
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
    }else{
        dLinkedList->size++;
        newNode->previous = dLinkedList->tail;
        (dLinkedList->tail)->next = newNode;
        dLinkedList->tail = newNode;
    }
    return;
}

int insertAfter(DLinkedList* dLinkedList, void* newData){
  
  //Your code here.
    if (dLinkedList->current == NULL){
        return 0;
    }else{
        LLNode* newNode = create_llnode(newData);
        if((dLinkedList->current)->next == NULL){
            insertTail(dLinkedList, newData);
        }else{
            dLinkedList->size++;
            newNode->next = (dLinkedList->current)->next;
            newNode->previous = dLinkedList->current;
            ((dLinkedList->current)->next)->previous = newNode;
            (dLinkedList->current)->next = newNode;
        }
        return 1;
    }
}


int insertBefore(DLinkedList* dLinkedList, void* newData){
  
  //Your code here.
    if (dLinkedList->current == NULL){
        return 0;
    }else{
        LLNode* newNode = create_llnode(newData);
        if((dLinkedList->current)->previous == NULL){
            insertHead(dLinkedList, newData);
        }else{
            dLinkedList->size++;
            newNode->next = dLinkedList->current;
            newNode->previous = (dLinkedList->current)->previous;
            ((dLinkedList->current)->previous)->next = newNode;
            (dLinkedList->current)->previous = newNode;
        }
        return 1;
    }

}



void* deleteBackward(DLinkedList* dLinkedList){
  
  //Your code here.
    LLNode* buff = dLinkedList->current;
    if (dLinkedList -> size == 1) {
            dLinkedList -> current = NULL;
            dLinkedList -> head = NULL;
            dLinkedList -> tail = NULL;
            dLinkedList -> size--;
            free(buff);
            return NULL;
        }
    if(dLinkedList->current == NULL) {
        free(buff);
            return NULL;
    } 
    else if(dLinkedList->current == dLinkedList->head) {
        dLinkedList->head = (dLinkedList->head)->next;
        dLinkedList->current = NULL;
        dLinkedList->size--;
        free(buff);
        return NULL; //////(dLinkedList->current)->data
    } else if(dLinkedList->current == dLinkedList->tail) {
        dLinkedList->tail = (dLinkedList->tail)->previous;
        dLinkedList->current = dLinkedList->tail;
        dLinkedList->size--;
        free(buff);
        return(dLinkedList->current)->data;
    } else {
        ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
        ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
        dLinkedList->current = (dLinkedList->current)->previous;
        dLinkedList->size--;
        if(dLinkedList->current == NULL) {
            free(buff);
            return NULL;
        } else {
            free(buff);
            return (dLinkedList->current)->data;
        }
    }
}



void* deleteForward(DLinkedList* dLinkedList){
  
  //Your code here.
    LLNode* buff = dLinkedList->current;
    if (dLinkedList -> size == 1) {
            dLinkedList -> current = NULL;
            dLinkedList -> head = NULL;
            dLinkedList -> tail = NULL;
            dLinkedList -> size--;
            free(buff);
            return NULL;
        }
    if(dLinkedList->current == NULL) {
            free(buff);
            return NULL;
    }
    else if(dLinkedList->current == dLinkedList->head) {
        dLinkedList->head = (dLinkedList->head)->next;
        dLinkedList->current = dLinkedList->head;
        dLinkedList->size--;
        free(buff);
        return (dLinkedList->current)->data;
    } else if(dLinkedList->current == dLinkedList->tail) {
        dLinkedList->tail = (dLinkedList->tail)->previous;
        dLinkedList->current = NULL;
        dLinkedList->size--;
        free(buff);
        return (dLinkedList->current)->data;
    } else {
        ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
        ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
        dLinkedList->current = (dLinkedList->current)->next;
        dLinkedList->size--;
        if(dLinkedList->current == NULL) {
            free(buff);
            return NULL;
        } else {
            free(buff);
            return (dLinkedList->current)->data;
        }
    }
    
}


void destroyList(DLinkedList* dLinkedList){
  if(dLinkedList->head != NULL){
    getHead(dLinkedList);
    while(deleteForward(dLinkedList)){};
  }
  free(dLinkedList);
}



void* getHead(DLinkedList* dLinkedList){
  
  //Your code here.
    if (dLinkedList->head == NULL) {
        return NULL;
    } else {
        dLinkedList->current = dLinkedList->head;
        return (dLinkedList->head)->data;
    }
}

void* getTail(DLinkedList* dLinkedList){
  
  //Your code here.
    if (dLinkedList->tail == NULL) {
        return NULL;
    } else {
        dLinkedList->current = dLinkedList->tail;
        return (dLinkedList->tail)->data;
    }
}



void* getCurrent(DLinkedList* dLinkedList){
  
  //Your code here.
    if (dLinkedList->current == NULL){
        return NULL;
    } else {
        return (dLinkedList->current)->data;
    }
}



void* getNext(DLinkedList* dLinkedList){
  
  //Your code here.
    if (dLinkedList->current == NULL) {
        return NULL;
    } else if (dLinkedList->current == dLinkedList->tail) {
        dLinkedList->current = NULL;
        return NULL;
    } else {
        dLinkedList->current = (dLinkedList->current)->next;
        return (dLinkedList->current)->data;
    }
}



void* getPrevious(DLinkedList* dLinkedList){
  
  //Your code here.
    if (dLinkedList->current == NULL) {
        return NULL;
    } else if (dLinkedList->current == dLinkedList->head) {
        dLinkedList->current = NULL;
        return NULL;
    } else {
        dLinkedList->current = (dLinkedList->current)->previous;
        return (dLinkedList->current)->data;
    }
}


int getSize(DLinkedList* dLinkedList){
  
  //Your code here.
    return dLinkedList->size;
}

