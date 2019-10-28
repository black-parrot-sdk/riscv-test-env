// See LICENSE for license details.

#ifndef _ENV_PHYSICAL_SINGLE_CORE_TIMER_H
#define _ENV_PHYSICAL_SINGLE_CORE_TIMER_H

#include "../p/riscv_test.h"

#define TIMER_INTERVAL 50
#define MTIME_BASE       0x0200BFF8
#define MTIMECMP_BASE    0x02004000
#define MTIMECMP_SHIFT   3  // 64 bits for rv32/64 

#undef EXTRA_INIT_TIMER
#define EXTRA_INIT_TIMER                                                \
        li a0, MIP_MTIP;                                                \
        csrs mie, a0;                                                   \
	csrr t0, mstatus;						\
	ori t0, t0, MSTATUS_MPIE;					\
       	csrw mstatus, t0;						\
	li t0, MTIME_BASE;  /* mtime address */		      	       	\
	li t1, MTIMECMP_BASE;			       			\
	csrr t2, mhartid;						\
	slli t2, t2, MTIMECMP_SHIFT;					\
	add t1, t1, t2;	 /* mtimecmp address */		         	\
	ld a0, 0(t0);  /* csrr a0, mtime */		       		\
	addi a0, a0, TIMER_INTERVAL;					\
	sd a0, 0(t1);  /* csrw mtimecmp, a0 */	       	  		\

#if SSTATUS_XS != 0x18000
# error
#endif
#define XS_SHIFT 15

#undef INTERRUPT_HANDLER
#define INTERRUPT_HANDLER                                               \
        slli t5, t5, 1;                                                 \
        srli t5, t5, 1;                                                 \
        add t5, t5, -IRQ_M_TIMER;                                       \
        bnez t5, other_exception; /* other interrups shouldn't happen */\
	li t0, MTIME_BASE;  /* mtime address */                         \
        li t1, MTIMECMP_BASE;                                           \
        csrr t2, mhartid;                                               \
        slli t2, t2, MTIMECMP_SHIFT;                                    \
        add t1, t1, t2;  /* mtimecmp address */                         \
        ld t5, 0(t0);  /* csrr t5, mtime */		      		\
        addi t5, t5, TIMER_INTERVAL;                                    \
	sd t5, 0(t1);  /* csrw mtimecmp, t5 */		      	      	\
        mret;                                                           \

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#undef EXTRA_DATA
#define EXTRA_DATA                                                      \
        .align 3;                                                       \
regspill:                                                               \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
        .dword 0xdeadbeefcafebabe;                                      \
evac:                                                                   \
        .skip 32768;                                                    \

#endif
