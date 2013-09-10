/*
 * Cmd_Response
 *
 * Simple USB command/response handling for Arduino I/O capabilities
 */

#ifndef cmd_response_h
#define cmd_response_h

#include <Arduino.h>

#define BUFFER_LENGTH 200
#define EOS_TERMINATOR_CHAR '\n'

#define ADC_MAX_VALUE 1023
#define ADC_MIN_VALUE 0
#define PWM_MAX_VALUE 255
#define PWM_MIN_VALUE 0
#define IGNORE_FINISH -1

class Cmd_Response
{
  public:
    Cmd_Response();
    void readBuffer();         // read from the USB port
    void processCmd();         // handle one line
    
    String inputString;        // a string to hold incoming data
    boolean stringComplete;    // whether the string is complete

  private:
    void reset();              // empty the inputString and clear stringComplete
    void parse();              // parse and handle one command
    int sstoi(String s, int start, int finish);  // substring to integer

    void readADC(String s);
    void readBI(String s);
    void writeBO(String s);
    void writePWM(String s);
};

#endif

//########## SVN repository information ###################
// $Date$
// $Author$
// $Revision$
// $URL$
// $Id$
//########## SVN repository information ###################

