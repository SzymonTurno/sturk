#include "ub/logger/snapshot.h"
#include "UB/logger/log.h"
#include "UB/arith.h"
#include "UB/dict.h"
#include "UB/rbtree.h"
#include "ub/os/mem.h"
#include "types.h"
#include <string.h>

struct UBsnapshot {
	void* placeholder;
};

DICT(struct Tree, union { int n; void* align; });

static struct Tree* create_node(char* key)
{
	struct Tree* self = ub_malloc(sizeof(*self));

	dict_data(self)->n = 1;
	dict_setk(self, strcpy(ub_malloc(BUFF_MAX_SIZE), key));
	return self;
}

static int destroy_node(struct Tree* node)
{
	int ret = dict_data(node)->n;

	ub_free(dict_getk(node));
	dict_setk(node, NULL);
	ub_free(node);
	return ret;
}

static struct UBstrnode* strnode_cont(struct UBrbnode* ptr)
{
	return container_of(ptr, struct UBstrnode, node);
}

static int destroy_tree(struct Tree* root)
{
	int err = 0;

	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = rb_deepest(&dict_cast(root)->node);
		p = rb_parent(i);
		if (destroy_node(dict_cont(strnode_cont(i), struct Tree)))
			err = 1;

		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
	return err;
}

static int ordered_compare(UBfstream* a, UBfstream* b)
{
	int err = 0;
	char* abuff = ub_malloc(BUFF_MAX_SIZE);
	char* bbuff = ub_malloc(BUFF_MAX_SIZE);
	char* ares = NULL;
	char* bres = NULL;

	do {
		ares = ub_fgets(abuff, BUFF_MAX_SIZE, a);
		bres = ub_fgets(bbuff, BUFF_MAX_SIZE, b);
		if (!ares != !bres)
			err = 1;
		else if (ares)
			err = strcmp(ares, bres);
	} while (err == 0 && ares);
	ub_free(bbuff);
	ub_free(abuff);
	return err;
}

static int unordered_compare(UBfstream* a, UBfstream* b)
{
	struct Tree* root = NULL;
	struct Tree* tmp = NULL;
	char* buff = ub_malloc(BUFF_MAX_SIZE);
	int err = 0;

	while (ub_fgets(buff, BUFF_MAX_SIZE, a)) {
		tmp = dict_find(root, buff);
		if (tmp)
			++dict_data(tmp)->n;
		else
			root = dict_ins(root, create_node(buff));
	}

	while (ub_fgets(buff, BUFF_MAX_SIZE, b)) {
		tmp = dict_find(root, buff);
		if (tmp) {
			--dict_data(tmp)->n;
		} else {
			err = 1;
		}
	};
	ub_free(buff);
	return destroy_tree(root) ? 1 : err;
}

UBsnapshot* ub_snapshot_open(const char* factual)
{
	return (UBsnapshot*)ub_fopen(factual, "w+");
}

UBfstream* ub_snapshot_cast(UBsnapshot* snapshot)
{
	return (UBfstream*)snapshot;
}

static void copy_file(UBfstream* dest, UBfstream* src)
{
	char* buff = ub_malloc(BUFF_MAX_SIZE);

	while (ub_fgets(buff, BUFF_MAX_SIZE, src))
		ub_fputs(buff, dest);
	ub_free(buff);
}

static int eval(UBsnapshot* snapshot, const char* fexpected,
	int (*cb)(UBfstream*, UBfstream*))
{
	UBfstream* actual = (UBfstream*)snapshot;
	UBfstream* expected = NULL;
	int err = 1;

	ub_fseekset(actual, 0);
	if (ub_fexists(fexpected)) {
		expected = ub_fopen(fexpected, "r");
		err = cb(actual, expected);
	} else {
		expected = ub_fopen(fexpected, "w");
		copy_file(expected, actual);
		LOG(WARNING, "ub-logger", "Created new file: %s.", fexpected);
	}
	ub_fclose(expected);
	return err;
}

int ub_snapshot_ordered(UBsnapshot* snapshot, const char* fexpected)
{
	return eval(snapshot, fexpected, ordered_compare);
}

int ub_snapshot_unordered(UBsnapshot* snapshot, const char* fexpected)
{
	return eval(snapshot, fexpected, unordered_compare);
}

void ub_snapshot_close(UBsnapshot* snapshot)
{
	ub_fclose((UBfstream*)snapshot);
}
