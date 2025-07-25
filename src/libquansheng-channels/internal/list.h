#ifndef _LIBQUANSHANG_CHANNELS_INTERNAL_LIST_H
#define _LIBQUANSHANG_CHANNELS_INTERNAL_LIST_H

typedef struct qdc_List qdc_List;

typedef enum qdc_ListErr {
    qdc_ListErr_NONE = 0,
    qdc_ListErr_FAILED_TO_ALLOCATE_MEMORY = 1,
} qdc_ListErr;

typedef enum qdc_ListIndexErr {
    qdc_ListIndexErr_NONE = 0,
    qdc_ListIndexErr_INDEX_OUT_OF_BOUNDS = 1,
} qdc_ListIndexErr;

typedef enum qdc_ListInsertErr {
    qdc_ListInsertErr_NONE = 0,
    qdc_ListInsertErr_FAILED_TO_ALLOCATE_MEMORY = 1,
    qdc_ListInsertErr_INDEX_OUT_OF_BOUNDS = 2,
} qdc_ListInsertErr;

qdc_ListErr qdc_listNew(qdc_List **list);

qdc_ListIndexErr qdc_listGetFirst(qdc_List *list, void **data);

qdc_ListIndexErr qdc_listGetLast(qdc_List *list, void **data);

qdc_ListIndexErr qdc_listGet(qdc_List *list, unsigned long targetIndex, void **data);

qdc_ListErr qdc_listPrepend(qdc_List *list, void *data);

qdc_ListErr qdc_listAppend(qdc_List *list, void *data);

qdc_ListInsertErr qdc_listInsert(qdc_List *list, unsigned long targetIndex, void *data);

qdc_ListIndexErr qdc_listRemoveFirst(qdc_List *list, void (*freeElement)(void *));

qdc_ListIndexErr qdc_listRemove(qdc_List *list, unsigned long targetIndex, void (*freeElement)(void *));

qdc_ListIndexErr qdc_listRemoveLast(qdc_List *list, void (*freeElement)(void *));

unsigned long qdc_listLength(qdc_List *list);

void qdc_listDestroy(qdc_List *list, void (*freeElement)(void *));

#endif
