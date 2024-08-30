global set_gdt

set_gdt:
    ; load gdt
    MOV eax, [esp+4]
    LGDT [eax]

    ; update segment registers to point towards gdt
    MOV eax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax
    ; we can't move a value into the code segment, so we long jump to it
    JMP 0x08:.set_code_segment
    
.set_code_segment:
    RET

global set_ltr
set_ltr:
    MOV ax, 0x2B
    LTR ax
    RET
