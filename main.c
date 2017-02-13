#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////   Constants

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
{1,35,0,0,0,0},
{25,26,35,36,43,44},
{5,26,0,0,0,0},
{9,27,0,0,0,0},
{27,28,29,30,45,46},
{13,30,0,0,0,0},
{17,31,0,0,0,0},
{31,32,33,34,47,48},
{21,34,0,0,0,0},
};   // Nodes to which each and every note is connected

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

//Uncomment if this robot receives data from PC
//#define MASTER

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
char volatile noteToStrike = -1;
char volatile noteToProcess = -1;

char volatile tasks[MAX_NOTES];
char volatile taskCount = 0;
char volatile taskCost = 0;

char volatile otherBotLocation = -1;
char volatile otherBotMovingTo = -1;
char volatile otherBotTaskCost = 0;

#ifdef MASTER
	char volatile otherBotLastTaskIndex=13;
	int botloc=1;
#else
	char volatile otherBotLastTaskIndex=1;
	int botloc=13;
#endif
	

char volatile lastCommand = -1;

int botEnd = 0;

//status register bitfield
volatile struct
{
	unsigned char notesReceived	:1;
	unsigned char isWaiting		:1;
	
}status;

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
			status.notesReceived = 1;
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
	if(!(data == '\n')
	{
		//is a note
		notes[noteCount] = data;
		noteCount++;
	}
	else
	{
		//all notes received
		UDR2 = 'D';
		while(!(UCSR2A & (1 << UDRE2)));
		UDR2 = '\n';
		status.notesReceived = 1;
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

#ifdef MASTER
//send data to slave through ZigBee
void inline SendNotesToSlave()
{
	SendByteToSlave(RC_NOT_S);
	int i;
	for(i = 0; i<noteCount; i++)
	{
		int data = DATA(notes[i]);
		SendByteToSlave(DATA(data));
	}
	SendByteToSlave(RC_NOT_E);
}
#endif

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

int thresh=0x23; // Theshold Values for IR sensors
int node[48][4]; // Possible node connections for each and every node
int angle[48][4];   // Angle of orientation of a node from a paritcular node with respect to the xy plane
int cost[48];
int botang=90;


int taskitr=0;  // Task Iterating Variable

unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder 
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
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
//Function to configure LCD port
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

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
  motion_set(thresh);
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
  lcd_print(1,5,ShaftCountRight,3);
  //lcd_print(2,10,ShaftCountRight,2);
  if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
  break;
 }
 stop(); //Stop robot
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
	velocity(90,90);
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
	        tmploc[note_loc[j]-1]=1;  // each node which is added to the list is marked with 1
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
	}while(list[0]!=-1 && k!=-1);
}

void rotate(int turnang)
{
	if(turnang<0)
	{
		turnang=-turnang;
		right_degrees(turnang-25);
	}
	else
	{
		left_degrees(turnang-25);
	}
	while(1)
	{
		Center_white_line = ADC_Conversion(2);
		if(Center_white_line>0x78)
			break;
	}
}

int move(int n)
{
	int suc=1,turnang;
	turnang=angle[botloc-1][n]-botang;
	if(fabs(turnang)>45)
	{
		forward_mm(70);
		rotate(turnang);
	}
	botang=angle[botloc-1][n];
	forward();
	while(1)
	{
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		Front_IR_Sensor = ADC_Conversion(6);
		
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		
		ShaftCountRight=0;
		
		if(Front_IR_Sensor>0xf0)
		{
			rotate(180);
			botang=botang-180-turnang;
			if(botang<0)
				botang+=360;
			suc=0;
		}
		else if(Center_white_line>thresh && (Left_white_line>thresh || Right_white_line>thresh) && ShaftCountRight>10)
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
			velocity(0,90);
		}
		else if(Right_white_line>thresh)
		{
			//flag=1;
			forward();
			lcd_print(2,1,2,3);
			velocity(90,0);
		}
		else if(Center_white_line>0x78)
		{
			velocity(130,130);
			lcd_print(2,1,0,3);
			//flag=1;
		}
		else if(Center_white_line<thresh && Left_white_line<thresh && Right_white_line<thresh)
		{
			velocity(90,90);
		}
	}
	return suc;
}
int LastLocation;
int BotEndLocation(int loc)
{
	int i;
	if(cost[loc-1]==0)
	{
		LastLocation=loc;
		return LastLocation;
	}
	int mCost=cost[node[loc-1][0]];
	int pos=0;
	for(i=1;i<4;i++)
	{
		if(cost[node[loc-1][i]]<mCost)
		{
			mCost=cost[node[loc-1][i]];
			pos=i;
		}
	}
	return BotEndLocation(node[loc-1][pos]);
}

int main()
{
	declare_constants();
	init_devices();
	while(status.notesReceived);
	
	//send the notes to slave if this is the master
	#ifdef MASTER
	SendNotesToSlave();
	#endif
	
	while(notes[noteToStrike]!=0)
	{
		if(taskCount==taskitr)	// If the bot has traversed all Tasks in the list
		{
			costplan(nodesnear[(int)notes[(int)noteToProcess]-1]);	// Calculate cost for the next Note to process for the full map
			if(cost[botloc-1]>((int)otherBotTaskCost+cost[(int)otherBotLastTaskIndex])) // Give the Task to the Bot which has less Total Cost
			{
				SendTaskToSlave((int)noteToProcess);			// Send the Task to the other bot if it has less cost
				SendTaskCost(cost[(int)otherBotLastTaskIndex]);		// Send the cost for the processed task
				otherBotLastTaskIndex=BotEndLocation((int)otherBotLastTaskIndex);	// Update the other bot last index
			}
			else
			{
				tasks[taskCount]=(int)noteToProcess;	// If the bot has less cost than the other bot add the task to the list
				taskCost+=cost[botloc-1];		//
				taskCount++;
				SendLastIndex((int)noteToProcess);
				SendLastIndex((int)BotEndLocation(botloc));
			}
			NoteProcessed(noteToProcess);
		}
		else
		{	int taskDone=0;
			int i,minCost,nxtNode,pos;
			costplan(nodesnear[(int)notes[(int)tasks[taskitr]]-1]);
			if(cost[botloc-1]==0)
				taskDone=1;       // In Case the Bot is Already in the Dest Node.
			while(cost[botloc-1]!=0)
			{
				minCost=cost[node[botloc-1][0]];
				nxtNode=node[botloc-1][0];
				pos=0;
				for(i=1;i<4;i++)
				{
					if(node[botloc-1][i]!=0)
						if(cost[node[botloc-1][i]]<minCost)
						{
							minCost=cost[node[botloc-1][i]];
							nxtNode=node[botloc-1][i];
							pos=i;
						}
				}
				while(otherBotMovingTo==nxtNode);
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
				// Servo Motor Control
				// Strike the Note
				taskitr++;
				SendNoteStruck(noteToStrike);
			}
		}
		
	}
	BotEnded();
	while(botEnd!=1);
	buzzer_on();
	_delay_ms(6000);
	buzzer_off();
	return 0;
}



