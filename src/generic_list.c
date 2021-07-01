/****************************************************************************
 * Copyright (C) 2021 by Konrad Foit                                        *
 *                                                                          *
 *   This is a free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Box is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/** @file generic_list.c
 * @author Konrad Foit
 * @date 20.05.2021
 * @brief This is a generic two-way list module with commonly used functions to
 *        operate on the list
 *
 * module for easy handling of a two-way generic list with some commonly used
 * functions to operate on such list
 *
 */

#include "generic_list.h"

list_error_t genericList_newList(generic_list_t* list, freeData freeFunc, allocData allocFunc)
{
    /* validate params */
    if (NULL == list)
    {
        return LIST_INVALID_PARAM;
    }
    if (NULL == freeFunc)
    {
        return LIST_INVALID_PARAM;
    }
    if (NULL == allocFunc)
    {
        return LIST_INVALID_PARAM;
    }
    /* initialize list structure */
    list->size = 0;
    list->allocFunc = allocFunc;
    list->freeFunc = freeFunc;
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    return LIST_SUCCESS;
}

list_error_t genericList_append(generic_list_t* list, void* data)
{
    generic_list_node_t* newNode;
    if (NULL ==list)
    {
        return LIST_INVALID_PARAM;
    }
    newNode = (generic_list_node_t*)list->allocFunc(sizeof(generic_list_node_t));
    if (NULL == newNode)
    {
        return LIST_NO_MEM;
    }
    /* Init new node */
    newNode->next = NULL;
    newNode->prev = NULL;

    /* Set pointer to data */
    newNode->data = data;

    if (NULL == list->tail)
    {
        /* List is empty, new element will be both head and tail */
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        /* Insert at the end */
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    /* Increment list size */
    list->size++;

    return LIST_SUCCESS;
}

list_error_t genericList_insert(generic_list_t* list, void* data, unsigned int index)
{
    generic_list_node_t* oldNode;
    generic_list_node_t* newNode;
    list_error_t err;
    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    if ( list->size < index)
    {
        return LIST_INVALID_PARAM;
    }

    if ( list->size == index)
    {
        /* insert at end == append */
        return genericList_append(list, data);
    }

    if ( 0 == index )
    {
        /* Insert as head */
        newNode = (generic_list_node_t*)list->allocFunc(sizeof(generic_list_node_t));
        newNode->data = data;
        newNode->prev = NULL;
        newNode->next = list->head;
        list->head = newNode;
        list->size++;
        return LIST_SUCCESS;
    }

    /* Insert in the middle */
    err = genericList_getElementAt(list, index, &oldNode);
    if ( LIST_SUCCESS != err )
    {
        return err;
    }
    if ( oldNode == NULL )
    {
        return LIST_NOT_FOUND;
    }
    if ( NULL == oldNode->prev )
    {
        /* prev can be null only for head which would be handled before */
        return LIST_INTERNAL_ERROR;
    }
    /* Allocate new node and change pointers */
    newNode = (generic_list_node_t*)list->allocFunc(sizeof(generic_list_node_t));
    newNode->data = data;
    newNode->prev = oldNode->prev;
    newNode->next = oldNode;
    oldNode->prev->next = newNode;
    oldNode->prev = newNode;
    /* Update list size */
    list->size++;

    return LIST_SUCCESS;
}

list_error_t genericList_freeList(generic_list_t* list)
{
    generic_list_node_t* node;
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }

    /* Get head */
    node = list->head;

    /* Go through each element */
    while ( NULL != node )
    {
        /* Save pointer to next node */
        generic_list_node_t* next = node->next;
        /* Free data */
        list->freeFunc(node->data);
        /* Free node */
        list->freeFunc(node);
        /* go to next node */
        node = next;
    }

    /* Clear head, tail and list size */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return LIST_SUCCESS;
}

