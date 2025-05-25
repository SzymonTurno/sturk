#ifndef UB_MULTISET_H
#define UB_MULTISET_H

typedef struct CyMultiset CyMultiset;

CyMultiset* cy_multiset_ins(CyMultiset* mset, const char* str);

CyMultiset* cy_multiset_rem(CyMultiset* mset, const char* str);

const char* cy_multiset_getk(CyMultiset* mset);

int cy_multiset_count(CyMultiset* mset);

CyMultiset* cy_multiset_find(CyMultiset* mset, const char* str);

CyMultiset* cy_multiset_next(CyMultiset* mset);

void cy_multiset_destroy(CyMultiset* mset);

#endif /* UB_MULTISET_H */
