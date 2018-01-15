#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1;
#define FALSE 0;
#define PATH "/dev/leds"

//PROTOTYPES
int writeLeds(int fileDesc, char *buff);
int menu();
void decToBinary();
void rotatory();

static int fd;

int main(int argc, const char *argv[]){
	int option;

	if ((fd = open(PATH, O_RDWR)) < 0){
		puts("[ERROR]: Error while opening device");
		return -1;
	}

	do{
		//RESET LEDS
		if (writeLeds(fd, " ") != 1){
			"[ERROR]: writing in file";
			return -1;
		}

		option = menu();
		if (option == 1)
			rotatory();
		else if(option == 2)
			decToBinary();

	}while (option != 0);

	close(fd);

	return 0;
}

int writeLeds(int fileDesc, char *buff){
	int bytesWrited;
	int bytesToCopy;

	bytesToCopy = strlen(buff);

	bytesWrited = write(fileDesc, buff, bytesToCopy);
	if (bytesWrited != bytesToCopy){
		puts("[ERROR]: writing in file");
		return -1;
	}

	return bytesWrited;
}

int menu(){
	int userOption;
	int validOption = FALSE;

	puts("Select one option (0-3)");
	puts("(0) Exit");
	puts("(1) Rotatory");
	puts("(2) Decimal to binary");

	while(!validOption){
		scanf("%i", &userOption);

		if (userOption >= 0 && userOption <= 2){
			validOption = TRUE;
		}else{
			puts("[ERROR]: Invalid option");
		}
	}

	return userOption;
}

void decToBinary(){
	int buff;
	int exit = FALSE;
	char *toPrint;

	do{
		puts("Enter one number between 1 and 7, other for exit");
		scanf("%i", &buff);

		if (buff == 1) toPrint = "3";
		else if (buff == 2) toPrint = "2";
		else if (buff == 3) toPrint = "23";
		else if (buff == 4) toPrint = "1";
		else if (buff == 5) toPrint = "13";
		else if (buff == 6) toPrint = "12";
		else if (buff == 7) toPrint = "123";
		else exit = TRUE;

		writeLeds(fd, toPrint);

	}while(!exit);
}

void rotatory(){
	int i,j;

	for (i = 0; i < 5; i++){
		for(j = 49; j < 52; j++){ //49 it's character 1 in ASCII table and 51 it's character 3.
			writeLeds(fd, (char *)&j);
			sleep(1);
			writeLeds(fd, " ");
		}
	}
}
