#define MU_IO		(*(volatile unsigned int *)0x3f215040)
#define MU_LSR		(*(volatile unsigned int *)0x3f215054)
#define MU_LSR_TX_IDLE	(1U << 6)
#define MU_LSR_TX_EMPTY	(1U << 5)
#define MU_LSR_RX_RDY	(1U << 0)
#define ENTRY_ADDRESS	0x100000

typedef int (*functype)(void);

void printf(char *str){
	int i;
	for(i=0;str[i]!='\0';i++){
		while (!(MU_LSR & MU_LSR_TX_IDLE) && !(MU_LSR & MU_LSR_TX_EMPTY));
		MU_IO = (unsigned int)str[i];
	}
}

int pow(int a, int x){
	int i,ans=1;
	for(i=0;i<x;i++){
		ans*=a;
	}
	return ans;
}

char getchar(){
	volatile char ch;
	while (!(MU_LSR & MU_LSR_RX_RDY));
	ch = (unsigned char)MU_IO;
	return ch;
}

int getpsize(){
	// get program size
	// format: [digits][data]
	// eg: 3124 -> 124:int

	volatile char ch;
	int i,psize;

	ch = getchar();

	for(i=ch-'0'-1;i>=0;i--){
		psize += (getchar() - '0') * pow(10, i);
	}

	return psize;
}

int main(void)
{
	volatile char data, ch;
	volatile unsigned char * p = (volatile unsigned char *)ENTRY_ADDRESS;
	int i,psize;

	psize = getpsize();

	for(i=0;i<psize;i++) {
		while (!(MU_LSR & MU_LSR_RX_RDY));
		data = (unsigned char)MU_IO;
		*(p++) = (unsigned char)data; // write 1 byte to memory
	}

	printf("A");

	functype f = (functype)ENTRY_ADDRESS;
	f();

	return 0;
}
