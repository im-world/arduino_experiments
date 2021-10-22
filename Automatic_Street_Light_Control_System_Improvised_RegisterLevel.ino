/*An automatic street light control system using an Arduino Uno board, an LED, and 
an LDR. The LDR is used to sense the light levels. The Arduino is programmed at the 
register-level so that if the LDR value remains low for at least a minute, the LED 
would be turned ON, and if it remains high for at least a minute, the LED would be 
turned OFF. This is achieved in such a way that the Arduino can continue the execution of 
other instructions while waiting.*/

//Defining pins
#define ldrPin A1
#define ledPin 4

//Defining constants
int ldrValue = 0;
const int lightThreshold = 900;
const unsigned long clockFreq = 16000000;  //Hz
const int preScaler = 1024;         //maximum value chosen; Upon changing, also adjust TCCR1B accordingly
const float desiredTimeP = 60;        //s
const unsigned long OCR = ((clockFreq*desiredTimeP)/preScaler) - 1; 

//OCR is substantially greater than 65536; hence we use a smaller 'factor time period' 
//that can be handled by the 16-bit timer, and use its multiples to generate the desired
//time period

const int factorTimeP = 1;
const int OCRfinal = ((clockFreq*factorTimeP)/preScaler) - 1UL;
volatile int seconds;   //volatile since ISR involved

//flags
bool darkDetect = 0;
bool lightDetect = 1;


void setup()
{
  Serial.begin(9600);       //debugging purposes
  cli();                //stop all interrupts before changing internal register values

    //initialisation; remove any preset/default values
    TCCR1A = 0;
  TCCR1B = 0;         
  
    OCR1A = OCRfinal; 
  
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);     //CTC and 1024 prescaler 
  TIMSK1 |= (1 << OCIE1A);                  //output compare A match interrupt
                      
    sei();                                //enable all interrupts

  pinMode(ldrPin, INPUT);
    pinMode(ledPin, OUTPUT);
}

ISR(TIMER1_COMPA_vect){         //interrupt service routine 

  seconds++;
  Serial.print("Seconds elapsed since last query:");
  Serial.println(seconds);
    
    //generating the desired one minute period
    if(seconds == desiredTimeP){
        seconds = 0;
      ldrValue = analogRead(ldrPin);
        if(ldrValue < lightThreshold && darkDetect == 0){
          darkDetect = 1;
          lightDetect = 0;
          Serial.println("condition 1");
      }
      else if(ldrValue < lightThreshold && darkDetect == 1){
          turnLedOn();
        Serial.println("condition 2");
      }
      else if(ldrValue >= lightThreshold && lightDetect == 0){
          darkDetect = 0;
          lightDetect = 1;
        Serial.println("condition 3");
      }
      else if(ldrValue >= lightThreshold && lightDetect == 1){
        turnLedOff();
          Serial.println("condition 4");
      }
      else{       
            Serial.println("no change");
        }
    }
}


void loop()
{
  // your program here...
}

void turnLedOn(){
  digitalWrite(ledPin, HIGH);
}

void turnLedOff(){
  digitalWrite(ledPin, LOW);
}
