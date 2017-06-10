#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/delay.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DEFAULT_SPEED 40000

int main(int argc, char * argv[]) {
	int dev;

	if(argc != 4) { //실행인자 예외처리
		printf("Usage : %s <direction:R,L> <repeat:N> <speed:N>\n", argv[0]);
		return -1;
	}
	dev = open("/dev/m2_led", O_WRONLY);

	if (dev != -1) { //실행 예외처리
		char buf[7]={0};
		char direction=argv[1][0];
		int repeat=atoi(argv[2]);
		int speed=atoi(argv[3]);
		int i,j;

		if(!(0 < repeat || repeat < 10)) { //반복 예외처리
			printf("The number of iterations is only from 1 to 9\n");
			return -1;
		}
		if(!(0 < speed || speed < 10)) { //속도 예외처리
			printf("The speed is only from 1 to 9\n");
			return -1;
		}
		for(i=0; i<repeat; i++) {
			if(direction=='R' || direction=='r') {
				for(j=0; j<7; j++) {
					buf[j]=1; //LED ON
					write(dev,buf,7);
					usleep(speed*DEFAULT_SPEED);
					buf[j]=0; //LED OFF
					write(dev,buf,7);
				}
			}
			else if(direction=='L' || direction=='l') {
				for(j=6; j>=0; j--) {
					buf[j]=1;
					write(dev,buf,7);
					usleep(speed*DEFAULT_SPEED);
					buf[j]=0;
					write(dev,buf,7);
				}
			}
			else { //방향 예외처리
				printf("Direction is only R and L\n");
				return -1;
			}
		}
	}
	else {
		printf( "Device Open ERROR!\n");
		return -1;
	}
	close(dev);

	return 0;
}
