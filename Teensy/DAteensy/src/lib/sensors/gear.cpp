#include "gear.h"
namespace GEAR {
  void initGear(){
      for(int i=0; i<7; i++){
        pinMode(i, INPUT);
      }
  }

  int getGear(){
    for(int i=0; i<7; i++){
      if (!digitalRead(i)){
        return 6-i;
      }
    }
    return 255;
  }

  uint8_t desired_gear = 128;
  uint8_t getDesiredGear(){
    return desired_gear;
  }

  void setDesiredGear(uint8_t gear){
    desired_gear = gear;
  }


}

