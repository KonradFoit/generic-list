#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "generic_list.h"

#define MAX_ALLOCATED_BLOCKS     (256)

typedef struct
{
    void* ptr;
    size_t allocatedSize;
}memory_block_t;

static memory_block_t memoryTrace[MAX_ALLOCATED_BLOCKS];

static volatile uint32_t allocatedMem = 0;

void* tracedMalloc(size_t requestedSize)
{
    void* res = NULL;

    /* Find space in memory trace */
    for ( uint32_t i = 0; i < MAX_ALLOCATED_BLOCKS; i++ )
    {
        if ( 0 == memoryTrace[i].allocatedSize )
        {
            /* Got place in memory trace */
            res = malloc(requestedSize);
            if ( NULL != res )
            {
                memoryTrace[i].ptr = res;
                memoryTrace[i].allocatedSize = requestedSize;
                allocatedMem += requestedSize;
            }
            break;
        }
    }
    return res;
}

void tracedFree(void* ptr)
{
    /* Find address in memory trace */
    for ( uint32_t i = 0; i < MAX_ALLOCATED_BLOCKS; i++ )
    {
        if ( ptr == memoryTrace[i].ptr)
        {
            /* Got trace */
            memoryTrace[i].ptr = NULL;
            allocatedMem -= memoryTrace[i].allocatedSize;
            memoryTrace[i].allocatedSize = 0;
            free(ptr);
        }
    }
}

START_TEST(generic_list_create)
{
    generic_list_t list;
    list_error_t err;
    /* Create new list */
    err =  genericList_newList(&list, tracedFree, tracedMalloc);
    ck_assert_int_eq(err, LIST_SUCCESS);

    /* Verify that list structure is initialized */
    ck_assert_ptr_eq(list.current, NULL);
    ck_assert_ptr_eq(list.head, NULL);
    ck_assert_ptr_eq(list.tail, NULL);
    ck_assert_int_eq(list.size, 0);

}
END_TEST

START_TEST(generic_list_append_and_free)
{
    generic_list_t list;
    list_error_t err;
    /* Allocate data for two elements */
    uint32_t memStart = allocatedMem;
    uint8_t* data1 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    uint8_t* data2 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    ck_assert_ptr_ne(data1, NULL);
    ck_assert_ptr_ne(data2, NULL);
    *data1 = 1;
    *data2 = 2;
    /* Create new list */
    err =  genericList_newList(&list, tracedFree, tracedMalloc);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err =  genericList_append(&list, data1);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err =  genericList_append(&list, data2);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_ptr_eq(list.head->data, data1);
    ck_assert_ptr_eq(list.head->next->data, data2);
    ck_assert_ptr_eq(list.tail->data, data2);
    ck_assert_ptr_eq(list.tail->prev->data, data1);

    err = genericList_freeList(&list);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_int_eq(memStart, allocatedMem);
}
END_TEST

START_TEST(generic_list_insert)
{
    generic_list_t list;
    list_error_t err;
    /* Allocate data for two elements */
    uint32_t memStart = allocatedMem;
    uint8_t* data1 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    uint8_t* data2 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    uint8_t* data3 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    ck_assert_ptr_ne(data1, NULL);
    ck_assert_ptr_ne(data2, NULL);
    *data1 = 1;
    *data2 = 2;
    *data3 = 3;
    /* Create new list */
    err =  genericList_newList(&list, tracedFree, tracedMalloc);
    ck_assert_int_eq(err, LIST_SUCCESS);

    err = genericList_insert(&list, data1, 0);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err = genericList_insert(&list, data3, 1);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err = genericList_insert(&list, data2, 1);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_ptr_eq(list.head->data, data1);
    ck_assert_ptr_eq(list.head->next->data, data2);
    ck_assert_ptr_eq(list.head->next->next->data, data3);
    ck_assert_ptr_eq(list.tail->data, data3);
    ck_assert_ptr_eq(list.tail->prev->data, data2);
    ck_assert_ptr_eq(list.tail->prev->prev->data, data1);

    err = genericList_freeList(&list);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_int_eq(memStart, allocatedMem);
}
END_TEST

