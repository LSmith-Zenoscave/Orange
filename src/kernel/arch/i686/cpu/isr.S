
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : isr.S
	#      Purpose : ISR macros and common routines.
	# ==========================================================================

.section .text
.align 4

.macro ISR_NOERR idx
	.global _isr\idx
	_isr\idx:
		cli
		push $0
		push $\idx
		jmp isr_common
.endm

.macro ISR_ERR idx
	.global _isr\idx
	_isr\idx:
		cli
		push $\idx
		jmp isr_common
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
ISR_NOERR 127

.extern fault_handler
.type fault_handler, @function

isr_common:
	pusha

	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	push %esp
	call fault_handler
	add $4, %esp

	pop %gs
	pop %fs
	pop %es
	pop %ds

	popa
	add $8, %esp
	iret
