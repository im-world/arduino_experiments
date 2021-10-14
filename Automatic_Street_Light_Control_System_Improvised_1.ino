//Instructions can be executed while the micoprocessor waits.
//Further, switching of states takes place only when the light 
//level remains the same (dark/light) at the beginning and end
//of an interval of one minute. This ensures that the state does
//not change due to noise (e.g., bird sitting on the sensor)

#define ldrPin A1
#define ledPin 4

int ldrValue = 0;
const int lightThreshold = 900;  

const unsigned long waitTime = 1000*60;     //one minute delay
unsigned long previousTime = 0;
  
bool darkDetect = 0;
bool lightDetect = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ldrValue = analogRead(ldrPin);
  unsigned long currentTime = millis();
  
  if(currentTime - previousTime >= waitTime){
    if(ldrValue < lightThreshold && darkDetect == 0){
      darkDetect = 1;
        lightDetect = 0;
        Serial.println("if 1");
    }
    else if(ldrValue < lightThreshold && darkDetect == 1){
      turnLedOn();
    Serial.println("if 2");
    }
    else if(ldrValue >= lightThreshold && lightDetect == 0){
      darkDetect = 0;
        lightDetect = 1;
    Serial.println("if 3");
    }
    else if(ldrValue >= lightThreshold && lightDetect == 1){
    turnLedOff();
        Serial.println("if 4");
    }
    else{       Serial.println("if nothing");}
    
    previousTime = currentTime;
  }  
}

void turnLedOn(){
  digitalWrite(ledPin, HIGH);
}

void turnLedOff(){
  digitalWrite(ledPin, LOW);
}
