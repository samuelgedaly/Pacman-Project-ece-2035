#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

void vector_init(vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = (int*)malloc(sizeof(int) * v->capacity);
}

int vector_total(vector *v)
{
    return v->total;
}



void vector_add(vector *v, int item)
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

static void vector_resize(vector *v, int capacity)
{

    int *items = (int*)realloc(v->items, sizeof(int) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

int vector_get(vector *v, int index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

void vector_delete(vector *v, int index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;

    for (int i = 0; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize(v, v->capacity / 2);
}

void vector_free(vector *v)
{
    free(v->items);
}