#ifdef DEBUG
    #define ON_DEBUG(...)  __VA_ARGS__ 

    #define PRP(x) \
    if (x == nullptr)\
        return FAIL;\
    \


#else
    #define ON_DEBUG(...)
    #define PRP(...)
#endif 

