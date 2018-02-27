const int CLK = 2;//CLK->D2
const int DT = 3;//DT->D3
const int SW = 4;//SW->D4
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
const byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
const byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

int count = 0;//Define the count
int lastCLK = 0;//CLK initial value
int currentColor = 0; // 0 is r, 1 is g, 2 is b
int LED[3] = {0, 0, 0};
int updateLED[3] = {0, 0, 0};
int inputSum = -1;
int lastSwitch = 1;
int state = 0;
int lastState = 0;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4); 

void cycleColor() {
  if (++currentColor == 3) {
    currentColor = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  attachInterrupt(interrupt0, Clk, CHANGE);//Set the interrupt 0 handler, trigger level change
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(SW) && lastSwitch == 0) {
    Serial.println("Color change");
    cycleColor();
  }
  char key = customKeypad.getKey();
  if (key == '#') {
    if (state == 1) {
      state = 0;
    } else {
      state = 1;
    }
  } else if (key == 'A') {
    
  } else if (key == 'B') {
    
  } else if (key == 'C') {
    
  } else if (key == 'D') {
    
  } else if (key == '*') {
    if (state == 1) {
      if (inputSum != -1) {
        // set new rgb value
      } else {
        // reset inputsum
        inputSum = -1;
        cycleColor();
      }
    }
  } else {
    if (state == 1) {
      if (inputSum != -1) {
        inputSum *= 10;
      }
      inputSum += key - '0';
    }
  }
  lastSwitch = digitalRead(SW);
  analogWrite(ledR, led[0]);
  analogWrite(ledG, led[1]);
  analogWrite(ledB, led[2]);
}

void Clk() {
  int clkValue = digitalRead(CLK); //Read the CLK pin level
  int dtValue = digitalRead(DT); //Read the DT pin level
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    led[currentColor] += (clkValue != dtValue ? 5 : -5);//CLK and inconsistent DT + 1, otherwise - 1
    if (led[currentColor] < 0) {
      led[currentColor] = 0;
    }
    if (led[currentColor] > 255) {
      led[currentColor] = 255;
    }
    Serial.print("currentColor:");
    Serial.println(currentColor);
    Serial.print("amount: ");
    Serial.println(led[currentColor]);
  }
}

