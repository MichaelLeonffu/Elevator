#pragma config(Sensor, dgtl1,  limitSwitch1,         sensorTouch)
#pragma config(Sensor, dgtl2,  limitSwitch2,         sensorTouch)
//#pragma config(Sensor, dgtl3,  encoder,            sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  limitSwitchFloor2,    sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  limitSwitchFloor3,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  limitSwitch3,         sensorTouch)
#pragma config(Sensor, dgtl6,  button3,              sensorTouch)
#pragma config(Sensor, dgtl7,  button2,              sensorTouch)
#pragma config(Sensor, dgtl8,  button1,              sensorTouch)
#pragma config(Sensor, dgtl9,  limitSwitchFloor1,    sensorTouch)
#pragma config(Sensor, dgtl10, lED3,                 sensorLEDtoVCC)
#pragma config(Sensor, dgtl11, lED2,                 sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, lED1,                 sensorLEDtoVCC)
#pragma config(Motor,  port2, elevatorMotor,         tmotorVex393_MC29, openLoop)


//CONFIGUREATION:

	int FULLELABORATE;
	int SUPERBASICMODE = 0;
	int BASICMODE = 0;
	int SAFEMODE = 0;

	//SafetyPro
		int ELABORATESAFETYPRO = 1;
		int ELABORATESAFETYPROPOWER = 1;

	//CONSTANTS:
		//Motor Specifics
			int DIRECTION = 1; 
			int NORMALMOTORPOWER = 63;

		//SafetyPro constants 
			int SAFETYPROTIME = 20000;
			int FLOORDURATIONTIME = 5000;

		//ElevatorPosition
			int goTo = 0;

		//FailSafePro
			int ENABLEFAILSAFEPRO = 1;
			int FAILTIMER = 15000;
			int FAILSAFESAFE = 0;


//FUNCTIONS:

	elevatorPosition(int floor){
		int elevatorPositionPower;

		//Interpertaion of button pressess
			if(SensorValue(limitSwitchFloor1) == 1){
				if(SensorValue(limitSwitch2) != 0 || SensorValue(button2) != 0){
					goTo = 12;
				}
				if(SensorValue(limitSwitch3) != 0 || SensorValue(button3) != 0){
					goTo = 3;
				}
			}
			if(SensorValue(limitSwitchFloor2) == 1){
				if(SensorValue(limitSwitch1) != 0 || SensorValue(button1) != 0){
					goTo = 1;
				}
				if(SensorValue(limitSwitch3) != 0 || SensorValue(button3) != 0){
					goTo = 3;
				}
			}
			if(SensorValue(limitSwitchFloor3) == 1){
				if(SensorValue(limitSwitch2) != 0 || SensorValue(button2) != 0){
					goTo = 32;
				}
				if(SensorValue(limitSwitch1) != 0 || SensorValue(button1) != 0){
					goTo = 1;
				}
			}

		//Asignments to motor vectors
			if(goTo == 1){
				elevatorPositionPower = (-1 * DIRECTION * NORMALMOTORPOWER);

				if(SensorValue(limitSwitch1) == 1){
					goTo = 0;
				}
			}
			if(goTo == 12){
				elevatorPositionPower = (DIRECTION * NORMALMOTORPOWER);

				if(SensorValue(limitSwitch2) == 1){
					goTo = 0;
				}
			}
			if(goTo == 32){
				elevatorPositionPower = (-1 * DIRECTION * NORMALMOTORPOWER);

				if(SensorValue(limitSwitch2) == 1){
					goTo = 0;
				}
			}
			if(goTo == 3){
				elevatorPositionPower = (DIRECTION * NORMALMOTORPOWER);

				if(SensorValue(limitSwitch3) == 1){
					goTo = 0;
				}
			}
			if(goTo == 0){
				elevatorPositionPower = 0;
			}

		startMotor(elevatorMotor,elevatorPositionPower);
	}

	lEDIndicator(){
		if(SensorValue(limitSwitchFloor1) == 1){
			turnLEDOn(led1);
		}else{
			turnLEDOff(led1);
		}
		if(SensorValue(limitSwitchFloor2) == 1){
			turnLEDOn(led2);
		}else{
			turnLEDOff(led2);
		}
		if(SensorValue(limitSwitchFloor3) == 1){
			turnLEDOn(led3);
		}else{
			turnLEDOff(led3);
		}
	}

	safetyPro(){
		if(ELABORATESAFETYPROPOWER == 1){
			int elabSPP;
		}
		if((SensorValue(button1) != 0 || SensorValue(button2) != 0 || SensorValue(button3) != 0 || SensorValue(limitSwitch1) != 0 || SensorValue(limitSwitch2) != 0 || SensorValue(limitSwitch3) != 0) && time1(T1) >= SAFETYPROTIME){
			clearTimer(T1);
			while(ELABORATESAFETYPRO == 0 && SensorValue(limitSwitchFloor1) != 0 ){
				if(ELABORATESAFETYPROPOWER == 1){
					elabSPP = (DIRECTION * NORMALMOTORPOWER);
					startMotor(elevatorMotor,elabSPP);
				}else{
					startMotor(elevatorMotor,-63);
				}
			}
		}
	}

	failSafePro(){
		if(limitSwitchFloor1 == 0 && limitSwitchFloor2 == 0 && limitSwitchFloor3 == 0 && time1(T2) >= FAILTIMER){
			if(FAILSAFESAFE == 1){
				startMotor(elevatorMotor, -63);
			}else{
				startMotor(elevatorMotor,(-DIRECTION * NORMALMOTORPOWER));
			}
		}else{
			clearTimer(T2);
		}

	}


task main(){
	int eButton1;
	int eButton2;
	int eButton3;

	clearTimer(T1);

	while(true){
		eButton1 = (SensorValue(button1) + SensorValue(limitSwitch1));
		eButton2 = (SensorValue(button2) + SensorValue(limitSwitch2));
		eButton3 = (SensorValue(button3) + SensorValue(limitSwitch3));

		lEDIndicator();
		safetyPro();
		failSafePro();

	}
}





