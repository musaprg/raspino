#define MU_IO		(*(volatile unsigned int *)0x3f215040)
#define MU_LSR		(*(volatile unsigned int *)0x3f215054)
#define MU_LSR_TX_IDLE	(1U << 6)
#define MU_LSR_TX_EMPTY	(1U << 5)
#define MU_LSR_RX_RDY	(1U << 0)
#define ENTRY_ADDRESS	0x90000

int main(void)
{
	volatile char data;
	volatile unsigned char * p = (volatile unsigned char *)ENTRY_ADDRESS;

	while (1) {
		while (!(MU_LSR & MU_LSR_RX_RDY));
		data = (unsigned char)MU_IO;

		if (data == 0x1a) break; // 0x1a : EOF

		*(p++) = (unsigned char)data; // write 1 byte to memory

	}

	//while (!(MU_LSR & MU_LSR_TX_IDLE) && !(MU_LSR & MU_LSR_TX_EMPTY));
	//MU_IO = (unsigned int)'A';

	typedef int (*functype)(void);

	functype f = (functype)ENTRY_ADDRESS;
	f();

	return 0;
}
