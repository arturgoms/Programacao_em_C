/*
				ASCII Art Raspberry Pi 1 GPIO
			              	  PINOS
					   +3V3 [01] [02] +5V
			 SDA1 / GPIO  2 [03] [04] +5V
			 SCL1 / GPIO  3 [05] [06] GND
   				    GPIO  4 [07] [08] GPIO 14 / TXD0
   			            GND [09] [10] GPIO 15 / RXD0
   				    GPIO 17 [11] [12] GPIO 18
					GPIO 27 [13] [14] GND
					GPIO 22 [15] [16] GPIO 23
					   +3V3 [17] [18] GPIO 24
		     MOSI / GPIO 10 [19] [20] GND
		     MISO / GPIO  9 [21] [22] GPIO 25
 			 SCLK / GPIO 11 [23] [24] GPIO  8 / CE0#	CE_NOKIA
					    GND [25] [26] GPIO  7 / CE1#	RST_NOKIA
*/
#include<stdio.h>
#include"wiringSerial.h"
#include"RaspberryGPIO.h"
#include"PCD8544.h"
#include"HeartBeat.h"
#include"Logo.h"

#include"SystemClock.h"
#include"VeiWatch.h"

/*Display NOKIA*/

#define RST 7
#define CE 8
#define DC 25
#define DIN 24
#define CLK 23

/*Buttons*/
#define backLight	17
#define changeDisplay 27
/*Output GPIO*/
#define BL 18
struct Data{
	unsigned int BPM;
	float Temp;
	char *BPMState,*TempState;
	
}Sensors;

#define contrast 50


/* comentarios codigo novo
pesquisar sobre concatenar string recebida na serial, pesquisar como criar as interrupcoes para os botoes do menu,
 fim comentarios */




void lcdDisplayMain(){
	/* Inicialização do Relógio do Sistema	*/
	char	Nokia_Temp[10],Nokia_BPM[10],INFO[]={"INFORMACOES OU ALGO DO TIPO"};
	int lenght = strlen(INFO);
	snprintf(Nokia_Temp,10,"%.1f*C",Sensors.Temp);
	snprintf(Nokia_BPM,10,"%dBPM",Sensors.BPM);	

	LCDclear();
	LCDdisplay();
	getClockInformation(&info);

	printf("Data:%s\n",info.date);
	printf("Hora:%s\n",info.time);
	printf("Temp:%.1f\n",Sensors.Temp);
	printf("BPM:%d\n",Sensors.BPM);

	if (stadoDisplay == 0) {
		LCDdrawstring(20,0,"PRINCIPAL");
		LCDdrawstring(0,13,Nokia_Temp);
		LCDdrawstring(50,13,Nokia_BPM);
		LCDdrawstring(0,26,info.date);
		LCDdrawstring(50,26,info.time);

		LCDdrawline(0, 35, 83, 35, BLACK);
		// informações
		LCDdisplay();
	}else{}
}

void lcdDisplayProfile(){
	if (stadoDisplay == -1) {

	}else{}
}
int lcdDisplaySensors(char *state_BPM, char *state_Temp){
	char Nokia_Temp[10],Nokia_BPM[10];	
	snprintf(Nokia_Temp,10,"%.1f*C",Sensors.Temp);
	snprintf(Nokia_BPM,10,"%dBPM",Sensors.BPM);	
	printf("Temp:%.1f\n",Sensors.Temp);
	printf("BPM:%d\n",Sensors.BPM);
	if (stadoDisplay==1) {

	LCDclear();
	LCDdrawstring(20,0,"SENSORES");
	LCDdrawstring(25,10,Nokia_Temp);
	LCDdrawstring(20,20,state_Temp);
	LCDdrawstring(25,30,Nokia_BPM);
	LCDdrawstring(20,40,state_BPM);

	LCDdisplay();
}else{}
}
const unsigned char SERIAL_PORT[2][30] = {"/dev/ttyAMA0","/dev/ttyUSB0"};

const unsigned int BAUDS[2] = {115200,9600};

int main(void){
	LCDInit(CLK, DIN, DC, CE,RST, contrast);
  //LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  	LCDclear();
	LCDshowLogo();
	struct sGENERAL person;
	delay_ms(1000);

	system("clear");
	printf("Deseja Sobrescrever os Dados ? S/N\n");
	int choose;
	do{choose = (int)getchar();}while(choose != 115 && choose != 83 && choose != 110 && choose != 78);
	getchar();
	if(choose == 115 || choose == 83){
		LCDDrawBitmap(profile);
		if(healthProfile(&person) == false){
			printf("Falha ao realizar o Cadastro!\n");
			return -1;
		}
	}else{
		system("clear");
		LCDDrawBitmap(profile);
		if(importData(&person)==false){
			printf("Falha ao importar dados!\n");
			return -1;
		}
	}
	LCDDrawBitmap(sensors);
	int raspDuino = serialOpen(SERIAL_PORT[1],BAUDS[1]);
	if(raspDuino == -1){
		printf("Houve um erro ao Abrir a porta Serial!\n");
		return -1;
	}

	serialFlush(raspDuino);
	
	GPIOExport(changeDisplay);	GPIODirection(changeDisplay,INPUT);
	GPIOExport(backLight);	GPIODirection(backLight,INPUT);
	GPIOExport(BL);	GPIODirection(BL,OUTPUT);

	uint8_t change_Layer = 0;
	uint8_t light_State = LOW;

	while(1){
		
		
		  main();
		  perfil();
		  sensores();

		  staterBtn = digitalRead(rBtn);
		  statelBtn = digitalRead(lBtn);

		  if(staterBtn==1 && lastStaterBtn==0)
		  {
		      if ( lol != 1){

		       lol = 1;
		     }
		     stadoDisplay++;
		  }
		  if(statelBtn==1 && lastStatelBtn==0)
		  {
		      if ( lol != 1){

		       lol = 1;
		     }
		     stadoDisplay--;
		  }

		lastStaterBtn=staterBtn;
		lastStateeBtn=stateeBtn;
		lastStatelBtn=statelBtn;

		}
		
		
		// Código antigo daqui pra baixo 
		
		if(GPIORead(changeDisplay) == HIGH){
			while(GPIORead(changeDisplay) == HIGH){}
			change_Layer++;
			if(change_Layer > 2)	change_Layer = 0;
		}
		if(GPIORead(backLight) == HIGH){
			while(GPIORead(backLight) == HIGH){}
			light_State = ! light_State;
			GPIOWrite(BL,light_State);
		}
		switch(change_Layer){
			case 0:
				lcdDisplayMain();
				break;
			case 1:
				lcdDisplayProfile();
				break;
			case 2:
				lcdDisplaySensors(Sensors.BPMState,Sensors.TempState);
		}
		if(serialDataAvail(raspDuino)!=-1){ // Usar Threads 
			Sensors.BPM = (unsigned int)serialGetchar(raspDuino);
			Sensors.Temp = ((float)serialGetchar(raspDuino)*5/(1023))/0.01;
			Sensors.BPMState = healthState(person,Sensors.BPM);
			Sensors.TempState = isNormal(Sensors.Temp);
		}
	}
}
