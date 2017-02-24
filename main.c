#include <stdio.h>

//Uncomment if this robot receives data from PC
//#define MASTER

#define loop_until_bit_is_set(sfr,bit) \
do { } while (bit_is_clear(sfr, bit))

#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//////////////////////////////////////////////////////////////////////////
//LCD
#define RS 0
#define RW 1
#define EN 2
#define lcd_port PORTC

#define sbit(reg,bit)	reg |= (1<<bit)
#define cbit(reg,bit)	reg &= ~(1<<bit)

//unsigned char data; //to store received data from UDR1
unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;

//Function to configure LCD port
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

/*****Function to Reset LCD*****/
void lcd_set_4bit()
{
	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x20;				//Sending 2 to initialise LCD 4-bit mode
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	
}

/*****Function to Write Command on LCD*****/
void lcd_wr_command(unsigned char cmd)
{
	unsigned char temp;
	temp = cmd;
	temp = temp & 0xF0;
	lcd_port &= 0x0F;
	lcd_port |= temp;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
	
	cmd = cmd & 0x0F;
	cmd = cmd<<4;
	lcd_port &= 0x0F;
	lcd_port |= cmd;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

/*****Function to Write Data on LCD*****/
void lcd_wr_char(char letter)
{
	char temp;
	temp = letter;
	temp = (temp & 0xF0);
	lcd_port &= 0x0F;
	lcd_port |= temp;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	letter = letter & 0x0F;
	letter = letter<<4;
	lcd_port &= 0x0F;
	lcd_port |= letter;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

void lcd_home()
{
	lcd_wr_command(0x80);
}

/*****Function to Print String on LCD*****/
void lcd_string(char *str)
{
	while(*str != '\0')
	{
		lcd_wr_char(*str);
		str++;
	}
}

/*****Function to Initialize LCD*****/
void lcd_init()
{
	_delay_ms(1);

	lcd_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
	lcd_wr_command(0x01);
	lcd_wr_command(0x06);
	lcd_wr_command(0x0E);
	lcd_wr_command(0x80);
	
}

/*** Position the LCD cursor at "row", "column". ***/
void lcd_cursor (char row, char column)
{
	switch (row) {
		case 1: lcd_wr_command (0x80 + column - 1); break;
		case 2: lcd_wr_command (0xc0 + column - 1); break;
		case 3: lcd_wr_command (0x94 + column - 1); break;
		case 4: lcd_wr_command (0xd4 + column - 1); break;
		default: break;
	}
}

/***** Function To Print Any input value upto the desired digit on LCD *****/
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		lcd_home();
	}
	else
	{
		lcd_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcd_wr_char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd_wr_char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcd_wr_char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcd_wr_char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcd_wr_char(unit);
	}
	if(digits>5)
	{
		lcd_wr_char('E');
	}
	
}

void lcd_clear()
{
	lcd_wr_command(0x80);
	lcd_string("                                        ");
	lcd_wr_command(0xC0);
	lcd_string("                                        ");
	lcd_home();
}

//////////////////////////////////////////////////////////////////////////
int nodesnear[33][6]={
	{1,0,0,0,0,0},
	{2,0,0,0,0,0},
	{3,0,0,0,0,0},
	{4,0,0,0,0,0},
	{5,0,0,0,0,0},
	{6,0,0,0,0,0},
	{7,0,0,0,0,0},
	{8,0,0,0,0,0},
	{9,0,0,0,0,0},
	{10,0,0,0,0,0},
	{11,0,0,0,0,0},
	{12,0,0,0,0,0},
	{13,0,0,0,0,0},
	{14,0,0,0,0,0},
	{15,0,0,0,0,0},
	{16,0,0,0,0,0},
	{17,0,0,0,0,0},
	{18,0,0,0,0,0},
	{19,0,0,0,0,0},
	{20,0,0,0,0,0},
	{21,0,0,0,0,0},
	{22,0,0,0,0,0},
	{23,0,0,0,0,0},
	{24,0,0,0,0,0},
	{1,0,0,0,0,0},
	{25,36,43,44,0,0},
	{5,0,0,0,0,0},
	{9,0,0,0,0,0},
	{28,29,45,46,0,0},
	{13,0,0,0,0,0},
	{17,0,0,0,0,0},
	{32,33,47,48,0,0},
	{21,0,0,0,0,0},
};   // Nodes to which each and every note is connected

int noteangles[33][2]={
	{90,255},
	{75,240},
	{60,225},
	{45,210},
	{30,195},
	{15,180},
	{0,165},
	{345,150},
	{330,135},
	{315,120},
	{300,105},
	{285,90},
	{270,75},
	{255,60},
	{240,45},
	{225,30},
	{210,15},
	{195,0},
	{180,345},
	{165,330},
	{150,315},
	{135,300},
	{120,90},
	{105,285},
	{255,90},
	{240,60},
	{195,30},
	{135,330},
	{120,300},
	{75,270},
	{15,210},
	{0,180},
	{315,150},
};

