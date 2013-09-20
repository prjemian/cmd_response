/** cmd_response
 *
 * simple command/response access via USB to Arduino's I/O capabilities
 */

#define SOFTWARE_VERSION       2
#define SOFTWARE_ID            "cmd_response"

#define BUFFER_LENGTH          40
#define COMMAND_LENGTH         16
#define EOS_TERMINATOR_CHAR    '\n'
#define USB_BAUD               115200

#define UNDEFINED              -1

#define ADC_MAX_VALUE 1023
#define ADC_MIN_VALUE 0
#define PWM_MAX_VALUE 255
#define PWM_MIN_VALUE 0

#define PERIOD_DEFAULT_ms  1000
#define PERIOD_MIN_ms      5
#define PERIOD_MAX_ms      1000000

#define MULTIPLIER_DEFAULT  1000
#define MULTIPLIER_MIN      1
#define MULTIPLIER_MAX      1000000

char inputString[BUFFER_LENGTH+1];
char strPtr;
boolean stringComplete;
long arg1;
long arg2;
char baseCmd[COMMAND_LENGTH+1];

long period = PERIOD_DEFAULT_ms;
long multiplier = MULTIPLIER_DEFAULT;
int ai_watched[NUM_ANALOG_INPUTS];
long ai_sums[NUM_ANALOG_INPUTS];
float ai_mean[NUM_ANALOG_INPUTS];
long count, rate;
long nextUpdate;

void setup() {
  resetBuffer();
  Serial.begin(115200);
  Serial.print(F("cmd_response started: "));
  Serial.println(freeRam());
  for (int i = 0; i < NUM_ANALOG_INPUTS; i++) {
    ai_watched[i] = 0;    // by default, do not monitor
    ai_sums[i] = 0;
    ai_mean[i] = 0;
  }
  count = 0;
  rate = 0;
  nextUpdate = millis() + period;
}

void loop() {
  signalGathering();
  readBuffer();
  processCmd();
}

void signalGathering() {
  // periodically, average the watched ADC channels
  if (millis() >= nextUpdate) {
    // Serial.print("count"); Serial.print("  "); Serial.println(count);
    for (int i = 0; i < NUM_ANALOG_INPUTS; i++) {
      if (ai_watched[i] && count > 0) {    // if monitoring
        // Serial.print(i); Serial.print("  "); Serial.println(float(ai_sums[i]));
        ai_mean[i] = float(ai_sums[i]) / count;
        ai_sums[i] = 0;
      }
    }
    nextUpdate = millis() + period;
    rate = 1000 * count / period;    // loops per second
    count = 0;
  }
  // this is where the ADC channels are read
  for (int i = 0; i < NUM_ANALOG_INPUTS; i++) {
    if (ai_watched[i])    // if monitoring
      ai_sums[i] += analogRead(i);
  }
  count++;
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
    executeCommand();    // process the command
    resetBuffer();    // clear for the next command
  }
}

void resetBuffer() {
  inputString[0] = 0;    // discard the buffer contents
  strPtr = 0;
  stringComplete = false;
  baseCmd[0] = 0;
  arg1 = UNDEFINED;
  arg2 = UNDEFINED;
}

//  USB command interface of form: baseCmd [arg1 [arg2]]
//
// char* baseCmd (lower case)
// long arg1, arg2 (no octal or hex interpreted)

void executeCommand() {
  dissectCommand(inputString);
  if (strlen(baseCmd)) {
    if      (0 == strcmp(baseCmd, "?ai"))       readAI(inputString);
    else if (0 == strcmp(baseCmd, "?bi"))       readBI(inputString);
    else if (0 == strcmp(baseCmd, "!bo"))       writeBO(inputString);
    else if (0 == strcmp(baseCmd, "!pwm"))      writePWM(inputString);
    else if (0 == strcmp(baseCmd, "!pin"))      setPinMode(inputString);
    else if (0 == strcmp(baseCmd, "?#ai"))      get_num_ai_channels(inputString);
    else if (0 == strcmp(baseCmd, "?#bi"))      get_num_bi_channels(inputString);
    else if (0 == strcmp(baseCmd, "!t"))        set_period(inputString);
    else if (0 == strcmp(baseCmd, "?t"))        get_period(inputString);
    else if (0 == strcmp(baseCmd, "?t:min"))    get_period_min(inputString);
    else if (0 == strcmp(baseCmd, "?t:max"))    get_period_max(inputString);
    else if (0 == strcmp(baseCmd, "!k"))        set_multiplier(inputString);
    else if (0 == strcmp(baseCmd, "?k"))        get_multiplier(inputString);
    else if (0 == strcmp(baseCmd, "?k:min"))    get_multiplier_min(inputString);
    else if (0 == strcmp(baseCmd, "?k:max"))    get_multiplier_max(inputString);
    else if (0 == strcmp(baseCmd, "!ai:watch")) watch_ai_channel(inputString);
    else if (0 == strcmp(baseCmd, "?ai:mean"))  readAI_mean(inputString);
    else if (0 == strcmp(baseCmd, "?v"))        get_software_version(inputString);
    else if (0 == strcmp(baseCmd, "?id"))       get_software_id(inputString);
    else if (0 == strcmp(baseCmd, "?rate"))     get_loop_rate(inputString);
    else {
      Serial.print(F("ERROR_UNKNOWN_COMMAND:"));
      finalizeError(inputString);
    }
  }
}

/** dissectCommand
 *
 * parse the input "baseCmd arg1 [arg2]"
 */
