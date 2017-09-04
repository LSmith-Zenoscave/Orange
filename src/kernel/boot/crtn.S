
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : crtn.S
	#      Purpose : C Runtime finalizer.
	# ==========================================================================

.section .init
    popl %ebp
    ret

.section .fini
    popl %ebp
    ret
