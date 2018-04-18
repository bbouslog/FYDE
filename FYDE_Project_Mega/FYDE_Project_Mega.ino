#include <Blynk.h>
#include <Keypad.h>

/******************************************************

 RUN ME ON YOUR MEGA!!!

 This program sends your network and blynk token information
 to the WiFi module over serial, then should recieve characters over
 serial from the WiFi module in return.

 1) Wire Module:
 Wire according to this picture: https://www.forward.com.au/pfod/ESP8266/GPIOpins/ESP8266_01.jpg
 - Connect VCC and CH_PD to 3.3V on the arduino
 - Connect RX1 on the mega to TX on the programmer
 - Connect TX1 on the mega to RX on the programmer
 - Connect GND to GND
 
 2) Go to Tools -> Board and select Arduino/Genuino Mega or Mega 2560

 3) Sign up for Blynk (https://www.blynk.cc/) and install the app on your phone.
 MAKE SURE TO CREATE AND WRITE DOWN YOUR TOKEN!!!
 Then, go to Sketch -> Include Library -> Manage Libraries and search 'Blynk' and
 install the library

 4) Change the network name, password, and blynk token in the code to reflect your own 
 
 5) Finish writing the serial recieve in the 'void loop()'
 It needs to:
 - Continuously look for serial input on Serial 1 
 - Take any character recieved on Serial 1 and print
 the data out through the main Serial 

 6) Open a serial monitor by going to
 Tools -> Serial Monitor and set to 9600 baud once open to see output
 and run!
 
 ******************************************************/

#define led 13

const int CLK = 2;//CLK->D2
const int DT = 3;//DT->D3
const int SW = 5;//SW->D5
const int interrupt0 = 0;// Interrupt 0 is on pin 2 
const char hexaKeys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const int ledR = 9;
const int ledG = 10;
const int ledB = 11;
const byte rowPins[4] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
const byte colPins[4] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

static int count = 0;//Define the count
static int lastCLK = 0;//CLK initial value
static int currentColor = 0; // 0 is r, 1 is g, 2 is b
static int LED[3] = {0, 0, 0};
static int BreatheLED[3] = {255, 0, 0};
static int updateLED[3] = {0, 0, 0};
static int inputSum = -1;
static int lastSwitch = 1;
static int state = 0;
static int lastState = 0;
static int mode = 0; // 0 is normal, 2 is strobe
static int periodCounter = 0;
static int flashCounter = 0;
static int flashCycle = 0;
static int strobeCycle = 0;


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4); 

void cycleColor() {
  if (++currentColor == 3) {
    currentColor = 0;
  }
  flashCounter = 0;
  Serial.println("Changing color");
}




int     Pin_Number  = 255;
int     Pin_Integer = 0;
float   Pin_Float   = 0.0;

// **********************************
void DebugPrint(void) {

  Serial.print(Pin_Number);
  Serial.print(",");
  Serial.print(Pin_Integer);
  Serial.print(",");
  Serial.println(Pin_Float); 
 
}

