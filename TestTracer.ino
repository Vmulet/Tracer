#include <SigFox.h>//declaration of SigFox librairies needed to send message with SigFox
#include <ArduinoLowPower.h>//Useful to switchoff the board
#define TIMEOFF 1*60*1000// set the delay to 1 minutes

int debug = false;
byte buf[12];
int tmp=0;
int minutes=15;
void setup() 
{
  if (!SigFox.begin())
  {
    reboot(); //try rebooting if something is wrong
  }
  SigFox.end();// Send module to standby until we need to send a message
  if (debug) {
    SigFox.debug();// Enable debugging. Enabling the debugging all the power saving features are disabled and the led indicated as signaling pin (LED_BUILTIN as default) is used during transmission and receive events.
    Serial.begin(9600);
    while(!Serial1){}; // wait for serial port to connect. Needed for native USB
  }
  delay(10000);
  cleararray(buf);
  //LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, dummy, CHANGE);
}
//void dummy(){}
void reboot()
{
  NVIC_SystemReset();
  while(1);
}
void cleararray(byte arr[])
{
  for(int i=0; i<sizeof(arr);i++)
  {
    arr[i]=0;
  }
}
void loop() 
{
    if(!SigFox.begin())
    {
      buf[tmp++]=1;
      Serial.print(buf[tmp-1]);
      if(tmp>=12){tmp=0;}
      return;
    }
    delay(100);//need to wait after the first configuration ( 100ms ) 
    SigFox.beginPacket();
    SigFox.write(buf);
    if(SigFox.endPacket())
    {
      buf[tmp++]=2;
      Serial.print(buf[tmp-1]);
      if(tmp>=12){tmp=0;}
      return;
    }
    cleararray(buf);
    SigFox.end();
    //delay(TIMEOFF);
    for(int i=0;i<minutes;i++){ LowPower.sleep(TIMEOFF);}
}


