/* program dumpt to read continuously the FIFO of the first mutrig available */
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<fcntl.h>
#include<pci/pci.h>
#include<stdlib.h>
#include<string.h>

#include"mutrig.h"
#define MUTADR(brd,aadr) (sysadr + aadr/8)

#define VID 0x1172   /* manufacturer ID of device (Altera) */
#define UID 0x4567   /* device ID, i.e. MUTRIG board */

#define BARSIZE 0x02000000

volatile unsigned int *sysadr=0;
struct pci_access *pca;
struct pci_dev    *pcd;
unsigned char trgcnt[128];
char *serr;
char tnb;
int nbr;
int fd;


unsigned long myclk()
{
    struct timeval tv;
    if (gettimeofday(&tv,NULL)) {perror("gettimeofday");return(0);}
    return((long)tv.tv_usec);
}

/* Find the first matching device and return a session to it (if slot=0) */
/* or return the matching device in the given slot */
int myFind(slot)
short slot;
{
    int l,nb,nd;
    char str[128];

    pci_scan_bus(pca);
    for (pcd = pca->devices; pcd; pcd = pcd->next)
    {
	pci_fill_info(pcd,PCI_FILL_IDENT | PCI_FILL_IRQ | PCI_FILL_BASES | PCI_FILL_SIZES );
	l = pci_read_byte(pcd,8);
	if ((pcd->vendor_id==VID)&&(pcd->device_id==UID))
	  printf("found a MUTRIG %04x:%04x at %02x:%02x.0 in slot %i \n",pcd->vendor_id,pcd->device_id,pcd->bus,pcd->dev,l);
    }

    for (pcd = pca->devices; pcd; pcd = pcd->next)
    {
	pci_fill_info(pcd,PCI_FILL_IDENT | PCI_FILL_IRQ | PCI_FILL_BASES | PCI_FILL_SIZES);
	l = pci_read_byte(pcd,8);
	//printf("%04x %04x %i %s \n",pcd->vendor_id,pcd->device_id,l,pcd->phy_slot);
	if ((pcd->vendor_id==VID)&&(pcd->device_id==UID)&&((slot==0)||(slot==l)))
	  {nb=pcd->bus;nd=pcd->dev;break;}
    }

    if (!pcd) {printf("No MUTRIG found\n");exit(-1);}
    //printf("%04x %04x %i %s \n",pcd->vendor_id,pcd->device_id,l,pcd->phy_slot);
    printf("BAR0 at %08x  ",pci_read_long(pcd,16));
    sprintf(str,"/sys/bus/pci/devices/0000:%02x:%02x.0/resource0",nb,nd);
    printf("%s\n",str);
    l = open(str,O_RDWR|O_SYNC);
    if (l==-1) {perror("open");exit(-1);}

    return l;
}

int cnt0(unsigned short ac)
{
	int i,b;
	b=0;
	for(i=0;i<16;i++) if (!((ac>>i)&0x1)) b++;
	return(b);
}

int rdblock(aadr, sz, buff)
unsigned long aadr,sz,buff[];
{
  int i;
	for (i=0;i<sz;i++) {buff[i]= *(sysadr+aadr/4);}
	//memcpy(buff,(unsigned long *)(sysadr+aadr),sz*4);
	//lseek(fd,0x1800,SEEK_SET); perror("lseek");
	//printf("%i\n",	pread(fd,buff,sz*4,aadr));perror("pread");
	return 0;
}

/* send a Read/Write command to the remote telescope interface */
/* the answer is sent back after the pending triggers and then can be read on STATD after a few us */
long CTsendCMD(int brd,int testmode,int tscope,int writemode,int addr,unsigned long cmd)
{
    long i,j,st,v,sm;
	st= *(MUTADR(brd,CTSTAT));
	if (TXFREE(st)<12) return (TXFREE(st));
	v= 0x1FB;  *(MUTADR(brd,CTFO))= v;                /* Header= 1FB */
	v= (testmode & 1) * 0x80 + tscope; sm= v;
	*(MUTADR(brd,CTFO))= v;                           /* testbit and telescope number */
	v= 0;  *(MUTADR(brd,CTFO))= v;                    /* 0 */
	v= 0xFF;if (writemode!=0) {v=0;} sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /* 0xFF for reading 0x00 for writing */
	v= (addr & 0xFF00) >>8 ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /* address high */
	v= addr & 0xFF ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /* address low */
	v= (cmd & 0xFF000000) >>24 ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /* data high */
	v= (cmd & 0xFF0000) >>16 ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /*  */
	v= (cmd & 0xFF00) >>8 ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /*  */
	v= cmd & 0xFF ; sm+= v;
	*(MUTADR(brd,CTFO))= v;                           /* data low */

	*(MUTADR(brd,CTFO))= (char) sm;                   /* frame checksum */
	v= 0x11C;  *(MUTADR(brd,CTFO))= v;                /* Tail= 11C */
	return (TXFREE(st));
}

