ub-osal-objs := $(ub-osal-dir)/none/mem.o
ub-osal-objs += $(ub-osal-dir)/none/mutex.o
ub-osal-objs += $(ub-osal-dir)/none/sem.o
ub-osal-objs += $(ub-osal-dir)/none/sysio.o

ub-tmp-objs += $(ub-osal-objs)
ub-objs += $(ub-osal-build)/*.o
ub-builds += $(ub-osal-build)

$(ub-osal-build): $(ub-osal-objs)
	@echo "[UB] Creating osal/none."
	mkdir -p $@
	mv $(ub-osal-objs) $(ub-osal-build)/
