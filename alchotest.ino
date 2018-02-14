/*-----------------------------------------------
			Bluetooth Alcho module
-----------------------------------------------*/
#include <SoftwareSerial.h>

/* Bluetooth module config */
SoftwareSerial BTserial(0, 1);  // RX | TX

const int DEBUG=0;
const int AOUTalcho=0;
const int DOUTalcho=8;
const int DOUTled=13;


void setup() {
	BTserial.begin(9600);         //sets the baud rate
}


float get_R0() {
  	float sensor_volt;
  	float RS_air; 	        //  Get the value of RS via in a clear air
  	float R0; 		        // Get the value of R0 via in Alcohol
  	float sensorValue;
  
  	/*--- Get a average data by testing 100 times  ---*/
  	for(int x = 0; x < 100; x++){
  		sensorValue = sensorValue + analogRead(AOUTalcho);
		delay(10);
  	}
  	sensorValue = sensorValue / 100.0;
  	/*-----------------------------------------------*/
  
  
  	sensor_volt = sensorValue / 1024 * 5.0;
  	RS_air = (5.0 - sensor_volt) / sensor_volt;		// omit * RL
  	R0 = RS_air / 70.0;								// The ratio of RS/R0 is 70 in a clear air
  
  	return R0;
}


float get_ratio(float R0) {
	float sensor_volt;
	float RS_gas; 			// Get value of RS in a GAS
	float ratio;			// Get ratio RS_GAS/RS_air

	int sensorValue = analogRead(A0);
	sensor_volt = (float)sensorValue / 1024 * 5.0;
	RS_gas = (5.0 - sensor_volt) / sensor_volt;		// omit *RL

	/*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
	ratio = RS_gas / R0;		// ratio = RS/R0 
	/*-----------------------------------------------------------------------*/
	return ratio;
}


String get_BAC(float ratio) {
	String value;
	if (ratio > 2.2) {
		value = " < 0.1";
	}else if(1.80 < ratio && ratio < 2.20) {
		value = "0.1 - 0.2";
	}else if(1.40 < ratio && ratio < 1.80) {
		value = "0.2 - 0.3";
	}else if(1.00 < ratio && ratio < 1.40) {
		value = "0.3 - 0.4";
	}else if(0.89 < ratio && ratio < 1.00) {
		value = "0.4 - 0.5";
	}else if(0.78 < ratio && ratio < 0.89) {
		value = "0.5 - 0.6";
	}else if(0.70 < ratio && ratio < 0.78) {
		value = "0.6 - 0.7";
	}else if(0.64 < ratio && ratio < 0.70) {
		value = "0.7 - 0.8";
	}else if(0.60 < ratio && ratio < 0.64) {
		value = "0.8 - 0.9";
	}else if(0.55 < ratio && ratio < 0.60) {
		value = "0.9 - 1.0";
	}else if(0.35 < ratio && ratio < 0.55) {
		value = "1.0 - 2.0";
	}else if(0.26 < ratio && ratio < 0.35) {
		value = "2.0 - 3.0";
	}else if(0.20 < ratio && ratio < 0.26) {
		value = "3.0 - 4.0";
	}else if(0.19 < ratio && ratio < 0.20) {
		value = "4.0 - 5.0";
	}else if(0.18 < ratio && ratio < 0.19) {
		value = "5.0 - 6.0";
	}else if(0.15 < ratio && ratio < 0.18) {
		value = "6.0 - 7.0";
	}else if(0.14 < ratio && ratio < 0.15) {
		value = "7.0 - 8.0";
	}else if(1.12 < ratio && ratio < 0.14) {
		value = "8.0 - 9.0";
	}else if(1.10 < ratio && ratio < 0.12) {
		value = "9.0 - 10.0";
	}else{
		value = "> 10.0";
	}

	return value + " mg/L";
}


void loop() {
  	float R0 = get_R0();
  	int i = 0;
  	float ratio, min_ratio = 70;


  	digitalWrite(DOUTled, HIGH);

	while(i < 50) {
		ratio = get_ratio(R0);
		min_ratio = min(ratio, min_ratio);
		i ++;
		delay(100);
	}

	digitalWrite(DOUTled, LOW);
        
        
        BTserial.print((float)min_ratio);
        BTserial.print(",");
  	BTserial.print(get_BAC((float)min_ratio));
  	BTserial.print(";");

  	delay(1000);
}
