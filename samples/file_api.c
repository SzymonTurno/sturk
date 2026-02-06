#include "sample.h"
#include "sturk/debug.h"
#include "sturk/os/mem.h"

struct IoFile {
	struct StIo vt;
	FILE* fp;
};

static void file_putc(struct StIo* vp, char c)
{
	struct IoFile* self = (struct IoFile*)vp;

	putc(c, self->fp);
}

static char file_getc(struct StIo* vp)
{
	struct IoFile* self = (struct IoFile*)vp;

	return (char)getc(self->fp);
}

struct StIo* iofile_create(FILE* fp)
{
	struct IoFile* file = ST_NEW(struct IoFile);
	struct StIo* ret = (struct StIo*)file;

	/* Make sure that `vt` is the first member. */
	ASSERT(ret == &file->vt);
	ret->putc_cb = file_putc;
	ret->getc_cb = file_getc;
	file->fp = fp;
	return ret;
}
