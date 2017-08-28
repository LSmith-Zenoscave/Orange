
	# ==========================================================================
	# Orange: 0.1.0
	#
	# Copyright (c) 2017 Luke Smith <lsmith@zenoscave.com>
	#
	# Please refer to LICENSE for copyright information
	#
	#      File    : tss.S
	#      Purpose : TSS Flush routine (calls ltr).
	# ==========================================================================

.section .text
.align 4

.global tss_flush
.type tss_flush, @function

tss_flush:
  mov $0x2B, %ax
  ltr %ax
  ret
