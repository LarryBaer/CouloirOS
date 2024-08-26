typedef struct {
    uint16_t limit_low;        // Lower 16 bits of the segment limit
    uint16_t base_low;         // Lower 16 bits of the base address
    uint8_t  base_middle;      // Next 8 bits of the base address
    uint8_t  access;           // Access flags (read/write, descriptor type)
    uint8_t  granularity;      // Granularity and upper 4 bits of the limit
    uint8_t  base_high;        // Upper 8 bits of the base address
} __attribute__((packed)) GDTEntry;

typedef struct {
    uint16_t limit; 
    uint32_t base;
} __attribute__((packed)) GDTPointer;

typedef struct {
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint32_t trap;
	uint32_t iomap_base;
}__attribute__((packed)) TSSEntry;

void init_gdt();