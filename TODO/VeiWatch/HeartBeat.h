#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include <sys/time.h>
#define MALE 0
#define FEMALE 1
struct sMALE{
	uint8_t Excellent;
	uint8_t VerryGood;
	uint8_t Good;
	uint8_t AboveAverage;
	uint8_t Average;
	uint8_t BelowAverage;
	uint8_t Bad;
};
struct sFEMALE{
	uint8_t Excellent;
	uint8_t VerryGood;
	uint8_t Good;
	uint8_t AboveAverage;
	uint8_t Average;
	uint8_t BelowAverage;
	uint8_t Bad;
};
struct sGENERAL{
	uint8_t Excellent;
	uint8_t VerryGood;
	uint8_t Good;
	uint8_t AboveAverage;
	uint8_t Average;
	uint8_t BelowAverage;
	uint8_t Bad;
	char *Sex;
	uint8_t AgeGroup;
};

unsigned long getTime(void);
bool healthInit(uint8_t iSex,uint8_t iAgeGroup,struct sGENERAL *patient);
char* healthState(struct sGENERAL person,unsigned int pulse);
int extractData();
void healthExit(); // Will ends the programm and extract to a file the currently patient's data 
void healthReset_data();//Will flush the file that saves the currently patient's data

