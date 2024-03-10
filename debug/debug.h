#include <Arduino.h>

static const char* bin2tristate(const char* bin);
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

  const char* b = dec2binWzerofill(decimal, length);
  Serial.print("Decimal: ");
  Serial.print(decimal);
  Serial.print(" (");
  Serial.print( length );
  Serial.print("Bit) Binary: ");
  Serial.print( b );
  Serial.print(" Tri-State: ");
  Serial.print( bin2tristate( b) );
  Serial.print(" PulseLength: ");
  Serial.print(delay);
  Serial.print(" microseconds");
  Serial.print(" Protocol: ");
  Serial.println(protocol);
  
  // Serial.print("Raw data: ");
  // for (unsigned int i=0; i<= length*2; i++) {
  //   Serial.print(raw[i]);
  //   Serial.print(",");
  // }
  Serial.println();
  Serial.println();
}

static const char* bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}


void debug(RCSwitch receiver){
  output(receiver.getReceivedValue(), receiver.getReceivedBitlength(), receiver.getReceivedDelay(), receiver.getReceivedRawdata(),receiver.getReceivedProtocol());
}


void showJConfig(int data){
  String r = "rechts";
  String l = "links";
  String h = "hoch";
  String ru = "runter";
  String n = "neutral";
  Serial.print("J0 ");
  Serial.print("x: ");
  if ((data & 0b01000000) >> 6){
    Serial.print(n); 
    // data -> j1 -> right = (data & 0b01000000) >> 6;
  } 
  else {
      if ((data & 0b10000000)) Serial.print(r);
      else Serial.print(l);

  }
  Serial.print(" y: ");
  if ((data & 0b00010000) >> 4) Serial.print(n);
  else {
    if ((data & 0b00100000)) Serial.print(h);
    else Serial.print(ru);
  }
  
  Serial.println("");
  Serial.print("J1:");
  Serial.print(" x: ");
  if ((data & 0b00000100) >> 2) Serial.print(n);
  else {
      if ((data & 0b00001000)) Serial.print(r);
      else Serial.print(l);

  }
  Serial.print(" y: ");
  if ((data & 0b00000001)) Serial.print(n);
  else {
    if ((data & 0b00000010)) Serial.print(h);
    else Serial.print(ru);

  }
      Serial.println("");
    // Serial.print("data:");
    // Serial.print(data);
    // Serial.println("");
}


// void debugJParsedConfig(JCD jcd){
//   String r = "rechts";
//   String l = "links";
//   String h = "hoch";
//   String ru = "runter";
//   String n = "neutral";
//   Serial.print("J0 ");
//   Serial.print("x: ");
//   if (jcd.j0.x.none){
//     Serial.print(n); 
//   } 
//   else {
//       if (jcd.j0.x.right) Serial.print(r);
//       else if (jcd.j0.x.left) Serial.print(l);
//   }
//   Serial.print(" y: ");
//   if (jcd.j0.y.none) Serial.print(n);
//   else {
//     if (jcd.j0.y.up) Serial.print(h);
//     else if(jcd.j0.y.down) Serial.print(ru);
//   }
  
//   Serial.println("");
//   Serial.print("J1:");
//   Serial.print(" x: ");
//   if (jcd.j1.x.none) Serial.print(n);
//   else {
//       if (jcd.j1.x.right) Serial.print(r);
//       else if (jcd.j1.x.left) Serial.print(l);

//   }
//   Serial.print(" y: ");
//   if (jcd.j1.y.none) Serial.print(n);
//   else {
//     if (jcd.j1.y.up) Serial.print(h);
//     else if (jcd.j1.y.down) Serial.print(ru);

//   }
//       Serial.println("");
//     // Serial.print("data:");
//     // Serial.print(data);
//     // Serial.println("");
// }