/* send a fake trigger frame for loopback testing */
long CTsendFake(int brd,int testmode,int tscope,int ttype,int tcnt, char *tdate)
{
    long i,j,st,v,sm;
	st = *(MUTADR(brd,CTSTAT));
	if (TXFREE(st)<12) return (TXFREE(st));
	v= 0x1F7;  *(MUTADR(brd,CTFO))= v;                /* Header= 1F7 */
	v= (testmode & 1) * 0x80 + tscope; sm= v;
	*(MUTADR(brd,CTFO))= v;                           /* testbit and telescope number */
	v= ((ttype & 0xF) << 4) + (tcnt & 0xF); sm+=v;
	*(MUTADR(brd,CTFO))= v;                           /* trig type and count */
	for(i=0;i<7;i++)
	{
		v=tdate[i]; sm+=v;
		*(MUTADR(brd,CTFO))= v;                   /* trig date */
	}
	*(MUTADR(brd,CTFO))= (char) sm;                   /* frame checksum */
	//printf("sm= %02x\n",(char) sm);
	v= 0x11C;*(MUTADR(brd,CTFO))= v;                  /* Tail= 11C */
	return (TXFREE(st));
}

void CTsoft_trig(long brd,int tscope)
{
    int i,j;
    unsigned short a;
	printf("Sending a trigger softly to board %ld for telescope %i\n",brd,tscope);
	if (CTsendCMD(brd, 1, tscope, 1, CTCFG, RMSOFTRIG)<12) printf("Error TX FIFO is full\n"); /* Remote soft trig */
	usleep(100);
	if (CTsendCMD(brd, 1, tscope, 1, CTCFG, 0)<12) printf("Error TX FIFO is full\n"); /* Remote 0 */
	usleep(100);
}

long CTset_cfg(long brd, long cmd)
{
    long i;
	i= *(MUTADR(brd,CTCFG));
	i|= cmd;
	*(MUTADR(brd,CTCFG))= i;
	i= *(MUTADR(brd,CTCFG));
	return(i);
}

long CTunset_cfg(long brd, unsigned long cmd)
{
    long i;
	i= *(MUTADR(brd,CTCFG));
	if (!(i&cmd)) {printf("WARNING: bit(s) not set %08lx\n",cmd); return(i);}
	i|=cmd; i-=cmd;
	*(MUTADR(brd,CTCFG))= i;
	i= *(MUTADR(brd,CTCFG));
	return(i);
}

long CTget_reg(long brd, long reg)
{
    long i;
	i= *(MUTADR(brd,reg));
	return(i);
}

CTget_brd_status(char *n,int brd)
{
        char *an;
	int i;

	an = (char *) malloc(20);
	*n= (char) 0;

	i= *(MUTADR(brd,CTSTAT));

	if (i & NLOCK0) {strcat(n,"UNLK0 ");} else strcat(n,"LOCK0 ");
	if (i & NLOCK1) {strcat(n,"UNLK1 ");} else strcat(n,"LOCK1 ");
	if (i & NLOCK2) {strcat(n,"UNLK2 ");} else strcat(n,"LOCK2 ");
	if (i & NLOCK3) {strcat(n,"UNLK3 ");} else strcat(n,"LOCK3 ");
	if (i & BADCHKS) {strcat(n,"BADCKS ");}
        sprintf(an,"TXFREE=%d RXUSED=%d\n",TXFREE(i),RXUSED(i));
	strcat(n,an);

/*
	i = get_vme_adc(brd,STATUSR);
	if (i & RUNNING) {strcat(n," RUNG ");} else strcat(n," STPD ");
	if (i & EXTCLK) {strcat(n,"EXCK ");} else strcat(n,"INCK ");
	sprintf(an,"IK=%03i OK=%03i ",PLICLK(i)+1,PLOCLK(i)+1);
	strcat(n,an);
	if (i & PLLSTS) {strcat(n,"PLOK ");} else strcat(n,"UNLK ");
	if (i & MBUSY) {strcat(n,"FULL ");}
	 else if (i & MEMPTY) {strcat(n,"EMTY ");}
	  else {strcat(n,"DRDY ");}
	sprintf(an,"PAG%03x ",PAGCUR(i));
	strcat(n,an);
	//printf("%s\n",n);
	if (i & INIZING) printf("WARNING board in slot %2i INIZING\n",Barad[brd]>>3);

        i = get_vme_adc(brd,STATB);
        sprintf(an,"UNLKDET=%x",PUNLOCK(i));
        strcat(n,an);

	i = get_vme_adc(brd,COMMANDR);
	sprintf(an," CMDR=%08x",i);
	strcat(n,an);
*/
}