int movToDestNote[33][4]={
	{1,0,0,0},
	{2,0,0,0},
	{4,0,0,0},
	{4,0,0,0},
	{5,0,0,0},
	{6,0,0,0},
	{8,0,0,0},
	{8,0,0,0},
	{9,0,0,0},
	{10,0,0,0},
	{12,0,0,0},
	{12,0,0,0},
	{13,0,0,0},
	{14,0,0,0},
	{16,0,0,0},
	{16,0,0,0},
	{17,0,0,0},
	{18,0,0,0},
	{20,0,0,0},
	{20,0,0,0},
	{21,0,0,0},
	{22,0,0,0},
	{24,0,0,0},
	{24,0,0,0},
	{1,0,0,0},
	{36,25,44,43},
	{5,0,0,0},
	{9,0,0,0},
	{13,0,0,0},
	{17,0,0,0},
	{33,32,48,47},
	{21,0,0,0},
};

int angle[48][4], node[48][4];
void declare_constants()
{
	

	angle[0][0]=75;
	angle[0][1]=285;
	angle[0][2]=-1;
	angle[0][3]=-1;

	angle[1][0]=255;
	angle[1][1]=60;
	angle[1][2]=-1;
	angle[1][3]=-1;

	angle[2][0]=240;
	angle[2][1]=45;
	angle[2][2]=0;
	angle[2][3]=300;

	angle[3][0]=225;
	angle[3][1]=30;
	angle[3][2]=-1;
	angle[3][3]=-1;

	angle[4][0]=210;
	angle[4][1]=15;
	angle[4][2]=-1;
	angle[4][3]=-1;

	angle[5][0]=195;
	angle[5][1]=0;
	angle[5][2]=-1;
	angle[5][3]=-1;

	angle[6][0]=180;
	angle[6][1]=345;
	angle[6][2]=240;
	angle[6][3]=300;

	angle[7][0]=165;
	angle[7][1]=330;
	angle[7][2]=-1;
	angle[7][3]=-1;

	angle[8][0]=150;
	angle[8][1]=315;
	angle[8][2]=-1;
	angle[8][3]=-1;

	angle[9][0]=135;
	angle[9][1]=300;
	angle[9][2]=-1;
	angle[9][3]=-1;

	angle[10][0]=120;
	angle[10][1]=285;
	angle[10][2]=180;
	angle[10][3]=240;

	angle[11][0]=105;
	angle[11][1]=270;
	angle[11][2]=-1;
	angle[11][3]=-1;

	angle[12][0]=90;
	angle[12][1]=255;
	angle[12][2]=-1;
	angle[12][3]=-1;

	angle[13][0]=75;
	angle[13][1]=240;
	angle[13][2]=-1;
	angle[13][3]=-1;

	angle[14][0]=60;
	angle[14][1]=225;
	angle[14][2]=120;
	angle[14][3]=180;

	angle[15][0]=45;
	angle[15][1]=210;
	angle[15][2]=-1;
	angle[15][3]=-1;

	angle[16][0]=30;
	angle[16][1]=195;
	angle[16][2]=-1;
	angle[16][3]=-1;

	angle[17][0]=15;
	angle[17][1]=180;
	angle[17][2]=-1;
	angle[17][3]=-1;

	angle[18][0]=0;
	angle[18][1]=165;
	angle[18][2]=60;
	angle[18][3]=120;

	angle[19][0]=345;
	angle[19][1]=150;
	angle[19][2]=-1;
	angle[19][3]=-1;

	angle[20][0]=330;
	angle[20][1]=135;
	angle[20][2]=-1;
	angle[20][3]=-1;

	angle[21][0]=315;
	angle[21][1]=120;
	angle[21][2]=-1;
	angle[21][3]=-1;

	angle[22][0]=300;
	angle[22][1]=105;
	angle[22][2]=0;
	angle[22][3]=60;

	angle[23][0]=90;
	angle[23][1]=285;
	angle[23][2]=-1;
	angle[23][3]=-1;

	angle[24][0]=180;
	angle[24][1]=0;
	angle[24][2]=240;
	angle[24][3]=-1;

	angle[25][0]=180;
	angle[25][1]=0;
	angle[25][2]=60;
	angle[25][3]=300;

	angle[26][0]=180;
	angle[26][1]=0;
	angle[26][2]=120;
	angle[26][3]=240;

	angle[27][0]=0;
	angle[27][1]=180;
	angle[27][2]=300;
	angle[27][3]=-1;

	angle[28][0]=60;
	angle[28][1]=120;
	angle[28][2]=240;
	angle[28][3]=-1;

	angle[29][0]=60;
	angle[29][1]=240;
	angle[29][2]=300;
	angle[29][3]=180;

	angle[30][0]=60;
	angle[30][1]=240;
	angle[30][2]=0;
	angle[30][3]=120;

	angle[31][0]=240;
	angle[31][1]=60;
	angle[31][2]=180;
	angle[31][3]=-1;

	angle[32][0]=300;
	angle[32][1]=0;
	angle[32][2]=120;
	angle[32][3]=-1;

	angle[33][0]=300;
	angle[33][1]=120;
	angle[33][2]=180;
	angle[33][3]=60;

	angle[34][0]=300;
	angle[34][1]=120;
	angle[34][2]=240;
	angle[34][3]=0;

	angle[35][0]=120;
	angle[35][1]=60;
	angle[35][2]=300;
	angle[35][3]=-1;

	angle[36][0]=60;
	angle[36][1]=240;
	angle[36][2]=-1;
	angle[36][3]=-1;

	angle[37][0]=120;
	angle[37][1]=300;
	angle[37][2]=-1;
	angle[37][3]=-1;

	angle[38][0]=300;
	angle[38][1]=120;
	angle[38][2]=-1;
	angle[38][3]=-1;

	angle[39][0]=0;
	angle[39][1]=180;
	angle[39][2]=-1;
	angle[39][3]=-1;

	angle[40][0]=180;
	angle[40][1]=0;
	angle[40][2]=-1;
	angle[40][3]=-1;

	angle[41][0]=240;
	angle[41][1]=60;
	angle[41][2]=-1;
	angle[41][3]=-1;

	angle[42][0]=180;
	angle[42][1]=60;
	angle[42][2]=-1;
	angle[42][3]=-1;

	angle[43][0]=120;
	angle[43][1]=240;
	angle[43][2]=-1;
	angle[43][3]=-1;

	angle[44][0]=60;
	angle[44][1]=300;
	angle[44][2]=-1;
	angle[44][3]=-1;

	angle[45][0]=0;
	angle[45][1]=120;
	angle[45][2]=-1;
	angle[45][3]=-1;

	angle[46][0]=300;
	angle[46][1]=180;
	angle[46][2]=-1;
	angle[46][3]=-1;

	angle[47][0]=240;
	angle[47][1]=0;
	angle[47][2]=-1;
	angle[47][3]=-1;

	node[0][0]=2;
	node[0][1]=24;
	node[0][2]=0;
	node[0][3]=0;

	node[1][0]=1;
	node[1][1]=3;
	node[1][2]=0;
	node[1][3]=0;

	node[2][0]=2;
	node[2][1]=4;
	node[2][2]=25;
	node[2][3]=36;

	node[3][0]=3;
	node[3][1]=5;
	node[3][2]=0;
	node[3][3]=0;

	node[4][0]=4;
	node[4][1]=6;
	node[4][2]=0;
	node[4][3]=0;

	node[5][0]=5;
	node[5][1]=7;
	node[5][2]=0;
	node[5][3]=0;

	node[6][0]=6;
	node[6][1]=8;
	node[6][2]=37;
	node[6][3]=38;

	node[7][0]=7;
	node[7][1]=9;
	node[7][2]=0;
	node[7][3]=0;

	node[8][0]=8;
	node[8][1]=10;
	node[8][2]=0;
	node[8][3]=0;

	node[9][0]=9;
	node[9][1]=11;
	node[9][2]=0;
	node[9][3]=0;

	node[10][0]=10;
	node[10][1]=12;
	node[10][2]=28;
	node[10][3]=29;

	node[11][0]=11;
	node[11][1]=13;
	node[11][2]=0;
	node[11][3]=0;

	node[12][0]=12;
	node[12][1]=14;
	node[12][2]=0;
	node[12][3]=0;

	node[13][0]=13;
	node[13][1]=15;
	node[13][2]=0;
	node[13][3]=0;

	node[14][0]=14;
	node[14][1]=16;
	node[14][2]=39;
	node[14][3]=40;

	node[15][0]=15;
	node[15][1]=17;
	node[15][2]=0;
	node[15][3]=0;

	node[16][0]=16;
	node[16][1]=18;
	node[16][2]=0;
	node[16][3]=0;

	node[17][0]=17;
	node[17][1]=19;
	node[17][2]=0;
	node[17][3]=0;

	node[18][0]=18;
	node[18][1]=20;
	node[18][2]=32;
	node[18][3]=33;

	node[19][0]=19;
	node[19][1]=21;
	node[19][2]=0;
	node[19][3]=0;

	node[20][0]=20;
	node[20][1]=22;
	node[20][2]=0;
	node[20][3]=0;

	node[21][0]=21;
	node[21][1]=23;
	node[21][2]=0;
	node[21][3]=0;

	node[22][0]=22;
	node[22][1]=24;
	node[22][2]=41;
	node[22][3]=42;

	node[23][0]=1;
	node[23][1]=23;
	node[23][2]=0;
	node[23][3]=0;

	node[24][0]=3;
	node[24][1]=26;
	node[24][2]=36;
	node[24][3]=0;

	node[25][0]=25;
	node[25][1]=27;
	node[25][2]=37;
	node[25][3]=44;

	node[26][0]=26;
	node[26][1]=28;
	node[26][2]=38;
	node[26][3]=45;

	node[27][0]=11;
	node[27][1]=27;
	node[27][2]=29;
	node[27][3]=0;

	node[28][0]=11;
	node[28][1]=28;
	node[28][2]=30;
	node[28][3]=0;

	node[29][0]=29;
	node[29][1]=31;
	node[29][2]=39;
	node[29][3]=46;

	node[30][0]=30;
	node[30][1]=32;
	node[30][2]=40;
	node[30][3]=47;

	node[31][0]=19;
	node[31][1]=31;
	node[31][2]=33;
	node[31][3]=0;

	node[32][0]=19;
	node[32][1]=32;
	node[32][2]=34;
	node[32][3]=0;

	node[33][0]=33;
	node[33][1]=35;
	node[33][2]=41;
	node[33][3]=48;

	node[34][0]=34;
	node[34][1]=36;
	node[34][2]=42;
	node[34][3]=43;

	node[35][0]=3;
	node[35][1]=25;
	node[35][2]=35;
	node[35][3]=0;

	node[36][0]=7;
	node[36][1]=26;
	node[36][2]=0;
	node[36][3]=0;

	node[37][0]=7;
	node[37][1]=27;
	node[37][2]=0;
	node[37][3]=0;

	node[38][0]=15;
	node[38][1]=30;
	node[38][2]=0;
	node[38][3]=0;

	node[39][0]=15;
	node[39][1]=31;
	node[39][2]=0;
	node[39][3]=0;

	node[40][0]=23;
	node[40][1]=34;
	node[40][2]=0;
	node[40][3]=0;

	node[41][0]=23;
	node[41][1]=35;
	node[41][2]=0;
	node[41][3]=0;

	node[42][0]=35;
	node[42][1]=44;
	node[42][2]=0;
	node[42][3]=0;

	node[43][0]=26;
	node[43][1]=43;
	node[43][2]=0;
	node[43][3]=0;

	node[44][0]=27;
	node[44][1]=46;
	node[44][2]=0;
	node[44][3]=0;

	node[45][0]=30;
	node[45][1]=45;
	node[45][2]=0;
	node[45][3]=0;

	node[46][0]=31;
	node[46][1]=48;
	node[46][2]=0;
	node[46][3]=0;

	node[47][0]=34;
	node[47][1]=47;
	node[47][2]=0;
	node[47][3]=0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//    COMM LAYER

#define MAX_NOTES 20

//Comm commands
#define RC_NOT_S 1		//Send before starting to tx notes
#define RC_NOT_E 2		//Send on end of tx of notes
#define ADD_TASK 3		//ADD_TASK[15:8] | TASK_NO[7:0]
#define NODE_RCH 4		//NODE_RCH[15:8] | TASK_NO[7:0]
#define NXT_NODE 5		//NXT_NODE[15:8] | NODE[7:0]
#define NOTE_STK 6		//NOTE_STK[15:8] | NODE[7:0]
#define PRC_NOTE 7		//PRC_NOTE[15:8] | TASK[7:0]
#define ADD_COST 8		//ADD_COST[15:8] | TASKCOST[7:0]
#define LAST_IND 9		//LAST_IND[15:8] | LASTINDEX[7:0]
#define END_TASK 10		//END_TASK[15:8] | 0x00

//Data can have at max 7 bits (0 - 127)
//8th bit should be 1 to denote a data
//Use this to convert any byte to a Data byte for Zigbee transmission
#define DATA(a) a | 0x80

#define BACKTODAT(a) a & 0x7F

char volatile notes[MAX_NOTES];
char volatile noteCount = 0;
char volatile noteToStrike = 0;
char volatile noteToProcess = 0;

char volatile tasks[MAX_NOTES];
char volatile taskCount = 0;
char volatile taskCost = 0;

char volatile otherBotTaskCost = 0;

char volatile notesReceived = 0;

int otherBotTask[MAX_NOTES]={};
int	otherBotTaskCount=0;
int botLastIndex = 1;

#ifdef MASTER
char volatile otherBotLastTaskIndex=13;
int volatile botloc=1;
char volatile otherBotLocation = 13;
char volatile otherBotMovingTo = 13;
#else
char volatile otherBotLastTaskIndex=1;
int  volatile botloc=13;
char volatile otherBotLocation = 1;
char volatile otherBotMovingTo = 1;
#endif


char volatile lastCommand = -1;

int volatile botEnd = 0;

//ZigBee
ISR(USART0_RX_vect)
{
	char data = UDR0;
	
	//check if data or command
	if(data & 0x80)
	{
		//data
		data=BACKTODAT(data);
		switch(lastCommand)
		{
			case RC_NOT_S:
			notes[(int)noteCount] = data;
			noteCount++;
			PORTJ = noteCount;
			break;
			
			case ADD_TASK:
			tasks[(int)taskCount] = data;
			taskCount++;
			break;
			
			case NODE_RCH:
			otherBotLocation = data;
			break;
			
			case NXT_NODE:
			otherBotMovingTo = data;
			break;
			
			case NOTE_STK:
			if(noteToStrike == data)
			noteToStrike++;
			break;
			
			case PRC_NOTE:
			if(noteToProcess == data)
			noteToProcess++;
			break;
			
			case ADD_COST:
			taskCost+=data;
			break;
			
			case LAST_IND:
			otherBotLastTaskIndex=data;
			break;
			
			case END_TASK:
			botEnd=1;
			break;
			
		}
	}
	else
	{	//command
		lastCommand = data;
		switch(data)
		{
			case RC_NOT_E:
			notesReceived = 1;
			break;
		}
	}
}

#ifdef MASTER
//USB UART - PC
ISR(USART2_RX_vect)
{
	char data = UDR2;
	//indicate end of notes by \n
	if(!(data == 255))
	{
		//is a note
		notes[(int)noteCount] = data;
		noteCount++;
		PORTJ = noteCount;
	}
	else
	{
		//all notes received
		//UDR2 = 'D';
		//while(!(UCSR2A & (1 << UDRE2)));
		//UDR2 = '\n';
		PORTJ = 0;
		notesReceived = 1;
	}
}
#endif

//Send a single byte through ZigBee
void inline SendByteToSlave(unsigned char data)
{
	//wait till transmitter is ready
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}



//send this task to slave
void inline SendTaskToSlave(char task)
{
	SendByteToSlave(ADD_TASK);
	SendByteToSlave(DATA(task));
	otherBotLastTaskIndex=task;
}

//Inform that a node has been reached
void inline SendNodeReached(char node)
{
	SendByteToSlave(NODE_RCH);
	SendByteToSlave(DATA(node));
}

//Inform about the next node
void inline SendNextNode(char node)
{
	SendByteToSlave(NXT_NODE);
	SendByteToSlave(DATA(node));
}

//Inform that a note has been struck
void inline SendNoteStruck(char noteIndex)
{
	SendByteToSlave(NOTE_STK);
	SendByteToSlave(DATA(noteIndex));
	noteToStrike++;
}

//Inform that a note has been processed
void inline NoteProcessed(char noteIndex)
{
	SendByteToSlave(PRC_NOTE);
	SendByteToSlave(DATA(noteIndex));
	noteToProcess++;
}

// To Send the Cost of the task for the other robot.
void inline SendTaskCost(char taskcost)
{
	SendByteToSlave(ADD_COST);
	SendByteToSlave(DATA(taskcost));
}

// Inform the LastTaskIndex to the Other Bot
void inline SendLastIndex(char lastIndex)
{
	SendByteToSlave(LAST_IND);
	SendByteToSlave(DATA(lastIndex));
}

// To Inform that Bot Completed all its Tasks
void inline BotEnded()
{
	SendByteToSlave(END_TASK);
	SendByteToSlave(DATA(0));
}

// Debugging
void inline Debug(char * data)
{
	int i=0;
	while(*(data+i)!='\0')
	{
		while(!(UCSR2A & (1<<UDRE2)));
		UDR2 = *(data + i);
		i++;
	}
	while(!(UCSR2A & (1<<UDRE2)));
	UDR2 = '\n';
}

#ifdef MASTER
//send data to slave through ZigBee
void inline SendNotesToSlave()
{
	SendByteToSlave(RC_NOT_S);
	//lcd_string("Start");
	//_delay_ms(300);
	int i;
	for(i = 0; i<noteCount; i++)
	{
		SendByteToSlave(DATA(notes[i]));
		//lcd_print(1,1,(int)notes[i],3);
		//_delay_ms(300);
	}
	SendByteToSlave(RC_NOT_E);
	//_delay_ms(1000);
}
void inline SendTaskArray()
{
	int i;
	for(i = 0; i<otherBotTaskCount; i++)
	{
		SendTaskToSlave(otherBotTask[i]);
	}
}

#endif



/*
Function To Initialize UART2 - PC - USB
desired baud rate:9600
actual baud rate:9600 (error 0.0%)
char size: 8 bit
parity: Disabled*/
void inline uart2_init(void)
{
	UCSR2B = 0x00; //disable while setting baud rate
	UCSR2A = 0x00;
	UCSR2C = 0x06;
	UBRR2L = 0x5F; //set baud rate lo
	UBRR2H = 0x00; //set baud rate hi
	UCSR2B = 0x98;
}

/*Function To Initialize UART0 - Zigbee
desired baud rate:9600
actual baud rate:9600 (error 0.0%)
char size: 8 bit
parity: Disabled */
void inline uart0_init(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

int thresh=35; // Theshold Values for IR sensors
int node[48][4]; // Possible node connections for each and every node
int angle[48][4];   // Angle of orientation of a node from a paritcular node with respect to the xy plane
int cost[48];
int botang=90;

int speed=100;
int strike=0;

int taskitr=0;  // Task Iterating Variable

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned int Degrees; //to accept angle in degrees for turning

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned char Front_IR_Sensor = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
	DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
	PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//TIMER1 initialization in 10 bit fast PWM mode  
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 52.25Hz 
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 //OCR1BH = 0x03;	//Output compare Register high value for servo 2
 //OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 //OCR1CH = 0x03;	//Output compare Register high value for servo 3
 //OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}

//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)
{
	float PositionPanServo = 0;
	PositionPanServo = ((float)degrees / 1.86) + 35.0;
	OCR1AH = 0x00;
	OCR1AL = (unsigned char) PositionPanServo;
}

void servo_1_free (void) //makes servo 1 free rotating
{
	OCR1AH = 0x03;
	OCR1AL = 0xFF; //Servo 1 off
}



void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08; //Setting PORTC 3 as output
	PORTC = PORTC & 0xF7; //Setting PORTC 3 logic low to turnoff buzzer
}

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}
void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}

