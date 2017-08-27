
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : gdt.S
	#      Purpose : GDT Flush routine (calls lgdt).
	# ==========================================================================

.global gdt_flush

gdt_flush:
  mov 4(%esp), %eax
  lgdt (%eax)

  mov $0x10, %ax
 	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %ss

  jmp $0x08, $.flush

.flush:
  ret
