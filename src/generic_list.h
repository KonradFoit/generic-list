/*
 * list.h
 *
 *  Created on: 20 maj 2021
 *      Author: konra
 */

#ifndef SRC_TOOLS_GENERIC_LIST_H_
#define SRC_TOOLS_GENERIC_LIST_H_

#include "generic_list.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LIST_SUCCESS = 0,
    LIST_INVALID_PARAM,
    LIST_NO_MEM,
    LIST_NOT_FOUND,
    LIST_EMPTY,
    LIST_NOT_IMPLEMENTED,
    LIST_INTERNAL_ERROR
}list_error_t;

typedef void (*freeData)(void*);
typedef void* (*allocData)(size_t);

typedef struct list_node_t
{
    void* data;
    struct list_node_t* next;
    struct list_node_t* prev;
}generic_list_node_t;

typedef struct
{
    size_t size;
    generic_list_node_t* head;
    generic_list_node_t* tail;
    generic_list_node_t* current;
    freeData freeFunc;
    allocData allocFunc;
}generic_list_t;

/** @brief Create new generic list
 *
 * @param[in]   list        pointer to list context structure
 * @param[in]   freeFunc    pointer to function used to free memory @ref freeData
 * @param[in]   allocFunc   pointer to dunction used to allocate memory @ref allocData
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_newList(generic_list_t* list, freeData freeFunc, allocData allocFunc);

/** @brief Append list with new element
 *
 * @param[in]   list   pointer to list context structure
 * @param[in]   data   pointer to data that will be stored in the list
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_append(generic_list_t* list, void* data);

/** @brief insert new element into the list at given position
 *
 * @param[in]   list    pointer to list context structure
 * @param[in]   data    pointer to data that will be stored in the list
 * @param[in]   index   index at which new element will be inserted
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_insert(generic_list_t* list, void* data, unsigned int index);

/** @brief Free list and its elements
 *         NOTE: Data stored in the list will also be freed!
 *
 * @param[in]   list   pointer to list context structure
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_freeList(generic_list_t* list);

/** @brief Get element at position
 *
 * @param[in]    list    pointer to list context structure
 * @param[in]    index   element index
 * @param[out]   data    pointer to @ref generic_list_node_t pointer that will be set to element at given index
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_getElementAt(generic_list_t* list, unsigned int index, generic_list_node_t** data);

/** @brief Get elements data at position
 *
 * @param[in]    list    pointer to list context structure
 * @param[in]    index   element index
 * @param[out]   data    pointer to a pointer which will be set to data at element at given index
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_getDataAt(generic_list_t* list, unsigned int index, void** data);

/** @brief remove element at given index from list
 *
 * @param[in]   list    pointer to list context structure
 * @param[in]   index   index at which element will be removed
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_removeElementAt(generic_list_t* list, unsigned int index);

/** @brief Set currently selected element as head
 *
 * @param[in]   list   pointer to list context structure
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_rewind(generic_list_t* list);

/** @brief Move currently selected element pointer to next
 *
 * @param[in]   list    pointer to list context structure
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_next(generic_list_t* list);

/** @brief Check if currently selected element pointer is NULL
 *         and therefore is at the end of the list
 *
 * @param[in]   list    pointer to list context structure
 *
 * @return true is list has ended
 */
bool genericList_isAtEnd(generic_list_t* list);

/** @brief Check if currently selected element is last (next pointer is NULL)
 *
 * @param[in]   list    pointer to list context structure
 *
 * @return true if currently selected element is last
 */
bool genericList_isAtLastElement(generic_list_t* list);

/** @brief Get currently selected node
 *
 * @param[in]    list   pointer to list context structure
 * @param[out]   data   pointer to @ref generic_list_node_t pointer that will be set to currently selected element
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_getCurrentElement(generic_list_t* list, generic_list_node_t** data);

/** @brief Get currently selected elements data
 *
 * @param[in]    list   pointer to list context structure
 * @param[out]   data   pointer to a pointer which will be set to data of currently selected element
 *
 * @return LIST_SUCCESS on success, error code otherwise. @ref list_error_t
 */
list_error_t genericList_getCurrentData(generic_list_t* list, void** data);

#endif /* SRC_TOOLS_GENERIC_LIST_H_ */
