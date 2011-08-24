#ifndef _LINUX_VMALLOC_H
#define _LINUX_VMALLOC_H

#include <linux/spinlock.h>
#include <linux/init.h>
#include <asm/page.h>		/* pgprot_t */

struct vm_area_struct;		/* vma defining user mapping in mm_types.h */

/* bits in flags of vmalloc's vm_struct below */
#define VM_IOREMAP	0x00000001	/* ioremap() and friends */
#define VM_ALLOC	0x00000002	/* vmalloc() */
#define VM_MAP		0x00000004	/* vmap()ed pages */
#define VM_USERMAP	0x00000008	/* suitable for remap_vmalloc_range */
#define VM_VPAGES	0x00000010	/* buffer for pages was vmalloc'ed */

#if defined(CONFIG_MODULES) && defined(CONFIG_X86) && defined(CONFIG_PAX_KERNEXEC)
#define VM_KERNEXEC	0x00000020	/* allocate from executable kernel memory range */
#endif

/* bits [20..32] reserved for arch specific ioremap internals */

/*
 * Maximum alignment for ioremap() regions.
 * Can be overriden by arch-specific value.
 */
#ifndef IOREMAP_MAX_ORDER
#define IOREMAP_MAX_ORDER	(7 + PAGE_SHIFT)	/* 128 pages */
#endif

struct vm_struct {
	struct vm_struct	*next;
	void			*addr;
	unsigned long		size;
	unsigned long		flags;
	struct page		**pages;
	unsigned int		nr_pages;
	phys_addr_t		phys_addr;
	void			*caller;
};

/*
 *	Highlevel APIs for driver use
 */
extern void vm_unmap_ram(const void *mem, unsigned int count);
extern void *vm_map_ram(struct page **pages, unsigned int count,
				int node, pgprot_t prot);
extern void vm_unmap_aliases(void);

#ifdef CONFIG_MMU
extern void __init vmalloc_init(void);
#else
static inline void vmalloc_init(void)
{
}
#endif

extern void *vmalloc(unsigned long size);
extern void *vzalloc(unsigned long size);
extern void *vmalloc_user(unsigned long size);
extern void *vmalloc_node(unsigned long size, int node);
extern void *vzalloc_node(unsigned long size, int node);
extern void *vmalloc_exec(unsigned long size);
extern void *vmalloc_32(unsigned long size);
extern void *vmalloc_32_user(unsigned long size);
extern void *__vmalloc(unsigned long size, gfp_t gfp_mask, pgprot_t prot);
extern void *__vmalloc_node_range(unsigned long size, unsigned long align,
			unsigned long start, unsigned long end, gfp_t gfp_mask,
			pgprot_t prot, int node, void *caller);
extern void vfree(const void *addr);

extern void *vmap(struct page **pages, unsigned int count,
			unsigned long flags, pgprot_t prot);
extern void vunmap(const void *addr);

extern int remap_vmalloc_range(struct vm_area_struct *vma, void *addr,
							unsigned long pgoff);
void vmalloc_sync_all(void);
 
/*
 *	Lowlevel-APIs (not for driver use!)
 */

static inline size_t get_vm_area_size(const struct vm_struct *area)
{
	/* return actual size without guard page */
	return area->size - PAGE_SIZE;
}

extern struct vm_struct *get_vm_area(unsigned long size, unsigned long flags);
extern struct vm_struct *get_vm_area_caller(unsigned long size,
					unsigned long flags, void *caller);
extern struct vm_struct *__get_vm_area(unsigned long size, unsigned long flags,
					unsigned long start, unsigned long end);
extern struct vm_struct *__get_vm_area_caller(unsigned long size,
					unsigned long flags,
					unsigned long start, unsigned long end,
					void *caller);
extern struct vm_struct *remove_vm_area(const void *addr);

extern int map_vm_area(struct vm_struct *area, pgprot_t prot,
			struct page ***pages);
#ifdef CONFIG_MMU
extern int map_kernel_range_noflush(unsigned long start, unsigned long size,
				    pgprot_t prot, struct page **pages);
extern void unmap_kernel_range_noflush(unsigned long addr, unsigned long size);
extern void unmap_kernel_range(unsigned long addr, unsigned long size);
#else
static inline int
map_kernel_range_noflush(unsigned long start, unsigned long size,
			pgprot_t prot, struct page **pages)
{
	return size >> PAGE_SHIFT;
}
static inline void
unmap_kernel_range_noflush(unsigned long addr, unsigned long size)
{
}
static inline void
unmap_kernel_range(unsigned long addr, unsigned long size)
{
}
#endif

/* Allocate/destroy a 'vmalloc' VM area. */
extern struct vm_struct *alloc_vm_area(size_t size);
extern void free_vm_area(struct vm_struct *area);

/* for /dev/kmem */
extern long vread(char *buf, char *addr, unsigned long count);
extern long vwrite(char *buf, char *addr, unsigned long count);

/*
 *	Internals.  Dont't use..
 */
extern rwlock_t vmlist_lock;
extern struct vm_struct *vmlist;
extern __init void vm_area_register_early(struct vm_struct *vm, size_t align);

#ifdef CONFIG_SMP
# ifdef CONFIG_MMU
struct vm_struct **pcpu_get_vm_areas(const unsigned long *offsets,
				     const size_t *sizes, int nr_vms,
				     size_t align);

void pcpu_free_vm_areas(struct vm_struct **vms, int nr_vms);
# else
static inline struct vm_struct **
pcpu_get_vm_areas(const unsigned long *offsets,
		const size_t *sizes, int nr_vms,
		size_t align)
{
	return NULL;
}

static inline void
pcpu_free_vm_areas(struct vm_struct **vms, int nr_vms)
{
}
# endif
#endif

#define vmalloc(x)						\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc size overflow\n"))	\
		___retval = NULL;				\
	else							\
		___retval = vmalloc((unsigned long)___x);	\
	___retval;						\
})

#define vzalloc(x)						\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vzalloc size overflow\n"))	\
		___retval = NULL;				\
	else							\
		___retval = vzalloc((unsigned long)___x);	\
	___retval;						\
})

#define __vmalloc(x, y, z)					\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "__vmalloc size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = __vmalloc((unsigned long)___x, (y), (z));\
	___retval;						\
})

#define vmalloc_user(x)						\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc_user size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vmalloc_user((unsigned long)___x);	\
	___retval;						\
})

#define vmalloc_exec(x)						\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc_exec size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vmalloc_exec((unsigned long)___x);	\
	___retval;						\
})

#define vmalloc_node(x, y)					\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc_node size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vmalloc_node((unsigned long)___x, (y));\
	___retval;						\
})

#define vzalloc_node(x, y)					\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vzalloc_node size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vzalloc_node((unsigned long)___x, (y));\
	___retval;						\
})

#define vmalloc_32(x)						\
({								\
	void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc_32 size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vmalloc_32((unsigned long)___x);	\
	___retval;						\
})

#define vmalloc_32_user(x)					\
({								\
void *___retval;					\
	intoverflow_t ___x = (intoverflow_t)x;			\
	if (WARN(___x > ULONG_MAX, "vmalloc_32_user size overflow\n"))\
		___retval = NULL;				\
	else							\
		___retval = vmalloc_32_user((unsigned long)___x);\
	___retval;						\
})

#endif /* _LINUX_VMALLOC_H */
