
#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(0, 1); // RX, TX

// We can't rename device most of the time. So this char allows to make differnce between differnt device you would use.
// Try to change this every time you upload the code.
bool m_useDeviceId=true;
String m_deviceCharId0="C";
String m_deviceCharId1="D";


float m_charAsValueAlphaLenght =53.0;
char m_charAsValueAlpha[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z', '|',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z'
};

void GetLetterForPercent(float percent, char &charResult) {

  int index = round((m_charAsValueAlphaLenght-1)*percent);
  charResult= m_charAsValueAlpha[index];
}

struct ObserverBoolean {
  public:
    bool m_use = true;
    int m_index = 0;
    bool m_previousValue = false;
    char m_charOn;
    char m_charOff;

    // Constructor to set initial values
    ObserverBoolean(bool use, int index, bool initialValue, char on, char off) {
      m_use = use;
      m_index = index;
      m_previousValue = initialValue;
      m_charOn = on;
      m_charOff = off;
    }

    void SetValue(bool newValue, bool &hasChange) {
      hasChange = (m_previousValue != newValue);
      m_previousValue = newValue;
    }
};

struct ObserverFloat {
  public:
    bool m_use = true;
    int m_index = 0;
    float m_previousValue = 1.0f;
    float m_changeRange =0.1;
    char m_charIndex;

    // Constructor to set initial values
    ObserverFloat(bool use, int index, float initialValue, float changeRange, char charIndex) {
      m_use = use;
      m_index = index;
      m_previousValue = initialValue;
      m_changeRange =changeRange;
      m_charIndex = charIndex;
    }

    void SetValue(float newValue, bool &hasChange) {
      hasChange =
       (newValue < m_previousValue-m_changeRange || newValue>m_previousValue+m_changeRange)
      || ( newValue !=m_previousValue && newValue==1.0) 
      || (newValue!=m_previousValue && newValue==0.0);

      if(hasChange)  
        m_previousValue = newValue;
    }
};

float m_analogZeroStartAt=0.04;
const int numberOfObservers = 14;
ObserverBoolean observers[numberOfObservers] = {
  ObserverBoolean(false, 0, false, 'A', 'a'),
  ObserverBoolean(false, 1, false, 'B', 'b'),
  ObserverBoolean(true, 2, false, 'C', 'c'),
  ObserverBoolean(true, 3, false, 'D', 'd'),
  ObserverBoolean(true, 4, false, 'E', 'e'),
  ObserverBoolean(true, 5, false, 'F', 'f'),
  ObserverBoolean(true, 6, false, 'G', 'g'),
  ObserverBoolean(true, 7, false, 'H', 'h'),
  ObserverBoolean(true, 8, false, 'I', 'i'),
  ObserverBoolean(true, 9, false, 'J', 'j'),
  ObserverBoolean(true, 10, false, 'K', 'k'),
  ObserverBoolean(true, 11, false, 'L', 'l'),
  ObserverBoolean(true, 12, false, 'M', 'm'),
  ObserverBoolean(true, 13, false, 'N', 'n')
};

const int numberOfObserversAnalog = 6;
ObserverFloat observersAnalog[numberOfObserversAnalog] = {
  ObserverFloat(true, A0, 0.0,0.1, 'A'),
  ObserverFloat(true, A1, 0.0,0.1, 'B'),
  ObserverFloat(true, A2, 0.0,0.1, 'C'),
  ObserverFloat(true, A3, 0.0,0.1, 'D'),
  ObserverFloat(true, A4, 0.0,0.1, 'E'),
  ObserverFloat(true, A5, 0.0,0.1, 'F')
};

void setup() {
  Serial.begin(9600);
   bluetoothSerial.begin(9600);

  for (int i = 0; i < numberOfObservers; i++) {
    if( observers[i].m_use){
      pinMode(observers[i].m_index, INPUT_PULLUP);
    }
    
  }
    for (int i = 0; i < numberOfObserversAnalog; i++) {
    if( observersAnalog[i].m_use){
      pinMode(observersAnalog[i].m_index, INPUT_PULLUP);
    }
    
  }


}
int i=0;
void loop() {


  for (int i = 0; i < numberOfObservers; i++) {
      if( observers[i].m_use){
        bool newValue = digitalRead(observers[i].m_index);
        bool hasChanged=false;
        observers[i].SetValue(newValue, hasChanged );

        // If the value has changed, print a message
        if (hasChanged) {
          //Serial.print("Observer ");
          //Serial.print(observers[i].m_index);
          //Serial.print(": Value changed to ");
          if(!m_useDeviceId){
            char charToSend =newValue ? observers[i].m_charOn : observers[i].m_charOff;
            Serial.println(charToSend);
            bluetoothSerial.println(charToSend);
          }
          else{
            String str = ""+ m_deviceCharId0 + m_deviceCharId1 + String(newValue ? observers[i].m_charOn : observers[i].m_charOff) ;
            Serial.println(str);
            bluetoothSerial.println(str);  

          }
        }
      } 
    }
  for (int i = 0; i < numberOfObserversAnalog; i++) {
        if( observersAnalog[i].m_use){
          float newValue = analogRead(observersAnalog[i].m_index)/1023.0;
          newValue = (newValue-m_analogZeroStartAt)/ (1.0-m_analogZeroStartAt);
          if(newValue<0)
            newValue=0;
          bool hasChanged=false;
          observersAnalog[i].SetValue(newValue, hasChanged );

          // If the value has changed, print a message
          if (hasChanged) {
            //Serial.print("Observer ");
            //Serial.print(observers[i].m_index);
            //Serial.print(": Value changed to ");
            char percentChar ="A";
            GetLetterForPercent(newValue, percentChar);
            String str =m_useDeviceId?
            ""+(m_deviceCharId0)+(m_deviceCharId1)+ String(observersAnalog[i].m_charIndex)+percentChar
            : ""+ String(observersAnalog[i].m_charIndex)+""+percentChar;
            str =  ""+
            Serial.println(str);
            bluetoothSerial.println(str);
          }
        } 
      }



  // Write a character
   /*
   i++;
  if(i>5)
    i=0;
  char myChar = 'A'+ i;
  String s=String(myChar);;
  Serial.println(s);
  delay(500);
  s.toLowerCase();
  Serial.println(s);
  delay(500);

  Serial.println(s+"0");
  delay(150);
  Serial.println(s+"8");
  delay(150);
  Serial.println(s+"4");
  delay(150);
  Serial.println(s+"|");
  delay(150);
  Serial.println(s+"9");
  delay(150);
  Serial.println(s+"A");
  delay(150);
  Serial.println(s+"z");
  delay(150);
  Serial.println(s+"d");
  delay(150);
  Serial.println(s+"j");
  delay(150);
  Serial.println(s+"l");
  delay(150);
  Serial.println(s+"M");
  delay(150);
  Serial.println(s+"G");
  delay(150);
  delay(5000);

  // Write a string
  /*
  String myString = "Hello, Arduino!";
  Serial.print("String: ");
  Serial.println(myString);
  //*/
  // Wait for 1 second
  //*/
}


