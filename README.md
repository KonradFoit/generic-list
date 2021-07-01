# generic-list
Generic two-way list module in C

## Build
Simply add `generic_list.h` to your seatch path and `generic_list.c` to compiled sources

## Notes
Data pointer passed as an element to add to the list will be passed to `freeFunc` inside of `genericList_freeList`. Therefore it is advised that all data put into the list will be either dynamically allocated or all data passed into the list will be statically allocated with `freeFunc` set to empty function.

## Examples
### Create and add to list
```
generic_list_t list;
list_error_t err;
void* data1, data2;
err =  genericList_newList(&list, free, malloc);
assert(err == LIST_SUCCESS);
err =  genericList_append(&list, data1);
assert(err == LIST_SUCCESS);
err =  genericList_append(&list, data2);
assert(err == LIST_SUCCESS);
// Do stuff
err = genericList_freeList(&list);
assert(err == LIST_SUCCESS);
```
### Insert element at position
```
generic_list_t list;
void* data1, data2, data3;
list_error_t err;
assert(err == LIST_SUCCESS);
err =  genericList_append(&list, data1);
assert(err == LIST_SUCCESS);
err =  genericList_append(&list, data3);
assert(err == LIST_SUCCESS);
err = genericList_insert(&list, data2, 1);
assert(err == LIST_SUCCESS);
// Do stuff
err = genericList_freeList(&list);
assert(err == LIST_SUCCESS);
```
### Iterate through list
```
list_error_t err;
err = genericList_rewind(&list);
assert(err == LIST_SUCCESS);
while ( !genericList_isAtEnd(&list) )
{
    /* Get data */
    void* data
    err = genericList_getCurrentData(&list, &data);
    assert(err == LIST_SUCCESS);
    /* Do something with data */
    genericList_next(&list);
}
```

## License:
MIT License
https://opensource.org/licenses/MIT
