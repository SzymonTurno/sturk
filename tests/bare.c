#include "sturk/os/mem.h"
#include "sturk/os/sys.h"

int main()
{
	char* s = NEW(char, 128);

	strprint(s, "%s", "test");
	st_free(s);
	return 0;
}