void timer5_init();
void velocity(unsigned char, unsigned char);

//ADC pin configuration
void adc_pin_config (void)
{
	DDRF = 0x00;
	PORTF = 0x00;
	DDRK = 0x00;
	PORTK = 0x00;
}

void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

//Function to initialize ports
void port_init()
{
	motion_pin_config(); //robot motion pins config
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
	lcd_port_config();
	adc_pin_config();
	buzzer_pin_config();
}


// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
	For Overriding normal port functionality to OCRnA outputs.
	{WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
void print_sensor(char row, char coloumn,unsigned char channel)
{
	
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}


//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 		// removing upper nibbel for the protection
	PortARestore = PORTA; 		// reading the PORTA original status
	PortARestore &= 0xF0; 		// making lower direction nibbel to 0
	PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
	PORTA = PortARestore; 		// executing the command
}

void forward (void) //both wheels forward
{
	motion_set(0x06);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void stop (void)
{
	motion_set(0x00);
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;

	while (1)
	{
		//lcd_print(1,5,ShaftCountRight,3);
		//lcd_print(2,10,ShaftCountRight,2);
		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
		break;
	}
	//stop(); //Stop robot
	stop();
}



void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	velocity(255,255);
	angle_rotate(Degrees);
}

void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	right(); //Turn right
	velocity(255,255);
	angle_rotate(Degrees);
}

