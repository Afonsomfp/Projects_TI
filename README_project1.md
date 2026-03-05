The goal of this practical work is to build a version of the famous game Whac-A-Mole on Arduino. To do so, it was necessary to make an adaptation using LEDs, the objective of which is to light them all up, so that we can achieve victory.

To create this game, I began by making a mental map of it to understand how to implement certain functions needed for the proper operation of my program. However, 
throughout the development process, I encountered some difficulties.
One of them was how to implement the part where the button must be pressed for 2 seconds. In the end, I managed to implement this condition. I created a boolean 
function to check whether the button was pressed for 2 seconds (“verificapressaobotao()”). This is where I used debounce, which serves to check, within a short 
period of time (debounceDelay), whether the button is truly pressed. Additionally, with the help of an if statement, I checked whether the button state had changed 
and whether 2 seconds had passed since that change. If this condition is true, then the variable “botaopressionado” becomes true, and I return that value so the code 
can start the game.
Another challenging part was creating the game itself. That is, making the LEDs blink randomly until the button is pressed at the exact moment the LED is lit, and 
if that happens, keeping the LED on until the variable “jogoiniciado” is no longer true. For this, I created the function “ojogo()”. In this function, using several 
variables and another function called “numeroaleatorio()”, I generate a random number that becomes “k”, meaning it corresponds to the LED pin. With an if statement, 
I check whether that “k” is lit. If it is not, I turn it on. Otherwise, if “k” is already on, I turn it off.
To check whether the button state changed during the game—that is, whether it was pressed—I also used an if statement. If this condition is true, I check whether 
the LED is lit. If it is, I store that LED as lit inside the variable “leds”. If the condition is not met, then I turn that LED on.
Next, I created a for loop that goes through all LED pins and checks whether each LED should be on according to the “leds” variable or if it is the currently active LED.
In other words, the LED is turned on if it is already marked as on inside the “leds” variable (a correctly hit LED) or if it is the current LED. This keeps the 
LED lit until the end of the game.
Other functions I created and used include “piscarled(int num)”, which makes the LEDs blink the desired number of times; “verificavitoria()”, which checks whether all 
LEDs are lit so that victory can be achieved; and finally, the “reiniciarjogo()” function, which—as the name suggests—restarts the game if the button is pressed for 2 seconds.
Finally, for the game to run, these functions must be placed inside a loop; otherwise, the program would run only once. For this, we use the “loop()” function. Inside 
it, I created a condition: if the button is pressed for 2 seconds and the game has not yet started, then the game begins, and the LEDs blink 3 times to indicate 
the start. After the game begins, I run the “ojogo()” function and the “verificavitoria()” function. However, if the button is pressed for 2 seconds again, the game is 
restarted using the “reiniciarjogo()” function. If it is pressed again for the same duration, “ojogo()” and “verificavitoria()” run again, and so on.

 - Some gaps: Unfortunately, my program has a bug that I was not able to solve. After the game starts—that is, after the LEDs blink 3 times—one of the LEDs, chosen 
randomly, is immediately classified as a correct hit. In other words, right after the game begins, one of the LEDs remains lit, without the button being pressed, 
until the variable "jogoiniado" stops being true.
However, in an attempt to fix the problem, after the LEDs blinked 3 times, I tried resetting the button state, since I thought that pressing the button for 2 seconds 
might have affected something. This did solve that issue, but it created another one with the “reiniciarjogo()” function, which simply stopped executing.
I then had the idea of turning off all LEDs after they blinked 3 times, but this also did not work and did not change anything in the program’s behavior.
