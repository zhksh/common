#include <RCSwitch.h>

#define CHECK_BIT(var,pos) (((var) & (1<<(pos))) != 0)
#define BIT_RANGE(var, len, pos) ((var >> pos) & ((1 << len)-1))

long bitrange(long msg, int len, int pos){
  long mask = (1 << len)-1;
  // Serial.println(mask);
  long result = msg >> pos;
  return result & mask;
}

long islegit(long data, long prefix, int prefix_pos, int prefix_len) {
  return BIT_RANGE(data, prefix_len, prefix_pos) == prefix;
}

unsigned long addprefix(long data, long prefix, int prefix_pos) {
  unsigned long p = prefix << prefix_pos;
  return  data | p;
}

struct XAxis {
  bool right;
  bool left;
  bool none;
};

struct YAxis {
  bool up;
  bool down;
  bool none;
};

class Joystick {
  public:
    XAxis x;
    YAxis y;
  
  String name;
  
  Joystick(int data){
    parse(data);
  }

  Joystick(String desc){
    setName(desc);
  };

  void parse(int data){
    int xData = bitrange(data, 2, 0);
    int right = CHECK_BIT(xData, 1);
    x = XAxis{right, !right, CHECK_BIT(xData, 0)};

    int yData = bitrange(data, 2, 2);
    int up = CHECK_BIT(yData,1);
    y = YAxis{up, !up, CHECK_BIT(yData, 0)};
  }

  void setName(String descr){
    name = descr;
  }

};



class TZ {
    public: 
    bool stop = false;
    bool ausgefahren = false;
    bool faus = false;
    bool fein = false;
    unsigned long ausz = 0;
    unsigned long einz = 0;
    unsigned long counter = 0;
    int speedPin = 150;

    int pin1;
    int pin2;
    int stopPin;

    String name;

    TZ(unsigned long afz, unsigned long efz, int p1, 
        int p2, int stop_pin, int speed_pin, String descr){
      ausz = afz;
      einz = efz;
      pin1 = p1;
      pin2 = p2;
      stopPin = stop_pin;
      speedPin = speed_pin;
      name = descr;
    }

    void setSpeed(int speed){
      analogWrite(speedPin, speed);
    }

    void turnR(){
      setPins(LOW, HIGH);
      einz = true;
      log("einfahren");
      Serial.print(millis() - counter);
      Serial.print(")");
    };

    void turnL(){
      setPins(HIGH, LOW);
      einz = true;
      log("ausfahren");
      Serial.print((millis() - counter));
      Serial.print(")");
    };

    void stopTZ(){
      setPins(LOW, LOW);
      einz = false;
      ausz = false;
      counter = 0;
     log("stop");
    }

    void log(String action){
      Serial.print(name);
      Serial.print(":");
      Serial.print(action);
    };

    void setPins(int p1, int p2){
      digitalWrite(pin1, p1);
      digitalWrite(pin2, p2);
    };

    void update(int controlSig){
      if (digitalRead(stopPin) == 0){
        ausgefahren = true;
        stopTZ();
        return;
      }
      
      if (controlSig) {  
        if (!ausgefahren){
          if ((counter > 0) && (millis() - counter) >= ausz){
            stopTZ();
            ausgefahren = true;
          }
          else {
            //tauchzelle fährt nicht gerade aus
            if (!fein){
              //start timer ausfahrt
              if (!faus) counter = millis();
              turnL();      
            }       
          }         
        }     
      }
      //schalter aus
      else {
        //nur wenn ganz ausgefahren, wieder einfahren
        if (ausgefahren){
           //start timer einfahrt
          if (!fein) counter = millis();
          
          if ((counter > 0) && (millis() - counter) > einz){
            //genug eingefahren, stop
            log("eingefahren");
            stopTZ();
            ausgefahren = false;
          }
          else {
            //einfahren
            turnR();
          }       
        }      
        
      }
    }
};
