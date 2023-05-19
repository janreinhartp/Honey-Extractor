//     if (digitalRead(spinHome) == 0)
//     {
//         isSpinnerHome = true;
//     }

//     btnSpinner.update();
//    if (isSpinnerHome == true && homing == false && isSpinnerMoving == false)
//     {
//         if (btnSpinner.isSingleClick())
//         {
//             Serial.println("Single Spinner");
//             isSpinnerMoving = true;
//             TimerSpinner.start();
//         }
//     }
//     if (homing == false && isSpinnerMoving == false)
//     {
//         if (btnSpinner.isDoubleClick())
//         {
//             Serial.println("Homing Spinner is Clicked");
//             homing = true;
//         }
//     }

//     if (homing == true)
//     {
//         homeSpinner();
//     }
//     else if (isSpinnerMoving == true)
//     {
//         runSpinner();
//     }

//     if (isChainMoving == false && isSpinnerHome == true && digitalRead(lidSensor) == 1)
//     {
//         btnSpinner.update();
//         if (btnSpinner.isSingleClick())
//         {
//             Serial.println("Single Chain");
//         }
//         if (btnSpinner.isDoubleClick())
//         {
//             Serial.println("Double Chain");
//         }
//     }