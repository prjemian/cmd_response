/** cmd_response
 *
 * simple command/response access via USB to Arduino's I/O capabilities
 */

/***********************************************************************
 Examples
  
 1. Read analog input from pin 0:

    >>> ?ai 0
    41
 
 2. Set digital pin 11 for PWM output:

    >>> !pin 11 1
    Ok
 
 3. Set PWM output pin 11 to 128:
 
    >>> !pwm 11 128
    Ok
 
 4. Send a bad command (no space between baseCmd and pin):
 
    >>> !pwm11 128
    ERROR_UNKNOWN_COMMAND:!pwm11 128

 ***********************************************************************/

#define BUFFER_LENGTH          40
#define COMMAND_LENGTH         16
#define EOS_TERMINATOR_CHAR    '\n'
#define USB_BAUD               115200
#define UNDEFINED              -1

#define ADC_MAX_VALUE 1023
#define ADC_MIN_VALUE 0
#define PWM_MAX_VALUE 255
#define PWM_MIN_VALUE 0

//#define TALKATIVE true

char inputString[BUFFER_LENGTH+1];
char strPtr;
boolean stringComplete;
int pin, argValue;
char baseCmd[COMMAND_LENGTH+1];

void setup() {
  resetBuffer();
  Serial.begin(115200);
  Serial.print(F("cmd_response started: "));
  Serial.println(freeRam());
}

void loop() {
  readBuffer();
  processCmd();
}

/**
  * Finds the amount of free memory by calculating the difference
  * between the address of a new stack variable and the address of 
  * the end of the heap.
  */
