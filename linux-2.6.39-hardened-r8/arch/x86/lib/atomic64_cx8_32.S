/*
 * atomic64_t for 586+
 *
 * Copyright © 2010  Luca Barbieri
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/linkage.h>
#include <asm/alternative-asm.h>
#include <asm/dwarf2.h>

.macro SAVE reg
	pushl_cfi %\reg
	CFI_REL_OFFSET \reg, 0
.endm

.macro RESTORE reg
	popl_cfi %\reg
	CFI_RESTORE \reg
.endm

.macro read64 reg
	movl %ebx, %eax
	movl %ecx, %edx
/* we need LOCK_PREFIX since otherwise cmpxchg8b always does the write */
	LOCK_PREFIX
	cmpxchg8b (\reg)
.endm

ENTRY(atomic64_read_cx8)
	CFI_STARTPROC

	read64 %ecx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_read_cx8)

ENTRY(atomic64_read_unchecked_cx8)
	CFI_STARTPROC

	read64 %ecx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_read_unchecked_cx8)

ENTRY(atomic64_set_cx8)
	CFI_STARTPROC

1:
/* we don't need LOCK_PREFIX since aligned 64-bit writes
 * are atomic on 586 and newer */
	cmpxchg8b (%esi)
	jne 1b

	ret
	CFI_ENDPROC
ENDPROC(atomic64_set_cx8)

ENTRY(atomic64_set_unchecked_cx8)
	CFI_STARTPROC

1:
/* we don't need LOCK_PREFIX since aligned 64-bit writes
 * are atomic on 586 and newer */
	cmpxchg8b (%esi)
	jne 1b

	ret
	CFI_ENDPROC
ENDPROC(atomic64_set_unchecked_cx8)

ENTRY(atomic64_xchg_cx8)
	CFI_STARTPROC

	movl %ebx, %eax
	movl %ecx, %edx
1:
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

	ret
	CFI_ENDPROC
ENDPROC(atomic64_xchg_cx8)

.macro addsub_return func ins insc unchecked=""
ENTRY(atomic64_\func\()_return\unchecked\()_cx8)
	CFI_STARTPROC
	SAVE ebp
	SAVE ebx
	SAVE esi
	SAVE edi

	movl %eax, %esi
	movl %edx, %edi
	movl %ecx, %ebp

	read64 %ebp
1:
	movl %eax, %ebx
	movl %edx, %ecx
	\ins\()l %esi, %ebx
	\insc\()l %edi, %ecx

.ifb \unchecked
#ifdef CONFIG_PAX_REFCOUNT
	into
2:
	_ASM_EXTABLE(2b, 3f)
#endif
.endif

	LOCK_PREFIX
	cmpxchg8b (%ebp)
	jne 1b
	movl %ebx, %eax
	movl %ecx, %edx

.ifb \unchecked
#ifdef CONFIG_PAX_REFCOUNT
3:
#endif
.endif

	RESTORE edi
	RESTORE esi
	RESTORE ebx
	RESTORE ebp
	ret
	CFI_ENDPROC
ENDPROC(atomic64_\func\()_return\unchecked\()_cx8)
.endm

addsub_return add add adc
addsub_return sub sub sbb
addsub_return add add adc _unchecked
addsub_return sub sub sbb _unchecked

.macro incdec_return func ins insc unchecked
ENTRY(atomic64_\func\()_return\unchecked\()_cx8)
	CFI_STARTPROC
	SAVE ebx

	read64 %esi
1:
	movl %eax, %ebx
	movl %edx, %ecx
	\ins\()l $1, %ebx
	\insc\()l $0, %ecx

.ifb \unchecked
#ifdef CONFIG_PAX_REFCOUNT
	into
2:
	_ASM_EXTABLE(2b, 3f)
#endif
.endif

	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

	movl %ebx, %eax
	movl %ecx, %edx

.ifb \unchecked
#ifdef CONFIG_PAX_REFCOUNT
3:
#endif
.endif

	RESTORE ebx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_\func\()_return\unchecked\()_cx8)
.endm

incdec_return inc add adc
incdec_return dec sub sbb
incdec_return inc add adc _unchecked
incdec_return dec sub sbb _unchecked

ENTRY(atomic64_dec_if_positive_cx8)
	CFI_STARTPROC
	SAVE ebx

	read64 %esi
1:
	movl %eax, %ebx
	movl %edx, %ecx
	subl $1, %ebx
	sbb $0, %ecx

#ifdef CONFIG_PAX_REFCOUNT
	into
1234:
	_ASM_EXTABLE(1234b, 2f)
#endif

	js 2f
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

2:
	movl %ebx, %eax
	movl %ecx, %edx
	RESTORE ebx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_dec_if_positive_cx8)

ENTRY(atomic64_add_unless_cx8)
	CFI_STARTPROC
	SAVE ebp
	SAVE ebx
/* these just push these two parameters on the stack */
	SAVE edi
	SAVE esi

	movl %ecx, %ebp
	movl %eax, %esi
	movl %edx, %edi

	read64 %ebp
1:
	cmpl %eax, 0(%esp)
	je 4f
2:
	movl %eax, %ebx
	movl %edx, %ecx
	addl %esi, %ebx
	adcl %edi, %ecx

#ifdef CONFIG_PAX_REFCOUNT
	into
1234:
	_ASM_EXTABLE(1234b, 3f)
#endif

	LOCK_PREFIX
	cmpxchg8b (%ebp)
	jne 1b

	movl $1, %eax
3:
	addl $8, %esp
	CFI_ADJUST_CFA_OFFSET -8
	RESTORE ebx
	RESTORE ebp
	ret
4:
	cmpl %edx, 4(%esp)
	jne 2b
	xorl %eax, %eax
	jmp 3b
	CFI_ENDPROC
ENDPROC(atomic64_add_unless_cx8)

ENTRY(atomic64_inc_not_zero_cx8)
	CFI_STARTPROC
	SAVE ebx

	read64 %esi
1:
	testl %eax, %eax
	je 4f
2:
	movl %eax, %ebx
	movl %edx, %ecx
	addl $1, %ebx
	adcl $0, %ecx

#ifdef CONFIG_PAX_REFCOUNT
	into
1234:
	_ASM_EXTABLE(1234b, 3f)
#endif

	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

	movl $1, %eax
3:
	RESTORE ebx
	ret
4:
	testl %edx, %edx
	jne 2b
	jmp 3b
	CFI_ENDPROC
ENDPROC(atomic64_inc_not_zero_cx8)
