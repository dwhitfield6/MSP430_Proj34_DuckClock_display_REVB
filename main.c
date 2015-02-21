#include <msp430.h> 

#define CS BIT0  //2.0 is SPI CS
#define MOSI BIT7  //1.7 is SPI MOSI
#define SCLK BIT5  //1.5 is SPI clock
#define LED1 BIT4  //1.5 is SPI clock
#define LED0 BIT3  //1.5 is SPI clock
#define switchright BIT2  //1.5 is SPI clock
#define switchcenter BIT3  //1.5 is SPI clock
#define switchleft BIT1  //1.5 is SPI clock
#define alarmPin BIT2  //1.5 is SPI clock
#define POT1 INCH_0
#define zerocross BIT5
#define audiopower BIT1
#define AlarmON BIT4

#define ON 1
#define OFF 0
#define AM 51
#define PM 52


void Init_MAX7219(void);
void printmatrix(unsigned char one, unsigned char two, unsigned char three, unsigned char four, unsigned char five);
void setTime(void);
void updateNumber(void);
void setIntensity(void);
void SPI_Init(void); //SPI initialization
void SPI_Write(unsigned char);
void SPI_Write2(unsigned char, unsigned char);
void SPI_Write3(unsigned char, unsigned char, unsigned char);

int pot1value = 0;
int right=0;
int left=0;
int center=0;
unsigned int intensity =0;
unsigned char textBuffer[100];
unsigned char timeString[100];
unsigned char Length = 0;
int digits =30;
unsigned int a,b,c,d,e;
int j,i,ii;
unsigned int jj,jjj,jjjj,jjjjj,jjjjjj;
int hours =2;
int minutes =52;
int seconds =31;

unsigned char alarmONcount =0;

int hours12 =2;
unsigned char hoursfirst =2;
unsigned char hourssecond =3;
unsigned char minutessfirst =2;
unsigned char minutessecond =3;
unsigned char secondsfirst =2;
unsigned char secondssecond =3;
unsigned char hundseconds =0;
unsigned char update =1;
int i =0;
int Colon =0;

unsigned char Alarmhours =2;
unsigned char Alarmminutes=54;

unsigned char Alarmcount =0;
unsigned char Alarmset =0;
unsigned char hours12first =0;
unsigned char hours12second =0;
unsigned char Alarmhoursfirst =0;
unsigned char Alarmhourssecond =0;
unsigned char Alarmminutessfirst =0;
unsigned char Alarmminutessecond =0;
unsigned char timeOFday =AM;