int freeRam()
{   
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void readBuffer() {
  while (Serial.available()) {
    if (strPtr == BUFFER_LENGTH) {
      Serial.println(F("ERROR_BUFFER_OVERFLOW"));
      resetBuffer();    // discard the buffer contents
    } else {
      char inChar = (char)Serial.read();
      if (inChar == EOS_TERMINATOR_CHAR) {
        stringComplete = true;
        break;
      }
      inputString[strPtr++] = inChar;
      inputString[strPtr] = 0;
    }
  }
}

void processCmd() {
  if (stringComplete) {
    parseBuffer();    // process the command
    resetBuffer();    // clear for the next command
  }
}

void resetBuffer() {
  inputString[0] = 0;    // discard the buffer contents
  strPtr = 0;
  stringComplete = false;
  baseCmd[0] = 0;
  pin = UNDEFINED;
  argValue = UNDEFINED;
}

//  USB command interface
//
//  When present, "#" refers to the Arduino pin number used in the operation
//  
//  ============  ========  =====================================================================
//  command       value     action
//  ============  ========  =====================================================================
//  ?ai pin       0..1023   returns current value of numbered analog input
//  ?bi pin       0..1      returns current value of numbered digital input
//  !bo pin v     0..1      sets numbered digital output to value v
//  !pwm pin v    0..255    sets numbered PWM digital output to value v
//  !pin pin v    0..1      sets mode of digital pin to value v (value: 1=OUTPUT, not 1=INPUT)
//  other         ..        returns "ERROR_UNKNOWN_COMMAND:text"
//  ============  ========  =====================================================================
//
// notes: must use lower case (as shown in table)
//        integers must be specified in decimal (no octal or hex interpreted)
//
void parseBuffer() {
  dissectCommand(inputString);
  if (strlen(baseCmd)) {
    if      (0 == strcmp(baseCmd, "?ai"))  readAI(inputString);
    else if (0 == strcmp(baseCmd, "?bi"))  readBI(inputString);
    else if (0 == strcmp(baseCmd, "!bo"))  writeBO(inputString);
    else if (0 == strcmp(baseCmd, "!pwm")) writePWM(inputString);
    else if (0 == strcmp(baseCmd, "!pin")) setPinMode(inputString);
    else {
      Serial.print(F("ERROR_UNKNOWN_COMMAND:"));
      Serial.println(inputString);
      resetBuffer();
    }
  }
}

/** dissectCommand
 *
 * parse the input "baseCmd pin [argValue]"
 */
void dissectCommand(char *source_string) {
  char *cmd;
  char buf[BUFFER_LENGTH+1];
  
  strcpy(buf, source_string);  // copy locally so we don't modify source
  
  cmd = strtok(buf, " ");
  strcpy(baseCmd, cmd);
  
  cmd = strtok(NULL, " ");
  if (cmd) {
    pin = atoi(cmd);

    cmd = strtok(NULL, " ");
    if (cmd) {
      argValue = atoi(cmd);
      
      cmd = strtok(NULL, " ");
      if (cmd) {
        Serial.print(F("ERROR_TOO_MANY_ARGUMENTS:"));
        Serial.println(source_string);
        resetBuffer();
      }
    }
  } else {
    Serial.print(F("ERROR_COMMAND_FORMAT:"));
    Serial.println(source_string);
    resetBuffer();
  }
}


void readAI(char* in) {
  // TODO: range check the pin first!  NUM_ANALOG_INPUTS
  // TODO: check 0..NUM_ANALOG_INPUTS
#ifdef TALKATIVE
  Serial.print(F("ai "));
  Serial.print(pin);
  Serial.print(F(" "));
#endif
  Serial.println(analogRead(pin));
}

void readBI(char* in) {
  // TODO: range check the pin first!  NUM_DIGITAL_PINS
  // TODO: check 0..NUM_DIGITAL_PINS
#ifdef TALKATIVE
  Serial.print(F("bi "));
  Serial.print(pin);
  Serial.print(F(" "));
#endif
  Serial.println(digitalRead(pin));
}

void writeBO(char* in) {
  if (argValue < 0 || argValue > 1) {
    Serial.print(F("ERROR_BINARY_RANGE:"));
    Serial.println(inputString);
    resetBuffer();
  } else {
    // TODO: range check the pin first and make sure it is OK for output!   NUM_DIGITAL_PINS
	// TODO: check 0..NUM_DIGITAL_PINS
    digitalWrite(pin, argValue);
#ifdef TALKATIVE
    Serial.print(F("bo "));
    Serial.print(pin);
    Serial.print(F(" "));
    Serial.println(argValue);
#else
  Serial.println(F("Ok"));
#endif
  }
}

void writePWM(char* in) {
  if (argValue < PWM_MIN_VALUE || argValue > PWM_MAX_VALUE) {
    Serial.print(F("ERROR_PWM_RANGE:"));
    Serial.println(inputString);
    resetBuffer();
  } else {
    // TODO: range check the pin first and make sure it is OK for PWM output!
	// TODO: check 0..NUM_DIGITAL_PINS
	if (digitalPinHasPWM(pin)) {
      analogWrite(pin, argValue);
#ifdef TALKATIVE
      Serial.print(F("pwm "));
      Serial.print(pin);
      Serial.print(F(" "));
      Serial.println(argValue);
#else
	  Serial.println(F("Ok"));
#endif
    } else {
      Serial.print(F("ERROR_PIN_NOT_PWM:"));
      Serial.println(inputString);
      resetBuffer();
	}
  }
}

void setPinMode(char* in) {
  // TODO: check 0..NUM_DIGITAL_PINS
  argValue = (argValue == 1) ? OUTPUT : INPUT;
  pinMode(pin, argValue);
    // TODO: range check the pin first and make sure it is OK for output!   NUM_DIGITAL_PINS
	// TODO: check 0..NUM_DIGITAL_PINS
#ifdef TALKATIVE
  Serial.print(F("pin "));
  Serial.print(pin);
  Serial.print(F(" "));
  Serial.println(argValue);
#else
  Serial.println(F("Ok"));
#endif
}

//########## SVN repository information ###################
// $Date$
// $Author$
// $Revision$
// $URL$
// $Id$
//########## SVN repository information ###################