START_TEST(generic_list_remove)
{
    generic_list_t list;
    list_error_t err;
    /* Allocate data for two elements */
    uint32_t memStart = allocatedMem;
    uint8_t* data1 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    uint8_t* data2 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    uint8_t* data3 = (uint8_t*)tracedMalloc(sizeof(uint8_t));
    ck_assert_ptr_ne(data1, NULL);
    ck_assert_ptr_ne(data2, NULL);
    *data1 = 1;
    *data2 = 2;
    *data3 = 3;
    /* Create new list */
    err =  genericList_newList(&list, tracedFree, tracedMalloc);
    ck_assert_int_eq(err, LIST_SUCCESS);

    err = genericList_append(&list, data1);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err = genericList_append(&list, data2);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err = genericList_append(&list, data3);
    ck_assert_int_eq(err, LIST_SUCCESS);
    err = genericList_removeElementAt(&list, 1);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_ptr_eq(list.head->data, data1);
    ck_assert_ptr_eq(list.head->next->data, data3);
    ck_assert_ptr_eq(list.tail->data, data3);
    ck_assert_ptr_eq(list.tail->prev->data, data1);

    err = genericList_freeList(&list);
    ck_assert_int_eq(err, LIST_SUCCESS);

    ck_assert_int_eq(memStart, allocatedMem);
}
END_TEST


Suite * generic_list_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("generic-list");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, generic_list_create);
    tcase_add_test(tc_core, generic_list_append_and_free);
    tcase_add_test(tc_core, generic_list_insert);
    tcase_add_test(tc_core, generic_list_remove);

    suite_add_tcase(s, tc_core);

    return s;
}

START_TEST(tools_malloc_free_manual8)
{
    uint32_t startMem = allocatedMem;
    size_t mallocSize = 128;
    void* ptr1 = tracedMalloc(mallocSize);

    ck_assert_int_eq(startMem, allocatedMem - mallocSize);

    void* ptr2 = tracedMalloc(mallocSize);
    void* ptr3 = tracedMalloc(mallocSize);
    void* ptr4 = tracedMalloc(mallocSize);
    void* ptr5 = tracedMalloc(mallocSize);
    void* ptr6 = tracedMalloc(mallocSize);
    void* ptr7 = tracedMalloc(mallocSize);
    void* ptr8 = tracedMalloc(mallocSize);

    ck_assert_int_eq(startMem, allocatedMem - (8 * mallocSize) );

    tracedFree(ptr1);
    tracedFree(ptr2);
    tracedFree(ptr3);
    tracedFree(ptr4);
    tracedFree(ptr5);
    tracedFree(ptr6);
    tracedFree(ptr7);
    tracedFree(ptr8);

    ck_assert_int_eq(startMem, allocatedMem);

}

START_TEST(tools_malloc_free_loop_all)
{
    uint32_t memStart = allocatedMem;
    size_t mallocSize = 16;
    void** memtable;
    memtable = (void**)malloc(sizeof(void*)*MAX_ALLOCATED_BLOCKS);

    for ( uint32_t i = 0; i < MAX_ALLOCATED_BLOCKS; i++)
    {
        memtable[i] = tracedMalloc(mallocSize);
    }

    ck_assert_int_eq(memStart, allocatedMem - MAX_ALLOCATED_BLOCKS * mallocSize);

    for ( uint32_t i = 0; i < MAX_ALLOCATED_BLOCKS; i++)
    {
        tracedFree(memtable[i]);
    }

    ck_assert_int_eq(memStart, allocatedMem);
    free(memtable);

}

Suite * tools_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("tools-test");

    /* Core test case */
    tc_core = tcase_create("Tools");

    tcase_add_test(tc_core, tools_malloc_free_manual8);
    tcase_add_test(tc_core, tools_malloc_free_loop_all);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed = 0;
    Suite *listSuite;
    Suite *toolsSuite;
    SRunner *toolsSr;
    SRunner *sr;

    memset(memoryTrace, 0, sizeof(memoryTrace));

    listSuite = generic_list_suite();
    toolsSuite = tools_suite();

    toolsSr = srunner_create(toolsSuite);

    sr = srunner_create(listSuite);

    printf("Tests start\r\n");
    srunner_run_all(toolsSr, CK_NORMAL);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(toolsSr);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(toolsSr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}
