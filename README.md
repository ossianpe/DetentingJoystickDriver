DetentingJoystickDriver
=======================

This is a joystick Driver for controlling Ardunio Leonardo with HBridge and FSRs. For more information about this project please visit:

http://www.egr.msu.edu/classes/ece480/capstone/fall12/group04/index.html

Basic Features:
The program supports many features beyond the scope of design criteria.  Such features expand functionality for the user and allow for large control over the system.  Several of the additional features included in the code are:

•	Arrow key mode—use of arrow keys instead of mouse movement<br>
•	Five feedback controls<br>
•	Time Delay<br>
•	Position<br>
•	Kinetic compensation<br>
•	Overshoot<br>
•	Joystick overcompensates and then positions accurately, much like kinetic scrolling<br>
•	Undershoot<br>
•	The opposite direction of overshooting<br>
•	Stress<br>
•	The force required to move joystick to new position<br>
•	Active<br>
•	Provides force opposing user’s force, used for decreasing play in actuator, increasing strength used to move joystick by another means, provides more feedback immediately<br>
•	Five pedal click controls<br>
•	Single click<br>
•	Pedal release<br>
•	Single click with pedal release<br>
•	Single click with iterative output<br>
•	Single click with pedal release and iterative output<br> 
•	Delays<br>
•	Cursor<br>
•	Keyboard<br>
•	Pedal<br>
•	Axis orientation

A list of serial reprogrammable commands is as followed:

•	D – redefine number of detents<br>
•	S – specify force threshold value for moving joystick into next position<br>
•	P – redefine position feedback<br>
•	Used for realigning joystick if moved out of center<br>
•	C – redefine cursor delay value between each mouse movement<br>
•	E – redefine cursor and pedal delay value<br>
•	I – redefine pedal click between each left mouse click<br>
•	K – toggle keyboard arrow key mode<br>
•	M – toggle cursor movement<br>
•	X – redefine X axis orientation<br>
•	Y – redefine Y axis orientation

Additional Features:
Many additional features have been included beyond the design scope of the primary objectives for the joystick.  The addition of features allows for the user to select how the joystick operates to specific usage scenarios.  These customizations range in many different features.
The joystick has an optional mode of outputting keyboard  arrow key navigation with or without cursor support.  This feature can allow the joystick to function under conventional control—isolated to a single window and provide equal response with every on screen movement.  This feature can be used with mouse support as well and ported to any other form of input on a computer.
One large area of aided support is for delays.  Many delay controls are implemented throughout the code.  Some delay parameters work across multiple functions, ones that cannot operate at the same time, such as the five selectable mouse routines while others scale to each individual operation such as mouse, keyboard, or pedal delays.
A list of delay features available in the code are as followed:

•	Cursor<br>
•	Keyboard<br>
•	Pedal<br>
•	Cursor & Pedal<br>
•	Joystick linkage:<br>
•	Movement between detents<br>
•	Feedback:<br>
•	Overshoot/undershoot

Five available feedback functions exist in the code.  As implied earlier, many of these features are provided to expand positioning, but also allow for customizable feel over joystick movements.  Each feature is modular and most are activated passively (with the exclusion of active feedback).  A description of each is provided as followed:

•	Time Delay<br>
•	Position<br>
•	Overshoot/undershoot<br>
•	Stress<br>
•	Active

In addition five mouse modes exist as well.  The purpose of the additional mouse modes is for users selection of what feels most comfortable/useable.  A list of mouse functions and brief overview is as followed:

•	Click with user reprogrammable delay<br>
•	Click once until pedal is released<br>
•	Click once and waits for reprogrammable delay and then outputs a long click under the pedal is released<br>
•	Click once and waits for release and if within reprogrammable delay will output a long click<br>
•	Click once and waits for release and if within reprogrammable delay will output a long click; Also works for a click and waits for reprogrammable delay and then outputs a long click under the pedal is released
