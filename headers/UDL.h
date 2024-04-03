#ifndef UDL_H
#define UDL_H

#define CHECK_PTR(ptr)           \
    do                           \
    {                            \
    if (ptr == NULL)             \
        return NULL_INSTEAD_PTR; \
    }while(0)                    \

#define CALLOC(elem, type, size)                \
    do                                          \
    {                                           \
    void* clTmp_ = calloc(size, sizeof(type));  \
    if (clTmp_ == NULL)                         \
        return CALLOC_ERROR;                    \
    elem = (type*)clTmp_;                       \
    }while(0)


#define REALLOC(elem, type, size)                                 \
    do                                                            \
    {                                                             \
        void* clTmp_ = realloc(stk->data, size * sizeof(elem_t)); \
        if(clTmp_ == NULL)                                        \
            return REALLOC_ERROR;                                 \
        stk->data = (type*)clTmp_;                                \
    }while(0)

#define FOPEN(var, name, mode)             \
    FILE* var = fopen(name, mode);         \
        if(var == NULL)                    \
            return FOPEN_ERROR;


#endif //UDL_H