unsigned const char dispFRONT[53][8]={
		{	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	}	,	//	null
		{	248	,	20	,	18	,	17	,	18	,	20	,	248	,	0	}	,	//	A
		{	255	,	137	,	137	,	137	,	137	,	137	,	118	,	0	}	,	//	B
		{	60	,	66	,	129	,	129	,	129	,	129	,	129	,	0	}	,	//	C
		{	255	,	129	,	129	,	129	,	66	,	60	,	0	,	0	}	,	//	D
		{	255	,	137	,	137	,	137	,	137	,	129	,	129	,	0	}	,	//	E
		{	255	,	9	,	9	,	9	,	9	,	1	,	1	,	0	}	,	//	F
		{	60	,	66	,	129	,	129	,	161	,	226	,	32	,	0	}	,	//	G
		{	255	,	16	,	16	,	16	,	16	,	16	,	255	,	0	}	,	//	H
		{	129	,	129	,	129	,	255	,	129	,	129	,	129	,	0	}	,	//	I
		{	65	,	129	,	129	,	129	,	255	,	1	,	1	,	0	}	,	//	J
		{	255	,	8	,	20	,	34	,	65	,	128	,	0	,	0	}	,	//	K
		{	255	,	128	,	128	,	128	,	128	,	128	,	128	,	0	}	,	//	L
		{	255	,	2	,	4	,	8	,	4	,	2	,	255	,	0	}	,	//	M
		{	255	,	2	,	4	,	8	,	16	,	32	,	255	,	0	}	,	//	N
		{	255	,	129	,	129	,	129	,	129	,	129	,	255	,	0	}	,	//	O
		{	255	,	9	,	9	,	9	,	9	,	9	,	6	,	0	}	,	//	P
		{	127	,	65	,	65	,	97	,	65	,	193	,	127	,	0	}	,	//	Q
		{	255	,	9	,	9	,	25	,	41	,	73	,	143	,	0	}	,	//	R
		{	143	,	137	,	137	,	137	,	137	,	137	,	249	,	0	}	,	//	S
		{	1	,	1	,	1	,	255	,	1	,	1	,	1	,	0	}	,	//	T
		{	127	,	128	,	128	,	128	,	128	,	128	,	127	,	0	}	,	//	U
		{	31	,	32	,	64	,	128	,	64	,	32	,	31	,	0	}	,	//	V
		{	255	,	128	,	64	,	32	,	64	,	128	,	255	,	0	}	,	//	W
		{	130	,	68	,	40	,	16	,	40	,	68	,	130	,	0	}	,	//	X
		{	1	,	2	,	4	,	248	,	4	,	2	,	1	,	0	}	,	//	Y
		{	0	,	193	,	161	,	145	,	137	,	133	,	131	,	0	}	,	//	Z
		{	0	,	126	,	129	,	129	,	129	,	126	,	0	,	0	}	,	//	0
		{	0	,	130	,	129	,	255	,	128	,	128	,	0	,	0	}	,	//	1
		{	0	,	194	,	161	,	145	,	142	,	0	,	0	,	0	}	,	//	2
		{	0	,	0	,	66	,	145	,	145	,	110	,	0	,	0	}	,	//	3
		{	0	,	30	,	16	,	16	,	255	,	16	,	0	,	0	}	,	//	4
		{	0	,	79	,	137	,	137	,	137	,	113	,	0	,	0	}	,	//	5
		{	0	,	28	,	98	,	145	,	145	,	146	,	96	,	0	}	,	//	6
		{	0	,	1	,	1	,	249	,	5	,	3	,	0	,	0	}	,	//	7
		{	0	,	0	,	118	,	137	,	137	,	118	,	0	,	0	}	,	//	8
		{	0	,	70	,	137	,	137	,	73	,	62	,	0	,	0	}	,	//	9
		{	14	,	17	,	34	,	68	,	34	,	17	,	14	,	0	}	,	//	heart
		{	0	,	0	,	14	,	159	,	14	,	0	,	0	,	0	}	,	//	!
		{	60	,	66	,	149	,	161	,	149	,	66	,	60	,	0	}	,	//	smilly
		{	48	,	72	,	72	,	56	,	8	,	8	,	28	,	8	}	,	//	boner1
		{	8	,	28	,	8	,	8	,	56	,	72	,	72	,	48	}	,	//	boner2
		{	14	,	17	,	21	,	14	,	17	,	21	,	14	,	0	}	,	//	eyes right
		{	14	,	21	,	21	,	14	,	21	,	21	,	14	,	0	}	,	//	eyes center
		{	14	,	21	,	17	,	14	,	21	,	17	,	14	,	0	}	,	//	eyes left
		{	14	,	25	,	17	,	14	,	25	,	17	,	14	,	0	}	,	//	eyes low left
		{	14	,	17	,	25	,	14	,	17	,	25	,	14	,	0	}	,	//	eyes low right
		{	14	,	19	,	17	,	14	,	19	,	17	,	14	,	0	}	,	//	eyes high left
		{	14	,	17	,	19	,	14	,	17	,	19	,	14	,	0	}	,	//	eyes high right
		{	0	,	0	,	0	,	102	,	102	,	0	,	0	,	0	}	,	//	:
		{	48	,	60	,	62	,	127	,	62	,	60	,	48	,	0	}	,	//	bell
		{	102	,	102	,	0	,	236	,	38	,	69	,	38	,	236	}	,	//	Colon AM
		{	102	,	102	,	0	,	239	,	37	,	69	,	37	,	231	}	,	//	Colon PM
};