void CheckKeypad(void) {
  if (!digitalRead(SW) && lastSwitch == 1) {
    cycleColor();
  }
  
  char key = customKeypad.getKey();
  if (key == '#') {
    if (state == 1) {
      if (inputSum != -1) {
        updateLED[currentColor] = inputSum > 255 ? 255 : inputSum;
      }
      // reset inputsum and update led
      inputSum = -1;
      LED[0] = updateLED[0];
      LED[1] = updateLED[1];
      LED[2] = updateLED[2];
      state = 0;
      flashCounter = 400;
    } else {
      state = 1;
      currentColor = 0;
      flashCounter = 0;
    }
  } else if (key == 'A') {
    mode = 0;
  } else if (key == 'B') {
    mode = 1;
  } else if (key == 'C') {
    mode = 2;
  } else if (key == 'D') {
    Serial.print("inputSum: ");
    Serial.println(inputSum);
    Serial.print("State: ");
    Serial.println(state);
  } else if (key == '*') {
    if (state == 1) {
      if (inputSum != -1) {
        updateLED[currentColor] = inputSum;
      }
      // reset inputsum
      inputSum = -1;
      cycleColor();
    } else if (state == 0) {
      cycleColor();
    }
  } else if (key != 0x0) {
    if (state == 1) {
      if (inputSum != -1) {
        inputSum *= 10;
      } else {
        inputSum = 0;
      }
      Serial.println(key - '0');
      inputSum += key - '0';
    }
  }
  if (lastState != state) {
    Serial.print("State: ");
    Serial.println(state);
  }
  if (mode == 0) {
    analogWrite(ledR, LED[0]);
    analogWrite(ledG, LED[1]);
    analogWrite(ledB, LED[2]);
  } else if (mode == 1) {
    periodCounter++;
    if (periodCounter < 5000) {
      analogWrite(ledR, 0);
      analogWrite(ledG, 0);
      analogWrite(ledB, 0);
    } else {
      analogWrite(ledR, LED[0]);
      analogWrite(ledG, LED[1]);
      analogWrite(ledB, LED[2]);
    }
    if (periodCounter > 10000) {
      periodCounter = 0;
    }
  }
  lastSwitch = digitalRead(SW);
  lastState = state;
}

// **********************************
void SerialInput_to_Mega(void) {
  
  while (Serial1.available() > 0) {
    Serial.println(Serial1.peek());
    //Serial.println("In serial input");
    // look for the next valid integer in the incoming serial stream:
//    Serial.println(Serial1.read());
    Pin_Number  = Serial1.parseInt();
    Pin_Integer = Serial1.parseInt();
    Pin_Float = Serial1.parseFloat();
//     
//    // look for the newline.
    if (Serial1.read() == '\n') {
      DebugPrint();
    } else {
      Serial.println("weird");
    }
  Parser();
  }
}

int Val = 50;
int led_val = 4;
int brightness = 255;

// **********************************
void Parser(void) {
  //Serial.println("In the parser");
  
  if((Pin_Number == 1) && (Pin_Integer == 1))  {
    
    DebugPrint();
    digitalWrite(led, HIGH);
    
    Serial1.print(Val);
    Serial1.print(",");
    Serial1.print(Pin_Integer);
    Serial1.print(",");
    Serial1.print(Val*Val);
    Serial1.print("\n");

    Serial1.print(led_val);
    Serial1.print(",");
    Serial1.print(brightness);
    Serial1.print(",");
    Serial1.print(Pin_Integer);
    Serial1.print("\n");
  }
  
  if((Pin_Number == 1) && (Pin_Integer == 0))  {
    DebugPrint();
    digitalWrite(led, LOW);

    Serial1.print(Val);
    Serial1.print(",");
    Serial1.print(Pin_Integer);
    Serial1.print(",");
    Serial1.print(Val*Val*8.39);
    Serial1.print("\n");

    Serial1.print(led_val);
    Serial1.print(",");
    Serial1.print(Pin_Integer);
    Serial1.print(",");
    Serial1.print(Val);
    Serial1.print("\n");
    
  }

  if (Pin_Number == 5) { 
    brightness = Pin_Integer; 
    
    DebugPrint();

    Serial1.print(led_val);
    Serial1.print(",");
    Serial1.print(brightness);
    Serial1.print(",");
    Serial1.print(Pin_Integer);
    Serial1.print("\n");
  }
 
}

int sensorPin = A0;    // select the input pin for the potentiometer
float sensorValueOld = 0.0;  // variable to store the value coming from the sensor
float sensorValueNew = 0.0;

// **********************************
void ReadSensors(void) {
  sensorValueNew = analogRead(sensorPin);
  
  if (abs(sensorValueNew - (sensorValueOld)) > 0) {
    Serial1.print(49);
    Serial1.print(",");
    Serial1.print(0);
    Serial1.print(",");
    Serial1.print((sensorValueNew/1023)*5);
    Serial1.print("\n");
    sensorValueOld = sensorValueNew;
  
//    Serial.print("Analog pin value = ");
//    Serial.print(sensorValueNew);
//    Serial.print("\n");
  }
}

