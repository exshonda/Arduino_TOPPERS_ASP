/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#include <ToppersASP.h>

#define USE_SERIALUSB

// the setup function runs once when you press reset or power the board
void setup() {
#ifdef USE_SERIALUSB
  SerialUSB.begin(115200);
  ToppersASPDelayMs(1000);
  while (!SerialUSB);
#endif /* USE_SERIALUSB */

  pinMode(LED_BUILTIN, OUTPUT);

  // initialize digital pin LED_BUILTIN as an output.
  StartToppersASP();
}

void
task1(intptr_t exinf)
{
  int count = 0;
  while(1){
    syslog(LOG_NOTICE, "task1 is running. %d", count++);
    dly_tsk(1000);                     // wait for a second    
  }
}

void
task2(intptr_t exinf)
{
  int count = 0;
  while(1){
    syslog(LOG_NOTICE, "task2 is running. %d", count++);
    dly_tsk(1000);                     // wait for a second            
  }
}

void
user_inirtn(void)
{
  T_CTSK  ctsk;
  ER    ercd;

  ctsk.tskatr = TA_ACT;
  ctsk.exinf = 1;
  ctsk.task = task1;
  ctsk.itskpri = 10;
  ctsk.stksz = 1024;
  ctsk.stk = NULL;
  ercd = cre_tsk(TASK1, &ctsk);

  ctsk.task = task2;
  ercd = cre_tsk(TASK2, &ctsk);
  assert(ercd == E_OK);  
}

void
user_terrtn(void) {
  
}

int loop_count;

// the loop function runs over and over again forever
void loop() {
#ifdef USE_SERIALUSB
  SerialUSB.print("loop: ");
  SerialUSB.println(loop_count++);
#else /* USE_SERIALUSB */
	syslog(LOG_NOTICE, "loop is running. %d", loop_count++);
#endif /* USE_SERIALUSB */
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
