
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : crti.S
	#      Purpose : C Runtime initializer.
	# ==========================================================================

.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp

.section .fini
.global _fini
.type .fini, @function
_fini:
    push %ebp
    movl %esp, %ebp
