
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : idt.S
	#      Purpose : IDT Flush routine (calls lidt).
	# ==========================================================================

.global idt_flush

idt_flush:
  mov 4(%esp), %eax
  lidt (%eax)
  ret
