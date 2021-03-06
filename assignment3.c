/*  Control two motors. They need to stop, go forward, go backward, as well
    as speed control (Pulse Width Modulation). 
	You are to connect the motors to two channels on the Motor Shield.     */
#include <stdio.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>

struct Motor {
       int enable_motor;
       int forward_motor;
       int reverse_motor;
   };

void init(struct Motor *, int enable, int forward, int reverse);
void forward(struct Motor *, int );
void reverse(struct Motor *, int );
void stop(struct Motor *);

/* DC Motor 1 : enable-11 control-13,15
   DC Motor 2 : enable-22 control-16,18 

   by using PWM, you can set duty cycle of the pin to change the speed
   of the motors.

   the two input pins are connected to outputs
   the two outpin pins are connected directly to motor

   connect (-) of battery to the GND of the pi

   the two input pins give us a way to control the direction of the motor. 
   When both are set the same, the motor does not move. if one is high and one is low
   the motor will either spin clockwise or counterclockwise	
*/
  

 int main() {

	struct Motor m1;
	struct Motor m2;
    int speed_fast = 75;
    int speed_slow = 30;

	if(wiringPiSetup() < 0){
		printf("Initialization Failed.");
		return 1;
	} 

	else {

		printf("Initialization Successful\n");

			init(&m1, 0, 2, 3);
			init(&m2, 6, 4, 5);
		 	forward(&m1, speed_fast);
		 	forward(&m2, speed_fast);
		 	sleep(3);
            stop(&m1);
		 	stop(&m2);
		 	reverse(&m1, speed_slow);
		 	reverse(&m2, speed_slow);
            sleep(3);
		 	stop(&m1);
		 	stop(&m2);

 		return 0;
	}
}


void init(struct Motor * motors, int enable, int forward, int reverse){

//	printf("Initializating motor %d: \n", motors);
	motors->enable_motor = enable;
	motors->forward_motor = forward;
	motors->reverse_motor = reverse;

	/*final param says how long each cycle is in basic pulse units
		
		int softPwnCreate (int pin, int initValue, int pwmRange);
			creates software controlled PWM pin. Use 100 for the pwmRange
			0 (off)  100 (fully on)
				returns 0 on success
	*/

	//if
    softPwmCreate(enable, 75, 100);  //( != 0)  {
//		printf("error setting pulse width modulation.");
//		return 1;
//	}
//		else {
				printf("pulse width modulation set successfully.");
				printf("enable_motor is set to: %d\n", motors->enable_motor);
				printf("forward_motor is set to: %d\n", motors->forward_motor);
				printf("reverse_motor is set to: %d\n", motors->reverse_motor);

				printf("setting mode of pins ... %s\n");
				pinMode(motors->enable_motor, OUTPUT);
				pinMode(motors->forward_motor, OUTPUT);
				pinMode(motors->reverse_motor, OUTPUT);
				
				printf("writing values to HIGH or LOW\n");

				/*Turn on enable pin, the motors do not move since they are both set to low.*/
				digitalWrite(motors->enable_motor, 1);
				digitalWrite(motors->forward_motor, 0);
				digitalWrite(motors->reverse_motor,0);
			}
	//}

///* speed = duty cycle % from 0 - 100 */
void forward(struct Motor * motors, int speed){
		printf("Moving forward...");
		softPwmWrite(motors->enable_motor, speed);
		digitalWrite(motors->forward_motor, 0);
		digitalWrite(motors->reverse_motor, 1);
}

void reverse (struct Motor * motors, int speed){
		printf("Moving in reverse...");
		softPwmWrite(motors->enable_motor, speed);
		digitalWrite(motors->forward_motor, 1);
		digitalWrite(motors->reverse_motor, 0);
}
void stop (struct Motor * motors) {
		printf("Stopping...");
		softPwmWrite(motors->enable_motor, 0);
		digitalWrite(motors->forward_motor, 1);
		digitalWrite(motors->reverse_motor, 1);
}
