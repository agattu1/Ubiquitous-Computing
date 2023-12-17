  #ifndef CAPACTIVE_H
  #define CAPACTIVE_H

//Reasoning for above:
//The purpose is to prevent the header file from being included more than once in the same translation unit, which can lead to issues like multiple definitions. 
//The _h is just a naming convention to make it clear that it's related to a header file. It helps prevent naming clashes with other identifiers in your program.
  
  
  #include <Arduino.h>
  
  
  class Capactive {
    public:
      // Constructor
      Capactive(int touchPin);
      // Functions
      bool isSecure();
  
    private:  
      // Constructor Variables
      int touchPin;

      // Other Variables
      float CAPACITANCE_THRESHOLD = 100000; 
  };
  
  #endif
