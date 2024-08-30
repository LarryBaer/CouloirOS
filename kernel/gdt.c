#include <stdint.h>
#include <stddef.h> 
#include "./include/util.h"
#include "./include/gdt.h"
#include "../lib/include/string.h"

#define GDT_SIZE 6

extern void set_gdt(uint32_t);
extern void set_ltr();

GDTEntry gdt_entries[GDT_SIZE];
GDTPointer gdt;
TSSEntry tss_entry;

void insert_gdt_entry(int position, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity){
    gdt_entries[position].base_low = (base & 0xFFFF);
    gdt_entries[position].base_middle = (base >> 16) & 0xFF;
    gdt_entries[position].base_high = (base >> 24) & 0xFF;
    gdt_entries[position].limit_low = (limit & 0xFFFF);
    gdt_entries[position].granularity = (limit >> 16) & 0x0F;
    gdt_entries[position].granularity |= (granularity & 0xF0);
    gdt_entries[position].access = access;
}

void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);
    insert_gdt_entry(num, base, limit, 0xE9, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void init_gdt(){
    gdt.base = (uint32_t) &gdt_entries;
    gdt.limit = (sizeof(GDTEntry) * GDT_SIZE) - 1;

    insert_gdt_entry(0, 0, 0, 0, 0); // Null descriptor
    insert_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);	// Kernel mode code segment
	insert_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);	// Kernel mode data segment
    insert_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);	// User mode code segment
	insert_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);	// User mode data segment
    write_tss(5, 0x10, 0x0);
    set_gdt((uint32_t) &gdt);
    set_ltr();
}
