#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#include<time.h>


typedef struct LNode {
    int32_t val;
    struct LNode* prev;
    struct LNode* next;
} LNode_t;

LNode_t* createNode(int32_t val)
{
    LNode_t* node = (LNode_t*)malloc(sizeof(LNode_t));
    if(node == NULL) {
        printf("Memory allocation error! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    node->val = val;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

LNode_t* cutNode(LNode_t* head, size_t index)
{
    LNode_t* curr = head;
    bool is_head = index == 0;

    while(index > 0 && curr != NULL) {
        curr = curr->next;
        index--;
    }

    if(curr == NULL) {
        printf("Ya done messed up\n");
        exit(EXIT_FAILURE);
    }

    LNode_t* prev = (LNode_t*)curr->prev;
    LNode_t* next = (LNode_t*)curr->next;
    free(curr);
    
    if(prev != NULL) {
        prev->next = (struct LNode*)next;
    }
    if(next != NULL) {
        next->prev = (struct LNode*)prev;
    }

    return is_head ? next : head;
}

LNode_t* createLL(size_t size)
{
    LNode_t* head = createNode(0);
    LNode_t* curr = head;
    LNode_t* tmp;

    for(int i = 1; i < size; i++) {
        tmp = createNode(i);
        curr->next = tmp;
        tmp->prev = curr;
        curr = tmp;
    }

    return head;
}

void traverse_and_print(LNode_t* head)
{
    LNode_t* curr = head;
    size_t index = 0; 

    while(curr != NULL) {
        printf("(%zu) Val: %d\n", index, curr->val);
        curr = curr->next;
        index++;
    }
}

typedef struct lenArray {
    int32_t* arr;
    size_t len;
    size_t capacity;
} lenArray_t;

void lenArray_init(lenArray_t* arr, size_t size)
{
    if(arr == NULL) {
        printf("Invalid address for arr passed in!\n");
        return;
    }

    arr->arr = (int32_t*)malloc(size * sizeof(int32_t));
    if(arr->arr == NULL) {
        arr->len = arr->capacity = 0;
        printf("Allocation error!\n");
        return;
    } else {
        arr->len = arr->capacity = size;
        for(int i = 0; i < arr->len; i++) {
            arr->arr[i] = i;
        }
    }
}

void cut_index(lenArray_t* arr, size_t index)
{
    if(arr == NULL) {
        printf("Invalid address for arr passed in!\n");
        return;
    }

    assert(index < arr->capacity);
    
    memmove(&(arr->arr[index]), &(arr->arr[index + 1]), (arr->len - index - 1) * sizeof(int32_t)); 

    arr->len--;
}

void print_arr(lenArray_t arr)
{
    for(int i = 0; i < arr.len; i++) {
        printf("(%d) %d\n", i, arr.arr[i]);
    }
}


int main(int argc, char** argv)
{
    srand(time(NULL));

    size_t size;
    if(argc < 2) {
        size = 100;
    } else {
        size = atoi(argv[1]);
        // could put some additional checks here
        // but we're just doing a basic test, 
        // no need to over do it
    }

    printf("Size: %zu\n", size);


    int n_removes = size * 0.1;
    size_t* remove_idxs = (size_t*)malloc(n_removes * sizeof(size_t));
    if(remove_idxs == NULL) {
        printf("Sadness\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < n_removes; i++) {
        remove_idxs[i] = rand() % (size - i);
    }

    LNode_t* head = createLL(size);
    
    lenArray_t arr;
    lenArray_init(&arr, size);
    
    clock_t LL_start = clock();
    for(int i = 0; i < n_removes; i++) {
        head = cutNode(head, remove_idxs[i]);
    }
    clock_t LL_stop = clock();

    clock_t arr_start = clock();
    for(int i = 0; i < n_removes; i++) {
        cut_index(&arr, remove_idxs[i]);
    }
    clock_t arr_stop = clock();

    printf("\tLL time: %ju\n", (uintmax_t)(LL_stop - LL_start));
    printf("\tArr time: %ju\n", (uintmax_t)(arr_stop - arr_start));
    
    return 0;
}