void LED_bargraph_config (void)
{
	DDRJ = 0xFF;  //PORT J is configured as output
	PORTJ = 0x00; //Output is set to 0
}

void init_devices()
{
	cli(); //Clears the global interrupt
	port_init();  //Initializes all the ports
	uart0_init();	//init Zigbee comm
	uart2_init();	//init PC - USB comm
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	adc_init();
	timer5_init();
	lcd_set_4bit();
	lcd_init();
	servo1_pin_config();
	timer1_init();
	LED_bargraph_config();
	sei();   // Enables the global interrupt
}


// Linear Distance mm.
//Function used for moving robot forward by specified distance
void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;
	ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
	ShaftCountRight = 0;
	while(1)
	{
		if(ShaftCountRight > ReqdShaftCountInt)
		{
			break;
		}
	}
	stop(); //Stop robot
}


void forward_mm(unsigned int DistanceInMM)
{
	forward();
	velocity(speed,speed);
	linear_distance_mm(DistanceInMM);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void costplan(int note_loc[])    // Cost planning Function
{								//  Plans cost of moving from the every node in the arena to the destination nodes
	int j=0,k=-1;
	int list[48];
	int tmploc[48]={0};
	for(j=0;j<48;j=j+1)  // Initialization
	{
		cost[j]=99;
		list[j]=-1;
		tmploc[j]=0;
	}
	for(j=0;j<6;j=j+1)  // Assigns cost of dest as Zero
	{
		if(note_loc[j]!=0)
		{
			cost[note_loc[j]-1]=0;
			k=k+1;
			list[k]=note_loc[j];   // It is the list of nodes to explore
			tmploc[note_loc[j]-1]=1;  // each node added to the list is marked with 1
		}
	}
	
	int i;
	
	int s;
	//int e=end-1;
	do
	{
		s=list[0]-1;	// extracts a node from the list and starts exploring its adjacent nodes
		for(i=0;i<=k;i=i+1) // left shift the list by 1
		{
			list[i]=list[i+1];
		}
		k=k-1;
		for(i=0;i<4;i=i+1)  // Starts exploring the node if the adjacent nodes.
		{
			if(node[s][i]!=0)
			{
				
				if((cost[s]+1)<=cost[node[s][i]-1])
				cost[node[s][i]-1]=(cost[s]+1);
				if(tmploc[node[s][i]-1]!=1)
				{
					tmploc[node[s][i]-1]=1;
					k=k+1;
					list[k]=node[s][i];
				}
				
			}
		}
	}while(list[0]!=-1 || k!=-1);
}

void rotate(int turnang)
{
	int f=0;
	if(turnang<0)
	{
		f=1;
		turnang=-turnang;
		right_degrees((turnang-30));
	}
	else
	{
		left_degrees((turnang-30));
	}
	_delay_ms(200);
	forward();
	if(f==0)
		velocity(0,50);
	else
		velocity(50,0);
	while(1)
	{
			
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1,5,2);
		if(Center_white_line>thresh || Left_white_line>thresh || Right_white_line>thresh)
		{	
			stop();
			break;
		}
		
		
	}
	//velocity(0,0);
}