list_error_t genericList_getElementAt(generic_list_t* list, unsigned int index, generic_list_node_t** data)
{
    generic_list_node_t* node;

    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    if ( NULL == data )
    {
        return LIST_INVALID_PARAM;
    }
    if ( index >= list->size )
    {
        return LIST_INVALID_PARAM;
    }

    /* Get head */
    node = list->head;
    /* Go to element at index but watch for the end*/
    for ( unsigned int i = 0; ( ( i < index ) && ( NULL != node)); i++ )
    {
        node = node->next;
    }
    *data = node;
    /* If node is null then element has not been found */
    if ( NULL != node )
    {
        return LIST_SUCCESS;
    }
    else
    {
        return LIST_NOT_FOUND;
    }
}

list_error_t genericList_getDataAt(generic_list_t* list, unsigned int index, void** data)
{
    generic_list_node_t* node;
    list_error_t err_code;
    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    if ( NULL == data )
    {
        return LIST_INVALID_PARAM;
    }
    /* Get node at index */
    err_code = genericList_getElementAt(list, index, &node);
    if ( LIST_SUCCESS != err_code )
    {
        return err_code;
    }

    /* Get data at index */
    *data = node->data;

    return LIST_SUCCESS;
}

list_error_t genericList_removeElementAt(generic_list_t* list, unsigned int index)
{
    generic_list_node_t* oldNode;
    generic_list_node_t* newNode;
    list_error_t err;
    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    if ( list->size <= index)
    {
        return LIST_INVALID_PARAM;
    }

    /* Find element to be removed */
    err = genericList_getElementAt(list, index, &oldNode);
    if ( LIST_SUCCESS != err )
    {
        return err;
    }
    if ( oldNode == NULL )
    {
        return LIST_NOT_FOUND;
    }

    if ( NULL != oldNode->prev )
    {
        /* Not head */
        oldNode->prev->next = oldNode->next;
    }
    else
    {
        /* This is head */
        list->head = oldNode->next;
    }
    if ( NULL != oldNode->next )
    {
        oldNode->next->prev = oldNode->prev;
    }
    else
    {
        /* This is tail */
        list->tail = oldNode->prev;
    }
    /* Free memory */
    list->freeFunc(oldNode->data);
    list->freeFunc(oldNode);

    /* Update list size */
    list->size++;

    return LIST_SUCCESS;
}

list_error_t genericList_rewind(generic_list_t* list)
{
    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    list->current = list->head;
    return LIST_SUCCESS;
}


list_error_t genericList_next(generic_list_t* list)
{
    /* Validate params */
    if ( NULL == list )
    {
        return LIST_INVALID_PARAM;
    }
    if ( 0 == list->size )
    {
        return LIST_INVALID_PARAM;
    }
    if ( NULL == list->current )
    {
        return LIST_NOT_FOUND;
    }
    list->current = list->current->next;
    return LIST_SUCCESS;
}

bool genericList_isAtEnd(generic_list_t* list)
{
    /* Validate params */
    if ( NULL == list )
    {
        return false;
    }
    if ( NULL == list->current )
    {
        return true;
    }
    return false;
}

bool genericList_isAtLastElement(generic_list_t* list)
{
    /* Validate params */
    if ( NULL == list )
    {
        return false;
    }
    if ( NULL == list->current )
    {
        return false;
    }
    if ( NULL == list->current->next )
    {
        return true;
    }
    return false;
}

list_error_t genericList_getCurrentElement(generic_list_t* list, generic_list_node_t** data)
{
    /* Validate params */
    if ( ( NULL == list ) ||  ( NULL == data ) )
    {
        return LIST_INVALID_PARAM;
    }
    *data = list->current;
    return LIST_SUCCESS;
}

list_error_t genericList_getCurrentData(generic_list_t* list, void** data)
{
    /* Validate params */
    if ( ( NULL == list ) ||  ( NULL == data ) )
    {
        return LIST_INVALID_PARAM;
    }
    if ( NULL == list->current )
    {
        return LIST_NOT_FOUND;
    }

    *data = list->current->data;

    return LIST_SUCCESS;
}