void setTime(void)
{
	while(1)
	{
	for(ii = 30;ii>0;ii--)
	{
	  	if((P1IN & switchleft) != switchleft)
	  	{
	  		left++;
	  	}
	  	if((P1IN & switchright) != switchright)
	  	{
	  		right++;
	  	}
	  	if((P1IN & switchcenter) != switchcenter)
	  	{
	  		center++;
	  	}

	  	_delay_cycles(10);
	}
	if(left > 20)
	{
		  if(Alarmhours<10)
		  {
			  printmatrix(0,(Alarmhoursfirst + (27 - 48)),49,(Alarmminutessecond +(27 - 48)),(Alarmminutessfirst+(27 - 48)));
			  //printmatrix(0,(hoursfirst + (27 - 48)),Colon,(secondssecond +(27 - 48)),(secondsfirst+(27 - 48)));
		  }
		  else
		  {

			  printmatrix((Alarmhourssecond+ (27 - 48)),(Alarmhoursfirst + (27 - 48)),49,(Alarmminutessecond +(27 - 48)),(Alarmminutessfirst+(27 - 48)));
		  }
	}


	if(right > 20)
	{
		if(Alarmset == ON)
		{
			Alarmminutes++;
			if(Alarmminutes >59)
			{
				Alarmminutes =0;
			}
			updateNumber();
			printmatrix(50,(Alarmminutessecond - 21),(Alarmminutessfirst-21),0,13);
			alarmONcount=0;
		}
		else
		{
		minutes++;
		if(minutes >59)
		{
			minutes =0;
		}
		alarmONcount=0;
		updateNumber();
		printmatrix(0,(minutessecond - 21),(minutessfirst-21),0,13);
		}
		_delay_cycles(5000000);

	}

	if(center > 20)
	{
		if(Alarmset == ON)
		{
			Alarmhours++;
			if(Alarmhours >24)
			{
				Alarmhours =1;
			}
			updateNumber();
			printmatrix(50,(Alarmhourssecond - 21),(Alarmhoursfirst-21),0,8);
		}
		else
		{
		hours++;
		if(hours > 24)
		{
			hours =1;
		}
		updateNumber();
		printmatrix(0,(hours12second - 21),(hours12first-21),0,8);
		}
		_delay_cycles(5000000);
	}
	if(right ==0 && left == 0 && center ==0)
	{
		break;
	}

	left=0;
	right=0;
	center=0;
	Alarmcount =0;
	}
	update =1;
}

unsigned int analogRead(unsigned int pin) {
 ADC10CTL0 = ADC10ON + ADC10SHT_0 + SREF_0;
 ADC10CTL1 = ADC10SSEL_0 + pin;
 if (pin==INCH_0){
 ADC10AE0 = 0x01;
 }
 else if (pin==INCH_1){
 ADC10AE0 = 0x02;
 }
 else if (pin==INCH_2){
 ADC10AE0 = 0x04;
 }
 else if (pin==INCH_3){
  ADC10AE0 = 0x08;
  }
 else if (pin==INCH_4){
  ADC10AE0 = 0x10;
  }
 else if(pin==INCH_5){
 ADC10AE0 = 0x20;
 }
 else if(pin==INCH_6){
  ADC10AE0 = 0x40;
  }
 else if(pin==INCH_7){
  ADC10AE0 = 0x80;
  }
 ADC10CTL0 |= ENC + ADC10SC;
 //_delay_cycles(10);
 while (1) {
 if (((ADC10CTL0 & ADC10IFG)==ADC10IFG)) {
 ADC10CTL0 &= ~(ADC10IFG +ENC);
 break;
 }
 }
 return ADC10MEM;
 }

void printmatrix(unsigned char one, unsigned char two, unsigned char three, unsigned char four, unsigned char five) //SPI initialization
{
  int aa;

  //setIntensity();

  for(aa=1;aa<9;aa++)
  {
    P2OUT &= ~CS;
    _delay_cycles(10);
    UCB0TXBUF = aa ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = dispFRONT[five][aa-1];
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = aa ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = dispFRONT[four][aa-1];
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = aa ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = dispFRONT[three][aa-1];
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = aa ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = dispFRONT[two][aa-1];
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = aa ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = dispFRONT[one][aa-1];
    while (UCB0STAT & UCBUSY);
    P2OUT |= CS;
  }

}