int move(int n)
{
	velocity(0,0);
	int suc=1,turnang;
	turnang=angle[botloc-1][n]-botang;
	if(fabs(turnang)>45)
	{
		forward_mm(70);
		rotate(turnang);
	}
	botang=angle[botloc-1][n];
	
	if(strike==1)
	{
		if(noteangles[(int)notes[(int)noteToStrike]-1][0]==botang)
			servo_1(0);//Strike Left
		else
			servo_1(180);//Strike Right
		
		// Servo Motor Control
		// Strike the Note
		_delay_ms(300);
		servo_1(90);
		strike=0;
	}
	
	forward();
	int flag=0;
	ShaftCountRight=0;
	while(1)
	{
		flag=0;
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		Front_IR_Sensor = ADC_Conversion(6);
		
		lcd_print(1,1,(int)Left_white_line,3);	//Prints value of White Line Sensor1
		lcd_print(1,5,(int)Center_white_line,3);	//Prints Value of White Line Sensor2
		lcd_print(1,9,(int)Right_white_line,3);	//Prints Value of White Line Sensor3
		lcd_print(1,12,(int)Front_IR_Sensor,3);	//Front ir sensor Value
		
		
		if(Front_IR_Sensor>0xff)
		{
			rotate(180);
			for(int j=0;j<4;j++)
				if(node[node[botloc-1][n]-1][j]==botloc)
				{
					botang=angle[node[botloc-1][n]-1][j];
					break;
				}
			suc=0;
			flag=1;
		}
		if(Center_white_line>thresh && (Left_white_line>thresh || Right_white_line>thresh) && ShaftCountRight>20 && flag==0)
		{
			forward();
			velocity(0,0);
			lcd_print(2,1,100,3);
			break;
		}
		else if(Left_white_line>thresh)
		{
			//flag=1;
			forward();
			lcd_print(2,1,1,3);
			velocity(0,speed);
		}
		else if(Right_white_line>thresh)
		{
			//flag=1;
			forward();
			lcd_print(2,1,2,3);
			velocity(speed,0);
		}
		else if(Center_white_line>thresh)
		{
			forward();
			velocity(speed,speed);
			lcd_print(2,1,0,3);
			//flag=1;
		}
		else
		{
			velocity(speed,speed);
		}
	}
	return suc;
}
int LastLocation;
int BotEndLocation(int loc)
{
	int i;
	if (cost[loc - 1] == 0)
	{
		LastLocation = loc;
		return LastLocation;
	}
	int mCost = cost[node[loc - 1][0] - 1];
	int pos = 0;
	for (i = 1; i<4; i++)
	{
		if (node[loc - 1][i]!=0)
			if (cost[node[loc - 1][i] - 1]<mCost)
			{
				mCost = cost[node[loc - 1][i] - 1];
				pos = i;
			}
	}
	return BotEndLocation(node[loc - 1][pos]);
}

