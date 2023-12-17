#include "Capactive.h"

/*
 * @brief Constructor for MagneticContact.
 * @param pin The pin number to be used for the switch
 */
Capactive::Capactive(int touchPin)
{
  // Set the switch pin as input with pull-up resistor
  this->touchPin = touchPin;
  touchRead(touchPin);
}

/*
 * @brief Checks if the magnetic contact switch is secured
 * @return  Status of contact switch
 */
bool Capactive::isSecure() {
  // Read and check the statea of pin and print the corresponding message

    //Serial.println(touchRead(touchPin));
  // put your main code here, to run repeatedly:

  if (touchRead(touchPin) < CAPACITANCE_THRESHOLD) {
    //Serial.println("DoorKnob Tocuhed");
    return true;
  } else {
    //Serial.println("DoorKnob Untouched");
    return false;
  }
}
