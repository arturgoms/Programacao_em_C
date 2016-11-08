#include"HeartBeat.h"
#include"RaspberryGPIO.h"
#include<string.h>
#define NSAMPLES 10
char sReport[8][15]={"LOW PULSE!!!","Excellent!","Very Good!","Good!","Above Average!","Average!","Below Average!","TOO HIGH!!!"};

unsigned long getTime(void){
	/*	Função deve ser portada para a Biblioteca "RaspberryGPIO.h", essa Função depende da Biblioteca <sys/time.h>	*/
	struct timeval time;
	gettimeofday(&time,NULL);
	unsigned long long t = (1000*time.tv_sec)+(time.tv_usec/1000);
	return t;
}
bool healthProfile(struct sGENERAL *patient){
	system("clear");
	printf("\tCadastro:\n");
	char name[150];
	unsigned int age=0,sex = 0;
	printf("Nome:");
	scanf("%[^\n]s",name);
	strcpy(patient->Name,name);
	printf("\nIdade:");
	scanf("%d",&age);
	printf("\nGênero [1]Masculino-[2]Feminino:");
	do{scanf("%d",&sex);}while(sex > -1 && sex < 2);

	if(healthInit(sex,age,patient) == false) return false;

	FILE *patient_data = fopen("patient_data.bin","w+"); // Verificar Parâmetro
	if(patient_data == NULL){
		printf("File doesn't open!\n");
		return false;
	}
	fwrite(patient,sizeof(struct sGENERAL),1,patient_data);
}
bool healthInit(uint8_t iSex,uint8_t iAgeGroup,struct sGENERAL *patient){
	if(iSex == MALE){
		struct sMALE getData[6];
		FILE *fMale = fopen("data_male.bin","r");
		unsigned int iCount = 0;
		iAgeGroup = iAgeGroup - 1;
		if(fMale == NULL)	printf("File doesn't open!\n");
		while(!feof(fMale)){
			fread(&getData[iCount],sizeof(struct sMALE),1,fMale);
			iCount++;
		}
		patient->Excellent = getData[iAgeGroup].Excellent;
		patient->VerryGood = getData[iAgeGroup].VerryGood;
		patient->Good = getData[iAgeGroup].Good;
		patient->AboveAverage = getData[iAgeGroup].AboveAverage;
		patient->Average = getData[iAgeGroup].Average;
		patient->BelowAverage = getData[iAgeGroup].BelowAverage;
		patient->Bad = getData[iAgeGroup].Bad;
		patient->Sex = "MALE";
		patient->AgeGroup = iAgeGroup;
		return true;
	}
	else if(iSex == FEMALE){
		struct sFEMALE getData[6];
		FILE *fFemale = fopen("data_female.bin","r");
		unsigned int iCount = 0;
		if(fFemale == NULL)	printf("File doesn't open!\n");
		while(!feof(fFemale)){
			fread(&getData[iCount],sizeof(struct sFEMALE),1,fFemale);
			iCount++;
		}
		patient->Excellent = getData[iAgeGroup].Excellent;
		patient->VerryGood = getData[iAgeGroup].VerryGood;
		patient->Good = getData[iAgeGroup].Good;
		patient->AboveAverage = getData[iAgeGroup].AboveAverage;
		patient->Average = getData[iAgeGroup].Average;
		patient->BelowAverage = getData[iAgeGroup].BelowAverage;
		patient->Bad = getData[iAgeGroup].Bad;
		patient->Sex = "FEMALE";
		patient->AgeGroup = iAgeGroup;
		return true;
	}
	else return false;
}
char* healthState(struct sGENERAL person,unsigned int pulse){
	while (pulse < person.Excellent)	return sReport[0];
	while (pulse > person.Excellent-1 && pulse < person.VerryGood)	return sReport[1];
	while (pulse > person.VerryGood-1 && pulse < person.Good)	return sReport[2];
	while (pulse > person.Good-1 && pulse < person.AboveAverage)	return sReport[3];
	while (pulse > person.AboveAverage-1 && pulse < person.Average)	return sReport[4];
	while (pulse > person.Average-1 && pulse < person.BelowAverage)	return sReport[5];
	while (pulse > person.BelowAverage-1 && pulse < person.Bad)	return sReport[6];
	while (pulse > person.Bad-1)	return sReport[7];
}
unsigned int healthPulse(uint8_t pin){
	unsigned long int time[NSAMPLES],aux;
	unsigned int iCount=0,average[NSAMPLES-1],pulse;
	while(iCount < NSAMPLES){
		if(GPIORead(pin)==HIGH){
			time[iCount] = getTime();
			iCount++;
		}
	}
	aux = time[0];
	for(register unsigned iCount = 1 ; iCount < NSAMPLES ; iCount++){
		average[iCount-1] = aux - time[iCount];
		pulse = average[iCount-1]+pulse;
		aux = time[iCount];
	}
	return 60000/(pulse/(NSAMPLES-1));
}
bool importData(struct sGENERAL *patient){
	FILE *data = fopen("patient_data.bin","r");
	if(data == NULL){
		printf("There is no file(or data)!\n");
		return false;
	}
	uint8_t count=0;
	//while(!feof(data)){
		fread(patient,sizeof(struct sGENERAL),1,data);
		//count ++;
	//}
	if(count > 1)	return false;
	else	return true;
}
