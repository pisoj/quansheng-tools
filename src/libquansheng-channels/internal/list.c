#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct qdc_ListItem qdc_ListItem;
typedef struct qdc_ListItem {
    void *data;
    qdc_ListItem *next;
} qdc_ListItem;

struct qdc_List {
    qdc_ListItem *first;
};

qdc_ListErr qdc_listNew(qdc_List **list) {
    qdc_List *newList = malloc(sizeof(qdc_List));
    if (newList == NULL) { return qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY; }
    memset(newList, 0, sizeof(*newList));
    *list = newList;
    return qdc_ListErr_NONE;
}

qdc_ListIndexErr qdc_listGetFirst(qdc_List *list, void **data) {
    if (list->first == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    *data = list->first->data;
    return qdc_ListIndexErr_NONE;
}

qdc_ListItem *_qdc_listGetLast(qdc_ListItem *anyItem) {
    while (anyItem->next != NULL) {
        anyItem = anyItem->next;
    }
    return anyItem;
}
qdc_ListIndexErr qdc_listGetLast(qdc_List *list, void **data) {
    if (list->first == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    *data = _qdc_listGetLast(list->first)->data;
    return qdc_ListIndexErr_NONE;
}

qdc_ListIndexErr _qdc_listGet(qdc_List *list, unsigned long targetIndex, qdc_ListItem **destination) {
    qdc_ListItem *currentItem = list->first;
    unsigned long currentIndex = 0;
    if (list->first == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    while (currentIndex < targetIndex && currentItem->next != NULL) {
        currentItem = currentItem->next;
        currentIndex++;
    }
    if (currentIndex < targetIndex) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    *destination = currentItem;
    return qdc_ListIndexErr_NONE;
}
qdc_ListIndexErr qdc_listGet(qdc_List *list, unsigned long targetIndex, void **data) {
    qdc_ListItem *item;
    qdc_ListIndexErr error = _qdc_listGet(list, targetIndex, &item);
    if (error != qdc_ListIndexErr_NONE) { return error; }
    *data = item->data;
    return qdc_ListIndexErr_NONE;
}

qdc_ListErr qdc_listPrepend(qdc_List *list, void *data) {
    qdc_ListItem *newItem = malloc(sizeof(qdc_ListItem));
    if (newItem == NULL) { return qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY; }
    newItem->data = data;
    newItem->next = list->first;
    list->first = newItem;
    return qdc_ListErr_NONE;
}

qdc_ListErr _qdc_listAppend(qdc_ListItem *anyItem, void *data) {
    qdc_ListItem *newItem = malloc(sizeof(qdc_ListItem));
    if (newItem == NULL) { return qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY; }
    newItem->data = data;
    newItem->next = NULL;
    _qdc_listGetLast(anyItem)->next = newItem;
    return qdc_ListErr_NONE;
}
qdc_ListErr qdc_listAppend(qdc_List *list, void *data) {
    if (list->first == NULL) {
        return qdc_listPrepend(list, data);
    }
    return _qdc_listAppend(list->first, data);
}

qdc_ListErr _qdc_listInsert(qdc_ListItem *insertAfter, void *data) {
    qdc_ListItem *newItem = malloc(sizeof(qdc_ListItem));
    if (newItem == NULL) { return qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY; }
    newItem->data = data;
    newItem->next = insertAfter->next;
    insertAfter->next = newItem;
    return qdc_ListErr_NONE;
}
qdc_ListInsertErr qdc_listInsert(qdc_List *list, unsigned long targetIndex, void *data) {
    if (targetIndex == 0) {
        switch (qdc_listPrepend(list, data)) {
            case qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY:
                return qdc_ListInsertErr_FAILED_TO_ALLOCATE_MEMORY;
            case qdc_ListErr_NONE:
                return qdc_ListInsertErr_NONE;
        }
    }

    qdc_ListItem *insertAfter;
    if(_qdc_listGet(list, targetIndex - 1, &insertAfter) != qdc_ListIndexErr_NONE) {
        return qdc_ListInsertErr_INDEX_OUT_OF_BOUNDS;
    }
    switch (_qdc_listInsert(insertAfter, data)) {
        case qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY:
            return qdc_ListInsertErr_FAILED_TO_ALLOCATE_MEMORY;
        default:
            return qdc_ListInsertErr_NONE;
    }
}

qdc_ListIndexErr qdc_listRemoveFirst(qdc_List *list, void (*freeElement)(void *)) {
    if (list->first == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    qdc_ListItem *first = list->first;
    list->first = first->next;
    if (freeElement != NULL) { freeElement(first->data); }
    free(first);
    return qdc_ListIndexErr_NONE;
}

qdc_ListIndexErr qdc_listRemove(qdc_List *list, unsigned long targetIndex, void (*freeElement)(void *)) {
    if (targetIndex == 0) {
        return qdc_listRemoveFirst(list, freeElement);
    }

    qdc_ListItem *removeAfter;
    qdc_ListIndexErr error = _qdc_listGet(list, targetIndex - 1, &removeAfter);
    if (error != qdc_ListIndexErr_NONE) { return error; }
    if (removeAfter->next == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    qdc_ListItem *remove = removeAfter->next;
    removeAfter->next = remove->next;
    if (freeElement != NULL) { freeElement(remove->data); }
    free(remove);
    return qdc_ListIndexErr_NONE;
}

qdc_ListIndexErr qdc_listRemoveLast(qdc_List *list, void (*freeElement)(void *)) {
    qdc_ListItem *previous = NULL;
    qdc_ListItem *current = list->first;
    if (current == NULL) { return qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS; }
    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }
    if (previous != NULL) {
        previous->next = NULL;
    } else {
        list->first = NULL;
    }
    if (freeElement != NULL) { freeElement(current->data); }
    free(current);
    return qdc_ListIndexErr_NONE;
}

unsigned long qdc_listLength(qdc_List *list) {
    unsigned long length = 0;
    qdc_ListItem *current = list->first;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

void qdc_listDestroy(qdc_List *list, void (*freeElement)(void *)) {
    qdc_ListItem *current = list->first;
    while (current != NULL) {
        qdc_ListItem *next = current->next;
        if (freeElement != NULL) { freeElement(current->data); }
        free(current);
        current = next;
    }
    free(list);
}