int main(argc,argv)
short argc;
char *argv[];
{
  unsigned long lval=0;
  int brd=0;
  int i,j,jj,nbtrig,nn;
  unsigned int	rval,tim,sm,atrig[3],buff32[0x200];
  unsigned long long	al,buff64[0x200];
  char *str;

	str=(char *)malloc(80);
	system("date");
	pca = pci_alloc();
	pci_init(pca);
	for(i=0;i<128;i++) trgcnt[i]=0;

	fd = myFind(0);
	sysadr = (volatile unsigned int *) mmap(NULL,BARSIZE,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if (sysadr==(unsigned int *)MAP_FAILED) {perror("mmap");exit(-1);}

	//if (pci_device_map_range(pci_device_find_by_slot(0,6,0xe,0), 0xfa000000, BARSIZE, 0 , &sysadr)) perror("pcimap");

	rval=  *(sysadr+0x0800/4);
	printf("BAR0 AD800 = %08x \n",rval);
	printf("in SLOT: %d\n",GEOADR(rval));

	CTget_brd_status(str,0);
	printf("%s\n",str);

	{
		nbtrig=0;
		i=0;
		if (argc>=6)
		 {
			printf("Initing ...\n");
			CTstatusprint(rval);
			usleep(100);

			printf("FIFOOUT: %08x\n",*(MUTADR(0,CTFO)));
			printf("CMD: %08x\n",*(MUTADR(brd,CTCFG)));

			printf("Reseting Unlocks ...\n");

			*(MUTADR(brd,CTCMD))= 0x4;
			*(MUTADR(brd,CTCMD))= 0x0;

			rval= *(MUTADR(0,CTSTAT));
			CTstatusprint(rval);


			//if (viMoveIn32(CTRIGBRD[i], CT_SPA, CTSTAT, 8, (ViAUInt32) buff32) < VI_SUCCESS) { perror("viMoveIn32"); }
			//strcpy(trgdate,"TESTTRG"); if (CTsendFake(i,1,44,3,1,trgdate)<12) printf("Error TX FIFO full\n");

			if (CTsendCMD(i, 1, 44, 0, 0x2000, 0x1234)<12) printf("Error TX FIFO is full\n"); /* Remote read CMD (=bac00d) */
			usleep(100);

			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMCMD:  %08lx\n",lval);

			if (CTsendCMD(i, 1, 44, 0, 0x800, 0x1234)<12) printf("Error TX FIFO is full\n");/* Remote read status */
			usleep(100);

			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMSTAT: %08lx\n",lval);
			if ((lval&0XE0000000)==0x20000000) CTRMstatusprint(lval); else printf("toto\n");

			lval= *(MUTADR(brd,CTCFG));
			printf("CFG: %08lx\n",lval);
			CTset_cfg(i,SYNC0|SYNC1|SYNC2|SYNC3);
			usleep(10000);
			lval= *(MUTADR(brd,CTCFG));
			printf("CFG: %08lx\n",lval);
			CTunset_cfg(i,SYNC0|SYNC1|SYNC2|SYNC3);
			usleep(2000);
			lval= *(MUTADR(brd,CTCFG));
			printf("CFG: %08lx\n",lval);

			if (CTsendCMD(i, 1, 44, 1, CTCFG, SYNCCT)<12) printf("Error TX FIFO is full\n");/* Remote send SYNC */
			usleep(200000);
			if (CTsendCMD(i, 1, 44, 1, CTCFG, 0x0)<12) printf("Error TX FIFO is full\n");/* Remote unsend SYNC */
			usleep(10000);

			if (argc==6)
			  {
			    if (CTsendCMD(i, 1, 44, 1, CTCFG, RMRSTPLL)<12) printf("Error TX FIFO is full\n"); /* Remote reset PLL*/
			    usleep(100);
			    if (CTsendCMD(i, 1, 44, 1, CTCFG, 0)<12) printf("Error TX FIFO is full\n"); /* Remote unreset PLL*/
			    usleep(100);
			  }
			else
			  {
			    printf("Sending Remote Reset\n");
			    if (CTsendCMD(i, 1, 44, 1, CTCFG, RMRST0|RMRST1)<12) printf("Error TX FIFO is full\n"); /* Remote reset */
			    usleep(100);
			    if (CTsendCMD(i, 1, 44, 1, CTCFG, 0)<12) printf("Error TX FIFO is full\n"); /* Remote unreset */
			    usleep(100);
			  }

			printf("TESTING trigger link\n");
			if (CTsendCMD(i, 1, 44, 0, 0x800, 0x1234)<12) printf("Error TX FIFO is full\n");/* Remote read status */
			usleep(100);
			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMSTAT: %08lx\n",lval);
			if (CTsendCMD(i, 1, 44, 0, 0x000, 0x1234)<12) printf("Error TX FIFO is full\n"); /* Remote read CFG reg */
			usleep(100);
			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMCFG:  %08lx\n",lval);

			if (CTsendCMD(i, 1, 44, 1, 0x000, 0x3400)<12) printf("Error TX FIFO is full\n");/* Remote write cfg */
			usleep(100);

			if (CTsendCMD(i, 1, 44, 0, 0x800, 0x1234)<12) printf("Error TX FIFO is full\n");/* Remote read status */
			usleep(100);

			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMSTAT: %08lx\n",lval);
			if (CTsendCMD(i, 1, 44, 0, 0x000, 0x1234)<12) printf("Error TX FIFO is full\n"); /* Remote read CFG reg */
			usleep(100);

			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMCFG:  %08lx\n",lval);

			if (CTsendCMD(i, 1, 44, 0, 0x000, 0x1234)<12) printf("Error TX FIFO is full\n"); /* Remote read CFG reg */
			usleep(100);

			lval= *(MUTADR(brd,CTSTATDA));
			printf("RMCFG:  %08lx\n\n",lval);

			for(j=0;j<8;j++)
			  {
			    if (CTsendCMD(i, 1, 44, 0, 0x800, 0x1234)<12) printf("Error TX FIFO is full\n");/* Remote read status */
			    usleep(100);
			    lval= *(MUTADR(brd,CTSTATDA));
			    if ((lval!=0x370000ba)&&(lval!=0x330000b6)&&(lval!=0x310000b4)&&(lval!=0x2f0000b2)) printf("RMSTAT: %08lx\n",lval);
			    if (CTsendCMD(i, 1, 44, 0, 0x2000, 0x1234)<12) printf("Error TX FIFO is full\n"); /* Remote read CMD reg */
			    usleep(100);
			    lval= *(MUTADR(brd,CTSTATDA));
			    if (lval!=0xa3bac00d) printf("RMCMD:  %08lx\n",lval);
			  }

			//CTsoft_trig(i,44);
			//usleep(100);

			//if (CTsendCMD(i, 1, 44, 1, 0x0, 0x00800000)<12) printf("Error TX FIFO is full\n");/* Remote send TESTTRIG */
			//if (CTsendCMD(i, 1, 44, 1, 0x0, 0x00000000)<12) printf("Error TX FIFO is full\n");/* Remote stop TESTTRIG */

			usleep(100);

		 }
		else if (argc==4)
		 {
//			printf("Sending %i fake triggers \n",atoi(argv[1]));
//		   for (j=0;j< atoi(argv[1]);j++) {CTsendFake(0,0,7,0,j, "00000000");usleep(1000);}
//		   goto kkkb;
			printf("Sending a trigger softly \n");
			*(sysadr+0x1000/4) = 0x1FB; /* Header= 1FB */
			*(sysadr+0x1000/4) = 0xC4; /* testbit and telescope number */
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;; /* address high */
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x20;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			sm= 0xE4;
			*(sysadr+0x1000/4) = sm;
			*(sysadr+0x1000/4) = 0x11C;
			usleep(100); /**/

			*(sysadr+0x1000/4) = 0x1FB; /* Header= 1FB */
			*(sysadr+0x1000/4) = 0xC4;; /* testbit and telescope number */
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0; /* address high */
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			*(sysadr+0x1000/4) = 0x0;
			sm= 0xC4;
			*(sysadr+0x1000/4) = sm;
			*(sysadr+0x1000/4) = 0x11C;
kkkb:			usleep(100);

		 }
		else if (argc==3)
		 {

			if (rdblock(0x1800, 1, buff32)) { perror("rdblock"); }
	       		printf("%08x\n",buff32[0]);
			if (rdblock(0x1800, 1, buff32)) { perror("rdblock"); }
	       		printf("%08x\n",buff32[0]);
			if (rdblock(0x1800, 1, buff32)) { perror("rdblock"); }
	       		printf("%08x\n",buff32[0]);


/*
			if (rdblock(0x1800, 3, buff32)) { perror("rdblock"); }

			i=0;
	       		j=(buff32[1+3*i]&0x00FFFFFF)*100+4*((buff32[0+3*i]&0xF8000000)>>27)
	       		    +cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))/4-jj;
	       		jj=(buff32[1+3*i]&0x00FFFFFF)*100+4*((buff32[0+3*i]&0xF8000000)>>27)
	       		    +cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))/4;
	       		if (i) al+=j;
	       		printf("%08x%08x%08x  %04xs + %9ins + %2ins + %03x    delta=%9ins   %i\n",buff32[2+3*i],buff32[1+3*i],buff32[0+3*i],
	       		  ((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24), (buff32[1+3*i]&0x00FFFFFF)*100,
	       		  4*((buff32[0+3*i]&0xF8000000)>>27),((buff32[0+3*i]&0x07FF0000)>>16),j,
	       		  cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))%4);
*/

		        rval = *(sysadr+0x800/4);
			printf("STAT=%08x\n",rval);
		 }
		else if (argc==2) while ((rval&0x1FFF)>0)
		 {
			if (rdblock(0x1800, 0x100, buff32)) { perror("rdblock"); }
		        rval = *(sysadr+0x800/4);

		 }
		else if (argc==5) while ((rval&0x1FFF)>0)
		 {
			printf("FIFO: ");
			//if (rdblock(0x1800, 0x1, buff32)) { perror("rdblock"); }
		        rval = *(sysadr+0x1800/4);
			printf("%08x\n",rval);
		        rval = *(sysadr+0x800/4);

		 }
		else while (1)
		 {
		    while ((rval&0x1FFF)>=3)
		    {
			nn = rval&0x1FFF; if (nn>=255) {nn=255;} else nn=3*(nn/3);

			//printf("Reading %i trigger(s) \n",nn/3);
			//tim = myclk()-tim;
			//printf("%i\n",tim);
			//tim = myclk();

			if (rdblock(0x1800, nn, buff32)) { perror("rdblock"); }

			//tim = myclk()-tim;
			//printf("%i\n",tim);
			//tim = myclk();

			jj=0;al=0;
			for(i=0;i<nn/3;i++)
			 {

				//j=(buff32[2+6*i]&0x00FFFFFF)*100+4*((buff32[0+6*i]&0xF8000000)>>27)
				//    +cnt0(((buff32[4+6*i]&0xFF)<<8)+((buff32[2+6*i]&0xFF000000)>>24))/4-jj;
				//jj=(buff32[2+6*i]&0x00FFFFFF)*100+4*((buff32[0+6*i]&0xF8000000)>>27)
				//    +cnt0(((buff32[4+6*i]&0xFF)<<8)+((buff32[2+6*i]&0xFF000000)>>24))/4;
				//if (i) al+=j;
				//printf("%08x%08x%08x  %04xs + %9ins + %2ins + %03x    delta=%9ins   %i\n",buff32[4+6*i],buff32[2+6*i],buff32[0+6*i],
				//  ((buff32[4+6*i]&0xFF)<<8)+((buff32[2+6*i]&0xFF000000)>>24), (buff32[2+6*i]&0x00FFFFFF)*100,
				//  4*((buff32[0+6*i]&0xF8000000)>>27),((buff32[0+6*i]&0x07FF0000)>>16),j,
				//  cnt0(((buff32[4+6*i]&0xFF)<<8)+((buff32[2+6*i]&0xFF000000)>>24)));

				//j=(buff32[3+6*i]&0x00FFFFFF)*100+4*((buff32[1+6*i]&0xF8000000)>>27)
				//    +cnt0(((buff32[5+6*i]&0xFF)<<8)+((buff32[3+6*i]&0xFF000000)>>24))/4-jj;
				//jj=(buff32[3+6*i]&0x00FFFFFF)*100+4*((buff32[1+6*i]&0xF8000000)>>27)
				//    +cnt0(((buff32[5+6*i]&0xFF)<<8)+((buff32[3+6*i]&0xFF000000)>>24))/4;
				//if (i) al+=j;
				//printf("%08x%08x%08x  %04xs + %9ins + %2ins + %03x    delta=%9ins   %i\n",buff32[5+6*i],buff32[3+6*i],buff32[1+6*i],
				//  ((buff32[5+6*i]&0xFF)<<8)+((buff32[3+6*i]&0xFF000000)>>24), (buff32[3+6*i]&0x00FFFFFF)*100,
				//  4*((buff32[1+6*i]&0xF8000000)>>27),((buff32[1+6*i]&0x07FF0000)>>16),j,
				//  cnt0(((buff32[5+6*i]&0xFF)<<8)+((buff32[3+6*i]&0xFF000000)>>24)));

                                //printf("%08x%08x%08x  %04xs + %9ins + %2ins + %03x    delta=%9ins   %i\n",buff32[2+3*i],buff32[1+3*i],buff32[0+3*i],
                                //  ((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24), (buff32[1+3*i]&0x00FFFFFF)*100,
                                //  4*((buff32[0+3*i]&0xF8000000)>>27),((buff32[0+3*i]&0x07FF0000)>>16),j,
                                //  cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))%4);


				tnb= (buff32[2+3*i]&0x007F0000)/0x10000 ;
				if ((trgcnt[tnb]+1)%16 != (buff32[2+3*i]&0x0F00)/0x100 )
					{ serr="ERROR"; }
				 else serr="";
				if (i==0) { nbr=nn; } else nbr=0;
				trgcnt[tnb] = (buff32[2+3*i]&0x0F00)/0x100 ;

                                j=(buff32[1+3*i]&0x00FFFFFF)*100+4*((buff32[0+3*i]&0xF8000000)>>27)
                                    +cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))/4;//-jj;
				//jj=(buff32[1+3*i]&0x00FFFFFF)*100+4*((buff32[0+3*i]&0xF8000000)>>27)
				//    +cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))/4;
                                if (i) al+=j;
                                printf("%08x%08x%08x  T%2x  %04xs  %04x time=%9ins   %i %s\n",buff32[2+3*i],buff32[1+3*i],buff32[0+3*i],tnb,
                                  ((buff32[0+3*i]&0x07FF0000)/0x10000), ((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24),j,
                                  nbr,serr);

				//printf("%08x%08x%08x  T%2x  %04xs + %9ins + %2ins + %04x    delta=%9ins   %i %i %i %s\n"!
                                //  ((buff32[0+3*i]&0x07FF0000)/0x10000), (buff32[1+3*i]&0x00FFFFFF)*100,
                                //  4*((buff32[0+3*i]&0xF8000000)/0x8000000), ((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0x!
                                //  cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24))/4,
                                //  cnt0(((buff32[2+3*i]&0xFF)<<8)+((buff32[1+3*i]&0xFF000000)>>24)),nbr,serr);


			 }
			//if (i>1) printf("moy = %f\n",((double)al)/(float)(i-1));
		        rval = *(sysadr+0x800/4);
		    }
		    rval = *(sysadr+0x800/4);
		 }
	}

	printf("Close it\n");
	pci_cleanup(pca);

        munmap((void *)sysadr,BARSIZE);
 	if (close(fd)==-1) perror("close");
	printf("Terminating normally\n");

	return 0;
}

