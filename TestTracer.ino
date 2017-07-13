//Code Helloword SigFox
#include <SigFox.h>//declaration of SigFox librairies needed to send message with SigFox
#include <ArduinoLowPower.h>//Useful to switchoff the board
#define TIMEOFF 1*60*1000// set the delay to 15 minutes

int debug = false;
byte buf[12];
int tmp;
void setup() 
{
  Serial.begin(9600);
  while(!Serial1){}; // wait for serial port to connect. Needed for native USB

  delay(100);
  if (!SigFox.begin())
  {
    //try rebooting if something is wrong
    reboot();
  }
  SigFox.end();// Send module to standby until we need to send a message
  if (debug) {
    SigFox.debug();// Enable debugging. Enabling the debugging all the power saving features are disabled and the led indicated as signaling pin (LED_BUILTIN as default) is used during transmission and receive events.
  }
  cleararray(buf);
}
void reboot()
{
  NVIC_SystemReset();
  while(1);
}
void cleararray(byte Array[])
{
  for(int i=0; i<sizeof(Array)/sizeof(byte);i++)
  {
    Array[i]=0;
  }
}
void loop() 
{
    tmp=0;
    while(buf[tmp]!=0){tmp++;}
    if(!SigFox.begin())
    {
      buf[tmp]=1;
      return;
    }
    delay(100);//need to wait after the first configuration ( 100ms ) 
    SigFox.beginPacket();
    SigFox.write(buf);
    if(!SigFox.endPacket())
    {
      buf[tmp]=2;
      return;
    }
    cleararray(buf);
    SigFox.end();
    LowPower.sleep(TIMEOFF);//enter in sleep mode
}
