#ifndef UB_MOM_LOADER_H
#define UB_MOM_LOADER_H

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

typedef void* UBload;

struct UBloadVt {
	size_t (*size)(void);
	UBload (*ctor)(UBload, va_list);
	void (*eat)(void*, UBload);
};

typedef struct UBloader UBloader;

UBloader* ub_loader_create(const struct UBloadVt* vp);

void ub_loader_destroy(UBloader* loader);

UBload ub_load_create(UBloader* loader, va_list args);

void ub_load_destroy(UBload load, UBloader* loader);

#endif /* UB_MOM_LOADER_H */
