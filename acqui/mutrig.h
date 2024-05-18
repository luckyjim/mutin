/* Central Trigger firmware interface */
/* CTstatusprint() */

#if !defined(CTRIG_H)
#define CTRIG_H

/*     memory mapping          */
#define  CTCFG  	0x00000     /* D32 Config register */
#define  CTSTAT 	0x00800     /* D32 Status register */
#define  CTFO   	0x01000     /* D32 FIFO Sending Trames to Telescope */
#define  CTFI   	0x01800     /* D32 FIFO Receiving Trigger Trames from Telescope */
#define  CTCMD  	0x02000     /* D32 Command register */
//#define  CTSTATB 	0x02800     /* D32 StatusB register */
//#define  CTSTATC 	0x03000     /* D32 StatusC register */
#define  CTSTATDA 	0x03800     /* D32 StatusD register */
#define  CTSTATDB 	0x03810     /* D32 StatusD register */
#define  CTSTATDC 	0x03820     /* D32 StatusD register */
#define  CTSTATDD 	0x03830     /* D32 StatusD register */

#define  GEOADR(x)  ((x>>27)&0x1F)        /* PXI GEO Address */
#define  BADCHKS    (1<<26)               /* 1 if bad cheksum detected for last frame */
#define  NLOCK3     (1<<25)               /* 1 if Deserialiser not Locked */
#define  NLOCK2     (1<<24)               /* 1 if Deserialiser not Locked */
#define  NLOCK1     (1<<23)               /* 1 if Deserialiser not Locked */
#define  NLOCK0     (1<<22)               /* 1 if Deserialiser not Locked */
#define  TXFREE(x)  ((x>>13)&0x1FF)       /* TX Fifo Available in chars */
#define  RXUSED(x)  (x&0x1FFF)            /* RX Fifo Use in 32bits words */


#define  SYNC3     (1<<3)           /* CFG bit for Syncing Serialiser Link */
#define  SYNC2     (1<<2)           /* CFG bit for Syncing Serialiser Link */
#define  SYNC1     (1<<1)           /* CFG bit for Syncing Serialiser Link */
#define  SYNC0     (1<<0)           /* CFG bit for Syncing Serialiser Link */

#define  RSTLOK    (1<<2)           /* CMD bit for Reseting the Unlock Monitors */
#define  LEDR      (1<<1)           /*  */
#define  LEDV      (1<<0)           /*  */


/*    Telescope Remote Config    to be replaced with (MUTEL) interface */

#define  RMRST1     (1<<23)          /* Together with RMRST0 this should reset  */
#define  RMRST0     (1<<22)          /* the Telescope Central Trigger Interface */
#define  RMSOFTRIG  (1<<21)          /* The Telescope should send a test trigger */
#define  RMTEST     (1<<20)          /* The Telescope should setup a test trigger mode */
#define  RMTAGI     (1<<19)          /* The Telescope should send a PLLinputclock test trigger */
#define  RMTAGO     (1<<18)          /* The Telescope should send a PLLoutputclock test trigger */
#define  RMPHSTEP   (1<<17)          /* The Telescope should step the phase of its TTAG PLLoutputclock */
#define  RMPHUPDO   (1<<16)          /*     this step should be up (1) or down (0) of 250ps            */
#define  RMRSTPLL   (1<<15)          /* This should reset the TTAG PLL with a random phase */
#define  SYNCCT     (1<<0)           /* CFG bit for Syncing Serialiser Link */

/*    Telescope Remote Status    to be replaced with (MUTEL) interface */
#define  RMRST1     (1<<23)          /* Together with RMRST0 this should reset  */
#define  RMRST0     (1<<22)          /* the Telescope Central Trigger Interface */

#define  NLOCKTR    (1<<9)           /* Trigger link deserialiser is not locked */
#define  RXBAD      (1<<8)           /* Clock from the Deserialiser PLL unlock detected - needs reset*/
#define  RXLOCK     (1<<7)           /* Clock from the Deserialiser PLL is locked */
#define  IOXBAD     (1<<6)           /* Internal Oscillator PLL unlock detected - needs reset*/
#define  IOXLOCK    (1<<5)           /* Internal Oscillator PLL is locked */
#define  PHCKERR    (1<<4)           /* the fast clock is NOT synchro */
#define  DISOK      (1<<3)           /* The clock distribution says its clock is reliable (DESER) */
#define  DISBAD     (1<<2)           /* Distri Clock PLL unlock detected - needs reset*/
#define  DISLOCK    (1<<1)           /* Distri Clock PLL is locked */
#define  TRIGLOST   (1<<0)           /* trigger lost detected */

/*    Telescope Remote Commands    */
//


CTstatusprint(unsigned long i)
{
        printf("GEOADDR=%2ld ", GEOADR(i));
        if (i & NLOCK0) printf("UNLK0 "); else printf("LOKD0 ");
        if (i & NLOCK1) printf("UNLK1 "); else printf("LOKD1 ");
        if (i & NLOCK2) printf("UNLK2 "); else printf("LOKD2 ");
        if (i & NLOCK3) printf("UNLK3 "); else printf("LOKD3 ");
        printf("TXFREE=%ld RXUSED=%ld\n",TXFREE(i),RXUSED(i));
}

CTRMstatusprint(unsigned long i) /* to be replaced with (MUTEL) interface */
{
        printf("Telescope Interface Status:\n ");
        printf("TRDES");if (i & NLOCKTR) printf("UNLOCKED "); else printf("LOCKED   ");
        printf("|DESCLK");if (i & RXBAD) printf("BAD  "); else printf("GOOD ");
        printf("DESCLK");if (i & RXLOCK) printf("LOCKED   "); else printf("UNLOCKED   ");
        printf("|IOXCLK");if (i & IOXBAD) printf("BAD  "); else printf("GOOD ");
        printf("IOXCLK");if (i & IOXLOCK) printf("LOCKED   "); else printf("UNLOCKED   ");
        printf("|DISCLK");if (i & DISBAD) printf("BAD  "); else printf("GOOD ");
        printf("DISCLK");if (i & DISLOCK) printf("LOCKED   "); else printf("UNLOCKED   ");

        printf("DISTRI");if (i & DISOK) printf("GOOD "); else printf("BAD  ");
        printf("|PHCLK");if (i & PHCKERR) printf("BAD  "); else printf("GOOD ");
        printf("|");if (i & TRIGLOST) printf("TRIGLOST   "); else printf("NOTRIGLOST ");


        printf("\n");
}



#endif

