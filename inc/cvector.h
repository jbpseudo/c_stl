#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CVECTOR_INIT_CAP        32
#define CVECTOR_GROWTH_FACTOR   1.6

#define CVECTOR_DECLARE(T)\
\
typedef struct T##Vector {\
    size_t len;\
    size_t cap;\
    T    *elem;\
    T    *end;\
} T##Vector;\
\
int T##Vector_init(T##Vector *vec);\
void T##Vector_free(T##Vector *vec);\
\
T *T##Vector_get(T##Vector *vec, size_t i);\
T *T##Vector_set(T##Vector *vec, size_t i, T el);\
T *T##Vector_pop(T##Vector *vec);\
int T##Vector_del(T##Vector *vec, size_t i, T *ret);\
T *T##Vector_ins(T##Vector *vec, size_t i, T el);\
\
T *T##Vector_append(T##Vector *vec, T el);\
T *T##Vector_allocate(T##Vector *vec, size_t n);\
\
size_t T##Vector_find(T##Vector *vec, int (*pred)(T*, T*), T el);\
size_t T##Vector_rem(T##Vector *vec, int (*pred)(T*, T*), T el);\
\
int   T##Vector_foreach(\
        T##Vector *vec, int (*fun)(T, void *),\
        void *args);

#define CVECTOR_DEFINE(T)\
\
static int T##Vector_grow(T##Vector *vec){\
    size_t new_cap = vec->cap * CVECTOR_GROWTH_FACTOR;\
    T *new_elem = calloc(sizeof(*new_elem), new_cap + 1);\
    \
    if (new_elem == NULL){\
        return -1;\
    }\
    memcpy(new_elem, vec->elem, sizeof(vec->elem) * vec->cap);\
    free(vec->elem);\
    vec->elem = new_elem;\
    vec->cap = new_cap;\
    vec->end = &vec->elem[vec->cap];\
    return 0;\
};\
\
int T##Vector_init(T##Vector *vec){\
    vec->len = 0;\
    vec->cap = CVECTOR_INIT_CAP;\
    vec->end = NULL;\
    vec->elem = NULL;\
    \
    vec->elem = calloc(sizeof(*vec->elem), vec->cap + 1);\
    if (vec->elem == NULL){\
        return ENOMEM;\
    }\
    \
    vec->end = &vec->elem[vec->cap];\
\
    return 0;\
}\
\
void T##Vector_free(T##Vector *vec){\
    free(vec->elem);\
    vec->elem = NULL;\
    vec->cap = 0;\
    vec->len = 0;\
}\
\
T *T##Vector_get(T##Vector *vec, size_t i){\
    if (i >= vec->len){\
        errno = EFAULT;\
        return vec->end;\
    }\
    return &vec->elem[i];\
};\
\
T *T##Vector_set(T##Vector *vec, size_t i, T el){\
    if (i >= vec->len){\
        errno = EFAULT;\
        return vec->end;\
    }\
    \
    vec->elem[i] = el;\
    \
    return &vec->elem[i];\
};\
\
T *T##Vector_pop(T##Vector *vec){\
    if (vec->len == 0){\
        errno = EFAULT;\
        return vec->end;\
    }\
\
    vec->len--;\
    return &vec->elem[vec->len];\
}\
\
int T##Vector_del(T##Vector *vec, size_t i, T *ret){\
    if (i >= vec->len){\
        errno = EFAULT;\
        return -1;\
    }\
\
    if (ret != NULL){\
        *ret = vec->elem[i];\
    }\
\
    for (size_t j = i; j < vec->len - 1; j++){\
        vec->elem[j] = vec->elem[j + 1];\
    }\
    \
    vec->len--;\
    \
    return 0;\
}\
\
T *T##Vector_ins(T##Vector *vec, size_t i, T el){\
    if (i > vec->len){\
        errno = EFAULT;\
        return vec->end;\
    }\
    \
    if (vec->cap == vec->len){\
        int ret = T##Vector_grow(vec);\
        if (ret == -1){\
            return vec->end;\
        }\
    }\
    \
    for (size_t j = vec->len; j > i; j--){\
        vec->elem[j] = vec->elem[j - 1];\
    }\
    vec->elem[i] = el;\
    vec->len++;\
\
    return &vec->elem[i];\
}\
\
T *T##Vector_append(T##Vector *vec, T el)\
{\
    if (vec->cap == vec->len){\
        int ret = T##Vector_grow(vec);\
        if (ret == -1){\
            return vec->end;\
        }\
    }\
\
    vec->elem[vec->len] = el;\
    vec->len++;\
    return &vec->elem[vec->len - 1];\
}\
\
T *T##Vector_allocate(T##Vector *vec, size_t n){\
    T *ret;\
    while(vec->len + n > vec->cap){\
        T##Vector_grow(vec);\
    }\
    ret = &vec->elem[vec->len];\
    vec->len += n;\
    \
    return ret;\
}\
\
size_t T##Vector_find(T##Vector *vec, int (*pred)(T *a, T *b), T el)\
{\
    size_t ret = vec->len;\
    \
    for (size_t i = 0; i < vec->len; i++){\
        if (pred(&vec->elem[i], &el) == 0){\
            ret = i;\
            break;\
        }\
    }\
    return ret;\
}\
\
size_t T##Vector_rem(T##Vector *vec, int (*pred)(T *a, T *b), T el)\
{\
    size_t i = T##Vector_find(vec, pred, el);\
    if (i == vec->len){\
        return i;\
    }\
    T##Vector_del(vec, i, NULL);\
    \
    return i;\
    \
}\
\
int T##Vector_foreach(T##Vector *vec, int (*fun)(T, void *), void *args){\
    int ret = 0;\
    for (size_t i = 0; i < vec->len; i++){\
        ret = fun(vec->elem[i], args);\
        \
        if (ret != 0){ break; }\
    }\
    return ret;\
}\

#endif