void setup() {
  pinMode(led, OUTPUT);
  
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.begin(9600);
  
  digitalWrite(led, HIGH);
  
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  attachInterrupt(interrupt0, Clk, CHANGE);//Set the interrupt 0 handler, trigger level change
}


void loop() {
  SerialInput_to_Mega();
  //Execute();
  ReadSensors();
  CheckKeypad();
  FlashCurrent();
  Strobe();
  Breathe();
}


void Clk() {
  int clkValue = digitalRead(CLK); //Read the CLK pin level
  int dtValue = digitalRead(DT); //Read the DT pin level
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    LED[currentColor] += (clkValue != dtValue ? 5 : -5);//CLK and inconsistent DT + 1, otherwise - 1
    if (LED[currentColor] < 0) {
      LED[currentColor] = 0;
    }
    if (LED[currentColor] > 255) {
      LED[currentColor] = 255;
    }
    Serial.print("currentColor:");
    Serial.println(currentColor);
    Serial.print("amount: ");
    Serial.println(LED[currentColor]);
  }
}

void FlashCurrent(void) {
  if (flashCounter == 400) {
    return;
  } else {
    flashCounter++;
  }
  if (flashCycle < 50) {
    flashCycle++;
    switch(currentColor) {
      case 0:
        analogWrite(ledR, 255);
        analogWrite(ledG, 0);
        analogWrite(ledB, 0);
        break;
      case 1:
        analogWrite(ledR, 0);
        analogWrite(ledG, 255);
        analogWrite(ledB, 0);
        break;
      case 2:
        analogWrite(ledR, 0);
        analogWrite(ledG, 0);
        analogWrite(ledB, 255);
        break;
    }
  } else if (flashCycle == 100) {
    flashCycle = 0;
  } else {
    flashCycle++;
    switch(currentColor) {
      case 0:
        analogWrite(ledR, 0);
        analogWrite(ledG, 0);
        analogWrite(ledB, 0);
        break;
      case 1:
        analogWrite(ledR, 0);
        analogWrite(ledG, 0);
        analogWrite(ledB, 0);
        break;
      case 2:
        analogWrite(ledR, 0);
        analogWrite(ledG, 0);
        analogWrite(ledB, 0);
        break;
    }
  }
}

void Strobe(void) {
  if (mode == 2) {
    if (strobeCycle < 10) {
      strobeCycle++;
      analogWrite(ledR, LED[0]);
      analogWrite(ledG, LED[1]);
      analogWrite(ledB, LED[2]);
    } else if (strobeCycle == 20) {
      strobeCycle = 0;
    } else {
      strobeCycle++;
      analogWrite(ledR, 0);
      analogWrite(ledG, 0);
      analogWrite(ledB, 0);
    }
  }
}

void Breathe(void) {
  if (mode == 1) {
    // Led control
    if (BreatheLED[0] == 255 && BreatheLED[1] != 255) {
        if (BreatheLED[2] > 0) {
            BreatheLED[2]--;
        } else {
            BreatheLED[1]++;
        }
    } else if (BreatheLED[1] == 255 && BreatheLED[2] != 255) {
        if (BreatheLED[0] > 0) {
            BreatheLED[0]--;
        } else {
            BreatheLED[2]++;
        }
    } else if (BreatheLED[2] == 255 && BreatheLED[0] != 255) {
        if (BreatheLED[1] > 0) {
            BreatheLED[1]--;
        } else {
            BreatheLED[0]++;
        }
    }
    analogWrite(ledR, BreatheLED[0]);
    analogWrite(ledG, BreatheLED[1]);
    analogWrite(ledB, BreatheLED[2]);
  }
}

