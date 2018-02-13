int CLK = 2;//CLK->D2
int DT = 3;//DT->D3
int SW = 4;//SW->D4
const int interrupt0 = 0;// Interrupt 0 is on pin 2 
int count = 0;//Define the count
int lastCLK = 0;//CLK initial value
int currentColor = 0; // 0 is r, 1 is g, 2 is b
int led[3] = {0, 0, 0};
int ledR = 9;
int ledG = 10;
int ledB = 11;

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
  if (!digitalRead(SW)) {
    if (++currentColor == 3) {
      currentColor = 0;
    }
    Serial.println("Color change");
  }
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
    led[currentColor] += (clkValue != dtValue ? 1 : -1);//CLK and inconsistent DT + 1, otherwise - 1
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