void updateNumber(void)
{
	//  daysfirst = (days%10) + 48;
	//  dayssecond = (days/10) + 48;
	  Alarmhoursfirst = (Alarmhours%10) + 48;
	  Alarmhourssecond = (Alarmhours/10) + 48;
	  Alarmminutessfirst = (Alarmminutes%10) + 48;
	  Alarmminutessecond = (Alarmminutes/10) + 48;

	  hoursfirst = (hours%10) + 48;
	  hourssecond = (hours/10) + 48;
	  if(hours >12)
	  {
		  timeOFday = PM;
		  hours12 = hours - 12;
	  }
	  else
	  {
		  timeOFday = AM;
		  hours12 = hours;
	  }
	  hours12first = (hours12%10) + 48;
	  hours12second = (hours12/10) + 48;
	  minutessfirst = (minutes%10) + 48;
	  minutessecond = (minutes/10) + 48;
	  secondsfirst = (seconds%10) + 48;
	  secondssecond = (seconds/10) + 48;
		for(ii = 30;ii>0;ii--)
		{
	  	if((P1IN & AlarmON) != AlarmON)
	  	{
	  		Alarmcount++;
	  	}
		}
		if(Alarmcount > 20)
		{
			Alarmset = ON;
		}
		else
		{
			Alarmset = OFF;
		}
		Alarmcount =0;
}

void setIntensity(void)
{
	//pot1value = (analogRead(POT1)) >> 6;
	//intensity = (0x0F & pot1value);
	intensity = 0;
	P2OUT &= ~CS;
	    _delay_cycles(10);
	    UCB0TXBUF = 0x0A;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = intensity;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = 0x0A;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = intensity;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = 0x0A;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = intensity;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = 0x0A;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = intensity;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = 0x0A;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = intensity;
	    while (UCB0STAT & UCBUSY);
	    P2OUT |= CS;
}

void SPI_Init(void) //SPI initialization
{
	_delay_cycles(2000);
  P2DIR |= CS;
  P1SEL |= MOSI + SCLK;
  P1SEL2 |= MOSI + SCLK;
  UCB0CTL1 = UCSWRST;
  UCB0CTL0 |= UCMSB + UCMST + UCSYNC + UCCKPH; // 4-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 20;                          // /2
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  __enable_interrupt(); // enable all interrupts
}

void SPI_Write2(unsigned char MSB, unsigned char LSB) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(50);
  UCB0TXBUF = MSB ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = LSB ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}
void SPI_Write1(unsigned char MSB) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = MSB ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}
void SPI_Write3(unsigned char MSB,unsigned char MMSB,unsigned char LSB ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = MSB ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = MMSB ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = LSB ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}

void Matrix1(unsigned char address,unsigned char data ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = address ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = data ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0x00 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}
void Matrix2(unsigned char address,unsigned char data ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = address ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = data ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}

void Matrix3(unsigned char address,unsigned char data ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = address ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = data ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}

void Matrix4(unsigned char address,unsigned char data ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = address ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = data ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}

void Matrix5(unsigned char address,unsigned char data ) //SPI write one byte
{

  P2OUT &= ~CS;
  _delay_cycles(10);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = 0 ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = address ;
  while (UCB0STAT & UCBUSY);
  UCB0TXBUF = data ;
  while (UCB0STAT & UCBUSY);
  P2OUT |= CS;
}



