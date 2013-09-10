/*
 * cmd_response
 *
 * Simple USB command/response handling for Arduino I/O capabilities
 */

#include "Cmd_Response.h"

#define USB_BAUD 115200

Cmd_Response cr_handler;

void setup() {
  Serial.begin(USB_BAUD);
  // TODO: configure & document pins for analogWrite and digitalWrite
}

void loop() {
  cr_handler.processCmd();
}

void serialEvent() {
  cr_handler.readBuffer();
}

//########## SVN repository information ###################
// $Date$
// $Author$
// $Revision$
// $URL$
// $Id$
//########## SVN repository information ###################