////////////////////////////////////////////////////////
//PC Debug
uart_putchar(char c, FILE *stream)
{
	//if(c=='\n')
	//uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR2A, UDRE2);
	UDR2 = c;
}

FILE uart_debug = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
///////////////////////////////////////////////////////


void processNotes()
{
	int noteToProcess=0;
	while(noteToProcess!=noteCount)
	{
		costplan(nodesnear[(int)notes[(int)noteToProcess]-1]);
		if((taskCost+cost[botLastIndex-1])>((int)otherBotTaskCost+cost[(int)otherBotLastTaskIndex-1]))
		{
			otherBotTaskCost+=cost[otherBotLastTaskIndex-1];
			otherBotLastTaskIndex=BotEndLocation((int)otherBotLastTaskIndex);
			otherBotTask[otherBotTaskCount]=noteToProcess;
			otherBotTaskCount++;
		}
		else
		{
			taskCost+=cost[botLastIndex-1];
			botLastIndex=BotEndLocation(botLastIndex);
			tasks[(int)taskCount]=noteToProcess;
			taskCount++;
		}
		noteToProcess++;
		
	}
	
}


int main()
{
	declare_constants();
	init_devices();
	servo_1(90);
	PORTJ = 0x00;
	notesReceived = 0;
	lcd_string("Waiting...");
	while(!notesReceived);
	lcd_clear();
	lcd_string("Received!");
	PORTJ = 0xFF;
	
	#ifdef MASTER
	SendNotesToSlave();
	PORTJ = 0x0F;
	lcd_clear();
	lcd_string("Sent!");
	lcd_clear();
	lcd_string("Sending Tasks");
	processNotes();
	lcd_clear();
	lcd_string("Processed");
	SendTaskArray();
	lcd_clear();
	lcd_string("Task Sent");
	#endif

	while(taskCount==0);
	
	//PC Debug (printf)
	stdout = &uart_debug;
		
	int i=0;
	for(i=0;i<noteCount;i++)
	{
		lcd_print(2,1,(int)notes[i],2);
		_delay_ms(1000);
	}
	lcd_clear();
	
	while(notes[(int)noteToStrike]!=0 && taskitr!=taskCount)
	{	
		printf("Current Task Node=%d \n",(int)notes[(int)tasks[taskitr]]);
		int taskDone=0;
		int i,minCost,nxtNode,pos;
		costplan(nodesnear[(int)notes[(int)tasks[taskitr]]-1]);
		lcd_print(2,12,(int)notes[(int)tasks[taskitr]],2);
		printf("Cost Planned for task =%d Cost=%d",(int)tasks[taskitr],cost[botloc-1]);
		while(cost[botloc-1]!=0)
		{
			lcd_print(2,5,botloc,2);
			minCost=cost[node[botloc-1][0]-1];
			nxtNode=node[botloc-1][0];
			pos=0;
			for(i=1;i<4;i++)
			{
				if(node[botloc-1][i]!=0)
					if(cost[node[botloc-1][i]-1]<minCost)
					{
						minCost=cost[node[botloc-1][i]-1];
						nxtNode=node[botloc-1][i];
						pos=i;
					}
			}
			lcd_print(2,8,nxtNode,2);
			while((int)otherBotMovingTo==nxtNode);
			SendNextNode(nxtNode);
			//while(otherBotLocation==nxtNode);
			taskDone=move(pos);
			if(taskDone==0)
			{
				for(i=0;i<4;i++)
					if(node[node[botloc-1][pos]-1][i]==nxtNode)
						node[node[botloc-1][pos]-1][i]=0;
				node[botloc-1][pos]=0;
				break;
			}
			else
			{
				botloc=node[botloc-1][pos];
				SendNodeReached(botloc);
			}
		}
			
		if(cost[botloc-1]==0)
		{
			while(noteToStrike!=tasks[taskitr]);
			if(noteangles[(int)notes[(int)noteToStrike]-1][0]==botang || noteangles[(int)notes[(int)noteToStrike]-1][1]==botang)
			{
				if(noteangles[(int)notes[(int)noteToStrike]-1][0]==botang)
					servo_1(0); // Strike Left
				else
					servo_1(180);//Strike Right
					
				_delay_ms(300);
				servo_1(90);
				// Servo Motor Control
				// Strike the Note
			}
			else
			{
				int tpos=0,p=0;
				for(int j=0;j<4;j++)
				{
					if(nodesnear[(int)notes[(int)noteToStrike]-1][j]!=0 && botloc==nodesnear[(int)notes[(int)noteToStrike]-1][j])
					{
						tpos=j;
						break;
					}
				}
				for(int j=0;j<4;j++)
				{
					if(node[botloc-1][j]==movToDestNote[(int)notes[(int)noteToStrike]-1][tpos])
					{
						p=j;
						break;
					}
				}
				strike=1;
				taskDone=move(p);
				if(taskDone==1)
				{
					botloc=node[botloc-1][p];
					SendNodeReached(botloc);
				}
			}
			printf("Reached Destination node=%d \n",(int)notes[(int)tasks[taskitr]]);
			buzzer_on();
			_delay_ms(500);
			buzzer_off();
			taskitr++;
			SendNoteStruck(noteToStrike);
		}
	}
	printf("Gonna be Out!!!\n");
	BotEnded();
	while(botEnd!=1);
	buzzer_on();
	_delay_ms(6000);
	buzzer_off();
	return 0;
}