void Init_MAX7219(void)
{
  Matrix1(0x09, 0x00);       //
  Matrix1(0x0A, 0x0F);       //
  Matrix1(0x0B, 0x0F);       //
  Matrix1(0x0C, 0x01);       //
  Matrix1(0x0F, 0x0F);       //
  Matrix1(0x0F, 0x00);       //

  Matrix2(0x09, 0x00);       //
  Matrix2(0x0A, 0x0F);       //
  Matrix2(0x0B, 0x0F);       //
  Matrix2(0x0C, 0x01);       //
  Matrix2(0x0F, 0x0F);       //
  Matrix2(0x0F, 0x00);       //

  Matrix3(0x09, 0x00);       //
  Matrix3(0x0A, 0x0F);       //
  Matrix3(0x0B, 0x0F);       //
  Matrix3(0x0C, 0x01);       //
  Matrix3(0x0F, 0x0F);       //
  Matrix3(0x0F, 0x00);       //

  Matrix4(0x09, 0x00);       //
  Matrix4(0x0A, 0x0F);       //
  Matrix4(0x0B, 0x0F);       //
  Matrix4(0x0C, 0x01);       //
  Matrix4(0x0F, 0x0F);       //
  Matrix4(0x0F, 0x00);       //

  Matrix5(0x09, 0x00);       //
  Matrix5(0x0A, 0x0F);       //
  Matrix5(0x0B, 0x0F);       //
  Matrix5(0x0C, 0x01);       //
  Matrix5(0x0F, 0x0F);       //
  Matrix5(0x0F, 0x00);       //
}

int main(void)
{

	  WDTCTL = WDTPW | WDTHOLD;
	  BCSCTL1 = CALBC1_16MHZ;
	  DCOCTL = CALDCO_16MHZ;

	  P2IE |= zerocross; // P1.3 interrupt enabled
	  P2IES &= ~zerocross; // P1.3 interrupt enabled
	  P2IFG &= ~zerocross; // P1.3 IFG cleared

	P1REN |= (switchright + switchcenter + switchleft + AlarmON);
    P1OUT |= (switchright + switchcenter + switchleft + AlarmON);

    P2DIR |= (LED1 + LED0 + alarmPin + audiopower);
    P2OUT |= (LED1   + audiopower);
    _delay_cycles(1000);
    P2OUT &= ~(LED0  + alarmPin);

  SPI_Init();
  _delay_cycles(100000);
  Init_MAX7219();
  _delay_cycles(1000);
  setIntensity();

  __enable_interrupt(); // enable all interrupts

  while(1)
  {

	  updateNumber();
	  _delay_cycles(10000);
	  setTime();
	  if(update == 1)
	  {
	  if(hours12<10)
	  {
		  printmatrix(0,(hours12first + (27 - 48)),Colon,(minutessecond +(27 - 48)),(minutessfirst+(27 - 48)));
		  //printmatrix(0,(hoursfirst + (27 - 48)),Colon,(secondssecond +(27 - 48)),(secondsfirst+(27 - 48)));
	  }
	  else
	  {
		  printmatrix((hours12second+ (27 - 48)),(hours12first + (27 - 48)),Colon,(minutessecond +(27 - 48)),(minutessfirst+(27 - 48)));
	  }
	  update =0;
	  }
	  alarmONcount++;
	  if(alarmONcount>200)
	  {
		  alarmONcount =200;
		  P2OUT &= ~alarmPin;
	  }

  }

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
hundseconds++;
if(hundseconds >59)
{
	update = ON;
	hundseconds =0;
	P2OUT ^= LED0;
	seconds++;
	P2OUT ^= LED1;
	if(Alarmset == ON)
	{
		if(Colon == timeOFday)
		{
			Colon =50;
		}
		else
		{
			Colon =timeOFday;
		}
if(Alarmhours == hours && Alarmminutes == minutes && alarmONcount >199 && seconds == 5)
{
	 P2OUT |= alarmPin;
	 alarmONcount=0;
}
	}
	else
	{
		if(Colon == timeOFday)
		{
			Colon =0;
		}
		else
		{
			Colon = timeOFday;
		}
	}

}

	if(seconds >59)
	{
		seconds = 0;
		minutes++;
	}
	if(minutes >59)
	{
		minutes = 0;
		hours++;
	}
	if(hours >24)
	{
		hours = 1;
	}
	P2IFG &= ~zerocross; // P1.3 IFG cleared
}






