# Arduino Smart parking lot
This was my bachelor thesis in which I made smart parking lot using the Arduino microcontroller.

<br>There are two servo motors that control the entrance and exit from the lot. You can only enter the lot if you have appropriate RFID card. LCD display shows how many spots are available (max 4) and if there are non the entrance will be closed.
<br>Timer is implemented at the entrance gate so if you scan the appropriate card and decide to leave after a certain amount of time gate will close automatically.

<br>Exit from the lot is controlled with a push button. If you wanna leave you need to push it and the servo will open the gate.

<br>There are also 4 IR sensors (2 for entrance and 2 for exit) that make sure that the car is actually present and that the car has actually entered/left so that the counter can be incremented/decremented.
