.global set_gdtr
.global set_ltr

set_gdtr:
  # load gdt
  movl 4(%esp), %eax
  lgdt (%eax)

  # update segment registers to point towards gdt
  movw $0x10, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss
  # we can't move a value into the code segment, so we long jump to it
  jmp $0x08, $set_code_segment

set_code_segment:
  ret
  
set_ltr:
  movw $0x2B, %ax
  ltr %ax
  ret
  