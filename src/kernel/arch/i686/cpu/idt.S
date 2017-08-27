.global idt_flush

idt_flush:
  mov 4(%esp), %eax
  lidt (%eax)
  ret
