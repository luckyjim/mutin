#include<stdio.h>
#include<errno.h>
#include <usb.h>

#define MY_VID 0x04D8
#define MY_PID 0x4567

#define EP_IN 0x81
#define EP_OUT 0x01

#define RDSW2 0x81
#define CHKFIFO 0x47
#define SENDCMD 0x74
#define CHKTTAG 0x5A
#define RDTTAG 0xA5
#define TTEST 0x71


usb_dev_handle *MyLibusbDeviceHandle = NULL;


main()
{
	struct usb_bus *bus;
	struct usb_device *dev;
	unsigned char OutPak[64];
	unsigned char InPak[64];
	unsigned char cks,i,last;
	unsigned long cptslow;
	unsigned short tscope,cptsec,cptfast,photo,cpt,rcks,strt,stp,rlock;

	usb_init(); /* initialize the library */
	usb_find_busses(); /* find all busses */
	usb_find_devices(); /* find all connected devices */

	for(bus = usb_get_busses(); bus; bus = bus->next)
	 for(dev = bus->devices; dev; dev = dev->next)
	  if(dev->descriptor.idVendor==MY_VID && dev->descriptor.idProduct==MY_PID)
	   { MyLibusbDeviceHandle = usb_open(dev); break; }
	if(!(MyLibusbDeviceHandle)) { perror("usb_open"); exit(-1); }
	if(usb_set_configuration(MyLibusbDeviceHandle, 1) < 0)
	{
		perror("usb_set_config");
		usb_close(MyLibusbDeviceHandle);
		exit(-1);
	}
	if(usb_claim_interface(MyLibusbDeviceHandle, 0) < 0)
	{
		perror("usb_claim_int");
		usb_close(MyLibusbDeviceHandle);
		return ;
	}

	last=0;
	printf("strt stop tscp rlok sec    100ns 4ns phot  cpt  rcks ccks\n");

	while (1)
	{

	OutPak[0] = CHKTTAG;
	if (usb_bulk_write(MyLibusbDeviceHandle, EP_OUT, &OutPak[0], 64, 5000)!=64)
	 { perror("bulk_write"); }

	if (usb_bulk_read(MyLibusbDeviceHandle, EP_IN, &InPak[0], 64, 5000)!=64)
	 { perror("bulk_read"); }

	if (InPak[1]==0x01) // ttag fifo not empty
	{
		OutPak[0] = RDTTAG;
		if (usb_bulk_write(MyLibusbDeviceHandle, EP_OUT, &OutPak[0], 64, 5000)!=64)
		{ perror("bulk_write"); }

		if (usb_bulk_read(MyLibusbDeviceHandle, EP_IN, &InPak[0], 64, 5000)!=64)
		{ perror("bulk_read"); }

		//printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",InPak[0],InPak[1],InPak[2],InPak[3],InPak[4],InPak[5],InPak[6],InPak[7]);
		cks=0;
		for(i=0;i<7;i++) {cks+=(InPak[i]>>4)&0xF; cks+=InPak[i]&0xF;}
		cks+=(InPak[7]>>4)&0xF;
		cks=cks&0xF;
		//printf("%x\n",cks);
		strt=(InPak[0]>>7)&0x1;
		stp=(InPak[0]>>6)&0x1;
		tscope=((InPak[0]&0x3F)*2)+((InPak[1]>>7)&0x1);
		rlock=1-((InPak[1]>>6)&0x1);
		cptsec=(InPak[1]>>4)&0x3;
		cptslow=((unsigned long)(InPak[1]&0xF))*0x100000 + ((unsigned long)InPak[2])*0x1000 + ((unsigned long)InPak[3])*0x10 + ((InPak[4]>>4)&0xF);
		cptfast= (InPak[4]&0xF)*2 + ((InPak[5]>>7)&0x1);
		photo= (InPak[5]&0x7F)*2*256 + InPak[6]*2 + ((InPak[7]>>7)&0x1);
		cpt= (InPak[7]>>4)&0x7;
		rcks= InPak[7]&0xF;

		printf("  %i    %i   %3i   %i   %i  %8i  %2i %04x   %i     %x    %x  ",
		         strt,stp,tscope,rlock,cptsec,cptslow,cptfast,photo,cpt,rcks,cks);
		if ((rcks==cks)&&(cpt==((last+1)%8))) {printf("\n");} else printf("ERROR\n");
		last= cpt;
	}//else printf("\n");
	}
	usb_release_interface(MyLibusbDeviceHandle, 0);
	usb_close(MyLibusbDeviceHandle);

}

