#include "buttons.h"

namespace BUTTONS {

    // class Button
    Button::Button(uint8_t pin) : _pin(pin) {}

    void Button::begin() {
        pinMode(_pin, INPUT);
        _state = digitalRead(_pin);
    }

    void Button::setPressed( TransFuncPtr func){
        onButtonPressed = func;
    }
    void Button::setReleased(TransFuncPtr func){
        onButtonReleased = func;
    }
    void Button::setLongPressed(TransFuncPtr func){
        onButtonLongPressed = func;
    }



    void Button::events() {
        bool v = digitalRead(_pin);
        if (v != _state) {
            _state = v;
            if (_state) {
                if(onButtonReleased){
                    if (time_flag){
                    onButtonReleased();
                    }
                };
                return;
            } else {
                if(onButtonPressed){
                    onButtonPressed();
                }
                time = millis();
                time_flag = true;

            }
        } else if (!v && millis() - time > 5000){
            if (time_flag){
                if (onButtonLongPressed){
                    onButtonLongPressed();
                }
                time_flag = false;
            }

        }
        return;
    }

    void onRedButtonLongPress(){

        OBD2::clearDTC();
        DISPLAYY::setMainScreen();

    }

    uint8_t CURRENT_SCREEN = 0;

    void onRedButtonPress(){
    if (CURRENT_SCREEN == 0){

        OBD2::readDTC();
        DISPLAYY::setDebugScreen();
        CURRENT_SCREEN = 1;
    } else if (CURRENT_SCREEN == 1){
        DISPLAYY::setSensorScreen();
        CURRENT_SCREEN = 2;
    } else {
        DISPLAYY::setMainScreen();
        CURRENT_SCREEN = 0;
    }
}

void onGreenButtonPress(){
    // change gear to be send
    uint8_t actual = GEAR::getDesiredGear();
    if (actual == 128){
        actual = 0;
    } else if (actual == 255){
        actual = 128;
    } else if (actual == 6){
        actual = 255;
    } else {
        actual++;
    }
    GEAR::setDesiredGear(actual);

}


    BUTTONS::Button green_button(GREEN_BUTTON);
    BUTTONS::Button red_button(RED_BUTTON);

    void initButtons(){
        green_button.begin();
        green_button.setPressed(onGreenButtonPress);

        red_button.begin();
        red_button.setReleased(onRedButtonPress);
        red_button.setLongPressed(onRedButtonLongPress);

    }

    void checkButtons(){
        green_button.events();
        red_button.events();

    }

}




/*
uint32_t last_time_green_pressed = 0;
uint32_t last_time_red_pressed = 0;
bool previous_red = false;

void checkbuttons(){
    // if button pressed
    if (digitalRead(GREEN_BUTTON) == 0 && isContact()){
        if (millis() - last_time_green_pressed < 1000){
            return;
        }
        clearDTC();
        setMainScreen();
        previous_red = false;
        last_time_green_pressed = millis();
    }

        if (digitalRead(RED_BUTTON) == 0 && isContact()){
        if (millis() - last_time_red_pressed < 1000){
            return;
        }
        if (!previous_red){

            readDTC();
            //digitalWrite(DEBUG_LED, HIGH);
            setDebugScreen();
            previous_red = true;
        } else {
            //digitalWrite(DEBUG_LED, LOW);
                setMainScreen();
            previous_red = false;
        }

        last_time_red_pressed = millis();
    }


}
*/