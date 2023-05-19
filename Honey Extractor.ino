// Motor
#include "control.h"
// Sensor / Button Debounce Library
#include <PinButton.h>

Control Chain(4, 100, 101);
Control Flopper(3, 100, 101);
Control TimerSpinner(100, 100, 100);
Control MotorBrake(10, 100, 100);
Control SpinnerSlow(A3, 100, 100);
Control SpinnerFast(A2, 100, 100);

PinButton btnStart(A4);
PinButton btnChain(A5);

const int sensorTray = 8;
const int flopperHome = 6;
const int spinHome = 9;
const int chainHome = 7;
const int lidSensor = 5;

bool isSpinnerHome = false;
bool homing = false;

int counterTray = 0;
int status = 0;
void finish()
{
    counterTray = 0;
    status = 0;
}

bool delayChainFlag = false;

// THIS METHOD RUN THE SPINNER FAST WITH TIMER
void runSpinner()
{
    TimerSpinner.run();
    if (TimerSpinner.isTimerCompleted() == true) // CHECKING IF THE TIMER IS COMPLETE
    {
        SpinnerFast.relayOff();
        MotorBrake.relayOn();
        finish();
    }
    else
    {
        SpinnerFast.relayOn();
        MotorBrake.relayOff();
    }
}

// THIS METHOD RUN THE HOMING SEQUENCE OF BASKET
void homeSpinner()
{
    if (digitalRead(spinHome) == 1) // CHECKING IF THE BASKET IS HOMED
    {
        SpinnerSlow.relayOn();
    }
    else
    {
        SpinnerSlow.relayOff();
    }
}

bool trayStat, flopperFLag = false;
// THIS METHOD RUN THE CHAIN AND DECAPPING WITH TRAY COUNTER
void moveChain()
{
    if (trayStat == false && flopperFLag == false) // CHECKING IF THE TRAY STATUS IS EMPTY AND FLOPPER IS FALSE ON MOVE
    {
        if (digitalRead(sensorTray) == 0) // CHECKING IF THE TRAY IS DETECTED
        {
            trayStat = true;
            delay(2000);
        }
    }
    else if (flopperFLag == true && trayStat == false) // THIS ELSE IF RUN THE FLOPPER
    {
        if (flopperFLag == true) // CHECKING IF THE FLOPPER FLAG IS TRUE
        {
            moveFlopper();
        }
    }
    else if (trayStat == true && flopperFLag == false) // CHECKING IF TRAYSTAT IS TRUE AND RUN THE CHAIN
    {

        if (counterTray < 4 && flopperFLag == false) // THIS IF RUN THE CHAIN AND COUNT THE TRAY
        {
            Chain.relayOn();
            if (digitalRead(chainHome) == 0)
            {
                delay(2000);
                Chain.relayOff();
                counterTray += 1;
                trayStat = false;
                flopperFLag = true;
            }
        }
        else // WHEN THE TRAY COUNT IS EQUAL TO 4 THE MOTOR SPINNER TIMER START
        {
            TimerSpinner.start();
            status = 2;
        }
    }
}

void moveFlopper() // MOVES THE FLOPPER AND HOME IT
{

    Flopper.relayOn();
    if (digitalRead(flopperHome) == 1)
    {
        Flopper.relayOff();
        flopperFLag = false;
    }
}

void setup() // SETUP FOR PINS
{

    pinMode(spinHome, INPUT_PULLUP);
    pinMode(lidSensor, INPUT_PULLUP);
    pinMode(chainHome, INPUT_PULLUP);
    pinMode(flopperHome, INPUT_PULLUP);
    pinMode(sensorTray, INPUT_PULLUP);
    TimerSpinner.setTimer("001000");
    Serial.begin(9600);
}

void loop()
{
    btnStart.update(); // THIS IS THE CALL FOR BUTTON UPDATES


    if (digitalRead(lidSensor) == 0) // THIS FUNCTION CHECKS IF THE LID IS CLOSED
    {
        SpinnerSlow.relayOff();
        SpinnerFast.relayOff();
        Chain.relayOff();
        Flopper.relayOff();
        MotorBrake.relayOff();
        trayStat = false;
    }

    if (digitalRead(lidSensor) == 1 && status == 0) // IF THE LID IS CLOSE AND THE STATUS IS 0 IT ENABLE THE CLICKING OF THE BUTTON
    {
        if (digitalRead(spinHome) == 0) // ACTIVATE THE BRAKING OF THE BASKET IF THE BASKET IS HOMED
        {
            MotorBrake.relayOn();
            isSpinnerHome = true;
            Serial.println("Homing");
        }
        else
        {
            isSpinnerHome = false;
            MotorBrake.relayOff();
        }

        if (isSpinnerHome == false) // THIS SNIPPET IS THE HOMING SEQUENCE ONCE THE LID IS CLOSE
        {
            homeSpinner();
            if (digitalRead(spinHome) == 0)
            {
                MotorBrake.relayOn();
                isSpinnerHome = true;
            }
        }
        else // THIS ELSE CHECK THE BUTTON IS CLICK AND START THE SEQUENCE OF CHAIN, FLOPPER AND SPINNER
        {
            if (btnStart.isSingleClick())
            {
                status = 1;
            }
        }
    }
    else if (digitalRead(lidSensor) == 1 && status != 0) // THIS BLOCK OF CODE CHECK IF THE LID IS CLOSE AND RUN THE SEQUENCE OF THE MACHINE
    {
        switch (status) // THIS SWITCH CASE CHECK IF THE MACHINE IS IN WHAT STATE AND RUN THE CORRESPONDING METHODS
        {
        case 1:
            moveChain(); // CALLING OF CHAIN METHOD
            break;
        case 2:
            runSpinner(); // CALLING OF SPINNER METHOD
            break;
        default:
            status = 0; // RETURN THE STATUS TO NOTHING
            break;
        }
    }
}