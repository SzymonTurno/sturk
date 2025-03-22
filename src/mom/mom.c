#include "defs.h"
#include "ub/mom/scribe.h"
#include "ub/os/mem.h"
#include <string.h>

UBloader* ub_loader_create(const struct UBloadVt* vp)
{
	UBloader* self = ub_malloc(sizeof(*self));

	self->vp = vp;
	self->pool = vp->size() ? ub_pool_create(vp->size()) : NULL;
	return self;
}

void ub_loader_destroy(UBloader* loader)
{
	if (loader->pool)
		ub_pool_destroy(loader->pool);
	loader->pool = NULL;
	loader->vp = NULL;
	ub_free(loader);
}

UBload ub_load_create(UBloader* loader, va_list args)
{
	UBload self = loader->pool ? ub_pool_alloc(loader->pool) : NULL;

	return loader->vp->ctor(self, args);
}

void ub_load_destroy(UBload load, UBloader* loader)
{
	if (loader->pool)
		ub_pool_free(loader->pool, load);
}

UBchan* ub_chan_create(UBloader* loader)
{
	UBchan* self = ub_malloc(sizeof(*self));

	self->loader = loader;
	self->list = NULL;
	return self;
}

void ub_chan_destroy(UBchan* chan)
{
	while (chan->list)
		ub_free(ub_list_rem(&chan->list));
	chan->loader = NULL;
	ub_free(chan);
}

void ub_chan_attach(UBchan* chan, void* eater, unsigned prio)
{
	struct UBeaterList* entry = ub_malloc(sizeof(*entry));

	*ub_list_cast(entry) = eater;
	chan->list = ub_list_ins(chan->list, entry, prio);
}

void ub_chan_detach(UBchan* chan, void* eater)
{
	for (UB_LIST_ITER(i, &chan->list))
		if (*ub_list_data(*i) == eater) {
			ub_free(ub_list_rem(i));
			break;
		}
}

void ub_chan_upload(UBchan* chan, UBload load)
{
	void (*eat)(void*, UBload) = chan->loader->vp->eat;

	for (UB_LIST_ITER(i, &chan->list))
		eat(*ub_list_data(*i), load);
}

UBscriber* ub_scriber_create(void* eater, size_t size)
{
	UBscriber* self = ub_malloc(sizeof(*self));

	self->eater = ub_malloc(size);
	memcpy(self->eater, eater, size);
	self->list = NULL;
	return self;
}

void ub_scriber_destroy(UBscriber* scriber)
{
	while (scriber->list) {
		ub_chan_detach(*ub_list_data(scriber->list), scriber->eater);
		ub_free(ub_list_rem(&scriber->list));
	}
	ub_free(scriber->eater);
	scriber->eater = NULL;
	ub_free(scriber);
}

void ub_scribe(UBscriber* scriber, UBchan* chan, unsigned prio)
{
	struct ChanList* entry = ub_malloc(sizeof(*entry));

	*ub_list_data(entry) = chan;
	scriber->list = ub_list_ins(scriber->list, entry);
	ub_chan_attach(chan, scriber->eater, prio);
}

struct UBmess ub_mess_create(UBchan* chan, ...)
{
	struct UBmess ret = {0};
	va_list args;

	va_start(args, chan);
	ret.chan = chan;
	ret.load = ub_load_create(chan->loader, args);
	va_end(args);
	return ret;
}

void ub_lish(struct UBmess msg)
{
	ub_chan_upload(msg.chan, msg.load);
}

UBus* ub_bus_create(void)
{
	UBus* self = ub_malloc(sizeof(*self));

	self->pool = ub_pool_create(sizeof(struct MessQ));
	self->mut = ub_mutex_create(UB_MUTEX_PROTOCOL_PRIO_INHERIT);
	self->sem = ub_sem_create(0);
	self->q = NULL;
	return self;
}

void ub_bus_destroy(UBus* bus)
{
	ub_mutex_lock(bus->mut);
	while (bus->q)
		ub_free(ub_cirq_rem(&bus->q));
	ub_mutex_unlock(bus->mut);
	ub_sem_destroy(bus->sem);
	bus->sem = NULL;
	ub_mutex_destroy(bus->mut);
	bus->mut = NULL;
	ub_pool_destroy(bus->pool);
	bus->pool = NULL;
	ub_free(bus);
}

void ub_bus_ins(UBus* bus, struct UBmess msg)
{
	struct MessQ* entry = ub_pool_alloc(bus->pool);

	*ub_cirq_data(entry) = msg;
	ub_mutex_lock(bus->mut);
	bus->q = ub_cirq_ins(bus->q, entry);
	ub_sem_post(bus->sem);
	ub_mutex_unlock(bus->mut);
}

struct UBmess ub_bus_rem(UBus* bus)
{
	struct MessQ* entry = NULL;
	struct UBmess ret = {0};

	ub_sem_wait(bus->sem);
	ub_mutex_lock(bus->mut);
	entry = ub_cirq_rem(&bus->q);
	ub_mutex_unlock(bus->mut);
	ret = *ub_cirq_data(entry);
	ub_pool_free(bus->pool, entry);
	return ret;
}
