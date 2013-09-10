/*
 * Cmd_Response
 *
 * Simple USB command/response handling for Arduino I/O capabilities
 */

#include "Cmd_Response.h"

Cmd_Response::Cmd_Response() {
  inputString.reserve(BUFFER_LENGTH);
  reset();
}

// public: - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Cmd_Response::readBuffer() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == EOS_TERMINATOR_CHAR) {
      stringComplete = true;
      break;
    }    
  }
}

void Cmd_Response::processCmd() {
  if (stringComplete) {
    parse();    // process the command
    reset();    // clear for the next command
  }
}

// private: - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Cmd_Response::reset() {
  inputString = "";           // a string to hold incoming data
  stringComplete = false;     // whether the string is complete
}

//  USB command interface
//  
//  =======  =======================================================
//  command  action
//  =======  =======================================================
//  ?adc#    returns current value of numbered ADC (0..1023)
//  ?bi#     returns current value of numbered binary input (0..1)
//  !bo# v   sets numbered binary output to value v (0..1)
//  !pwm# v  sets numbered PWM output to value v (0..255)
//  other    returns "ERROR_UNKNOWN_COMMAND:text"
//  =======  =======================================================

void Cmd_Response::parse() {
  String lc_inputString = inputString;
  int pos, pin, value;
  
  lc_inputString.toLowerCase();    // modifies in-place as of Arduino v1.0

  if      (lc_inputString.startsWith("?adc")) readADC(lc_inputString);
  else if (lc_inputString.startsWith("?bi"))  readBI(lc_inputString);
  else if (lc_inputString.startsWith("!bo"))  writeBO(lc_inputString);
  else if (lc_inputString.startsWith("!pwm")) writePWM(lc_inputString);
  else {
    Serial.print("ERROR_UNKNOWN_COMMAND:");
    Serial.println(inputString);
  }
}

void Cmd_Response::readADC(String s) {
  int pin = sstoi(s, 4, IGNORE_FINISH);
  // TODO: range check the pin first!
  Serial.print("adc");
  Serial.print(pin);
  Serial.print(" ");
  Serial.println(analogRead(pin));
}

void Cmd_Response::readBI(String s) {
  int pin = sstoi(s, 3, IGNORE_FINISH);
  // TODO: range check the pin first!
  Serial.print("bi");
  Serial.print(pin);
  Serial.print(" ");
  Serial.println(digitalRead(pin));
}

void Cmd_Response::writeBO(String s) {
  int pos = s.indexOf(" ");
  int pin = sstoi(s, 3, pos);
  int value = sstoi(s, pos, IGNORE_FINISH);
  if (value < ADC_MIN_VALUE || value > ADC_MAX_VALUE) {
    Serial.print("ERROR_ADC_RANGE:");
    Serial.println(inputString);
  } else {
    // TODO: range check the pin first and make sure it is OK for output!
    digitalWrite(pin, value);
    Serial.print("bo");
    Serial.print(pin);
    Serial.print(" ");
    Serial.println(value);
  }
}

void Cmd_Response::writePWM(String s) {
  int pos = s.indexOf(" ");
  int pin = sstoi(s, 4, pos);
  int value = sstoi(s, pos, IGNORE_FINISH);
  if (value < PWM_MIN_VALUE || value > PWM_MAX_VALUE) {
    Serial.print("ERROR_PWM_RANGE:");
    Serial.println(inputString);
  } else {
    // TODO: range check the pin first and make sure it is OK for PWM output!
    analogWrite(pin, value);
    Serial.print("pwm");
    Serial.print(pin);
    Serial.print(" ");
    Serial.println(value);
  }
}



/** sstoi()
 *
 * Convert a String substring to an integer, If finish == IGNORE_FINISH,
 * then it converts from position "start" to end of String.
 */
int Cmd_Response::sstoi(String s, int start, int finish) {
  char buf[BUFFER_LENGTH];
  int number;
  String sub;

  if (finish != IGNORE_FINISH) {
    sub = s.substring(start, finish);
  } else {
    sub = s.substring(start);
  }
  sub.toCharArray(buf, BUFFER_LENGTH);
  number = atoi(buf);
  
  return(number);
}

//########## SVN repository information ###################
// $Date$
// $Author$
// $Revision$
// $URL$
// $Id$
//########## SVN repository information ###################