void dissectCommand(char *source_string) {
  char *cmd;
  char buf[BUFFER_LENGTH+1];
  
  strcpy(buf, source_string);  // copy locally so we don't modify source
  
  cmd = strtok(buf, " ");
  strcpy(baseCmd, cmd);
  
  cmd = strtok(NULL, " ");
  if (cmd) {
    arg1 = atoi(cmd);

    cmd = strtok(NULL, " ");
    if (cmd) {
      arg2 = atoi(cmd);
      
      cmd = strtok(NULL, " ");
      if (cmd) {
        Serial.print(F("ERROR_TOO_MANY_ARGUMENTS:"));
        Serial.println(source_string);
        resetBuffer();
      }
    }
  }
}


void finalizeError(char *in) {
  Serial.println(in);
  resetBuffer();
}

void readAI(char* in) {
  if (arg1 < 0 || arg1 > NUM_ANALOG_INPUTS) {
    Serial.print(F("ERROR_AI_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else {
    Serial.println(analogRead(arg1));
  }
}

void readBI(char* in) {
  if (arg1 < 0 || arg1 > NUM_DIGITAL_PINS) {
    Serial.print(F("ERROR_BI_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else {
    Serial.println(digitalRead(arg1));
  }
}

void writeBO(char* in) {
  if (arg2 < 0 || arg2 > 1) {
    Serial.print(F("ERROR_BINARY_RANGE:"));
    finalizeError(in);
  } else if (arg1 < 0 || arg1 > NUM_DIGITAL_PINS) {
    // TODO: but did we set the pin for output?
    Serial.print(F("ERROR_BO_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else {
    digitalWrite(arg1, arg2);
    Serial.println(F("Ok"));
  }
}

void writePWM(char* in) {
  if (arg2 < PWM_MIN_VALUE || arg2 > PWM_MAX_VALUE) {
    Serial.print(F("ERROR_PWM_RANGE:"));
    finalizeError(in);
  } else if (!digitalPinHasPWM(arg1)) {
    // TODO: but did we set the pin for output?
    Serial.print(F("ERROR_PIN_NOT_PWM:"));
    finalizeError(in);
  } else {
    analogWrite(arg1, arg2);
    Serial.println(F("Ok"));
  }
}

void get_num_ai_channels(char* in) {
  Serial.println(NUM_ANALOG_INPUTS);
}

void get_num_bi_channels(char* in) {
  Serial.println(NUM_DIGITAL_PINS);
}

void set_period(char* in) {
  if (arg1 < PERIOD_MIN_ms || arg1 > PERIOD_MAX_ms) {
    Serial.print(F("ERROR_PERIOD_RANGE:"));
    finalizeError(in);
  } else {
    period = arg1;
    Serial.println(F("Ok"));
    nextUpdate = millis();    // trigger a new update immediately
  }
}

void get_period(char* in) {
  Serial.println(period);
}

void get_period_min(char* in) {
  Serial.println(PERIOD_MIN_ms);
}

void get_period_max(char* in) {
  Serial.println(PERIOD_MAX_ms);
}

void set_multiplier(char* in) {
  if (arg1 < MULTIPLIER_MIN || arg1 > MULTIPLIER_MAX) {
    Serial.print(F("ERROR_MULTIPLIER_RANGE:"));
    finalizeError(in);
  } else {
    multiplier = arg1;
    Serial.println(F("Ok"));
  }
}

void get_multiplier(char* in) {
  Serial.println(multiplier);
}

void get_multiplier_min(char* in) {
  Serial.println(MULTIPLIER_MIN);
}

void get_multiplier_max(char* in) {
  Serial.println(MULTIPLIER_MAX);
}

void get_software_version(char* in) {
  Serial.println(SOFTWARE_VERSION);
}

void get_software_id(char* in) {
  // this is the ONLY string output that is not an error
  Serial.println(F(SOFTWARE_ID));
}

void get_loop_rate(char* in) {
  Serial.println(rate);
}

void watch_ai_channel(char* in) {
  // configure pin to be averaged (or not depending on arg2)
  if (arg1 < 0 || arg1 > NUM_ANALOG_INPUTS) {
    Serial.print(F("ERROR_AI_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else {
    ai_watched[arg1] = (arg2 == 0 ? 0 : 1);
    Serial.println(F("Ok"));
  }
}

void readAI_mean(char* in) {
  if (arg1 < 0 || arg1 > NUM_ANALOG_INPUTS) {
    Serial.print(F("ERROR_AI_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else if (!ai_watched[arg1]) {
    Serial.print(F("ERROR_AI_PIN_NOT_WATCHED:"));
    finalizeError(in);
  } else {
    Serial.println(long(ai_mean[arg1] * multiplier));
  }
}

void not_implemented_yet(char* in) {
  Serial.print(F("ERROR_NOT_IMPLEMENTED_YET:"));
  finalizeError(in);
}

void setPinMode(char* in) {
  if (arg1 < 0 || arg1 > NUM_DIGITAL_PINS) {
    // TODO: but did we set the pin for output?
    Serial.print(F("ERROR_DIGITAL_PIN_NOT_AVAILABLE:"));
    finalizeError(in);
  } else {
    pinMode(arg1, (arg2 == 1) ? OUTPUT : INPUT);
    Serial.println(F("Ok"));
  }
}

//########## SVN repository information ###################
// $Date$
// $Author$
// $Revision$
// $URL$
// $Id$
//########## SVN repository information ###################

