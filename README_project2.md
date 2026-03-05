The goal of this practical assignment is to write a program that implements the functionalities of a game called Super Bit Smasher. In this game, the player initially 
receives two numbers: one is the target value and the other is the starting value. With this, the player has two bitwise operations available during the game. 
The player must enter a number and perform one of the two available operations on the starting value, using the buttons, in order to achieve victory, which is to match 
the target value with the value being calculated. To make the game more challenging, we also use 4 LEDs to show the remaining time the player has.

To create this game, I began by thinking about how to implement certain functionalities so that everything would work as expected. However, during development, I 
encountered a problem with the “while” loops. 
In my code, to prevent the message “Introduza um valor:” from appearing repeatedly until the player enters a number in the serial monitor, I created a “while” loop 
for the serial available. However, one of the problems that took me the longest to solve was how to ensure that, if the time runs out or the player decides to restart 
the game, the “while” loop breaks so that the variable “jogoAtivo” can break the main loop’s while and start a new round. Despite this, the solution turned out to be 
quite simple: I created two boolean variables called “contar” and “reiniciar”, one for restarting the game and the other for time counting. In the conditions of the 
serial available “while”, I added these two variables, ensuring that as long as they were false, the loop would not break. If the time ran out or the player wanted to 
restart the game, these variables were updated to true, thus breaking the serial available loop. This same issue also occurred with the buttons, since I also have a 
“while” loop to check whether a button is pressed, and I solved it using the same approach.
Other functions I created for the proper functioning of the game include the initial function “nova_ronda()”, where the LED timer begins and where I define the target 
value and the starting value using two functions, “valor_base()” and “valor_target()”, each generating a random value with a maximum of 8 bits. I also have a function 
that reads the serial monitor to check the value entered by the player, called “numero_inserido()”.
I also created 3 functions for the 3 buttons, each performing one of the bitwise operations. These 3 functions are then used inside another function called 
“botao_pressionado()”, which uses debounce to determine which button was actually pressed and execute the corresponding operation. Another important function is 
“bit1()”, which checks whether bit 1 is active or not. To count the time of each round using the LEDs, I created the function “contagem_tempo()”. If the player wants 
to restart the game, then the function “reiniciar_jogo()” is executed. To ensure that each round starts without issues, the game variables must be reset, and for that 
I created the function “reset_jogo()”.
The most important function in the code is “ojogo()”, which essentially brings together several other functions so that the core of the game works. For example, this 
is where I define which buttons are available to be pressed using the “bit1()” function.

 - Some gaps: In the “bit1()” function, I used the number two to check whether the button is active because bit 1 corresponds to 2^1=2. 
   This means it cannot be the number 1, since that value only has bit 0 active (0b00000001. In other words, if I used the number 1 to check whether the bit is
   active, I would be checking whether bit 0 is active. However, what we want to verify is whether bit 1 is active, so I used the number 2.
