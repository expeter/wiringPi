/*
 * isr.c:
 *	Wait for Interrupt test program - ISR method
 *
 *	How to test:
 *	  Use the SoC's pull-up and pull down resistors that are avalable
 *	on input pins. So compile & run this program (via sudo), then
 *	in another terminal:
 *		gpio mode 0 up
 *		gpio mode 0 down
 *	at which point it should trigger an interrupt. Toggle the pin
 *	up/down to generate more interrupts to test.
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>


// What GPIO input are we using?
//	This is a wiringPi pin number

#define	BUTTON_PIN	0

// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

// globaler wert in dem die interrupts / signale gespeichert werden
static volatile int globalCounter = 0 ;


/*
 * myInterrupt:
 *********************************************************************************
 */
// methode die immer wenn eine unterbrechung /signal stattfindet aufgerufen wird
void myInterrupt (void)
{
  ++globalCounter ;
}


/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{
  int myCounter = 0 ;

  if (wiringPiSetup () < 0)
  {
    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
  
  
//#1
  //register interrupt, everytime the signal changes, call "myInterrupt" and count+1
  //interrupt registrieren (also immer wenn eine signal änderung stattfindet
  if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &myInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    return 1 ;
  }

  
//#2
  //endless loop
  //endlos schleife
  for (;;)
  {
  
    
//#3 
    //Waiting ... print to command line, just to show something is waiting...
    //Waiting ... ausgeben, im grunde wird hier auf den nächsten interrupt / signal gewartet
    printf ("Waiting ... ") ; fflush (stdout) ;

//#4
    //endless loop, waiting 100ms between each loop, it basically checks if the counter changed due to the interrupt method which is called on each interrupt / signal
    //endlosschleife, wartet 100ms zwischen neuer prüfung ob es ein signal gegeben hat
    while (myCounter == globalCounter)
      //100ms warten
      delay (100) ;
    
    
//#5
    // print new value
    // neuen wert ausgeben
    printf (" Done. counter: %5d\n", globalCounter) ;
    
    
//#6
    //save new value, so that the loop on #4 is working (until next interrupt)
    //neuen wert speichern, damit die endlosschleife bei #4 funktioniert (bis zum nächsten signal/interrupt)
    myCounter = globalCounter ;
  }

  return 0 ;
}
