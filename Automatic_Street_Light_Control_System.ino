#define ldrPin A1
#define ledPin 4

int ldrValue = 0;
int lightThreshold = 100;  
int delayTime = 1000*60;     //one minute delay

void setup() {
  // put your setup code here, to run once:
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  ldrValue = analogRead(ldrPin);
  
  if(ldrValue < lightThreshold){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  
  delay(delayTime);     //read LDR Value after some delay

}
