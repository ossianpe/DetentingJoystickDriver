DetentingJoystickDriver
=======================
This is a joystick Driver for controlling Ardunio Leonardo with HBridge and FSRs. For more information about this project please visit:

http://www.egr.msu.edu/classes/ece480/capstone/fall12/group04/index.html

Basic Features:
The program supports many features beyond the scope of design criteria.  Such features expand functionality for the user and allow for large control over the system.  Several of the additional features included in the code are:
•	Arrow key mode—use of arrow keys instead of mouse movement
•	Five feedback controls
•	Time Delay
•	Position
•	Kinetic compensation
•	Overshoot
•	Joystick overcompensates and then positions accurately, much like kinetic scrolling
•	Undershoot
•	The opposite direction of overshooting
•	Stress 
•	The force required to move joystick to new position
•	Active
•	Provides force opposing user’s force, used for decreasing play in actuator, increasing strength used to move joystick by another means, provides more feedback immediately
•	Five pedal click controls
•	Single click
•	Pedal release
•	Single click with pedal release
•	Single click with iterative output
•	Single click with pedal release and iterative output 
•	Delays
•	Cursor
•	Keyboard
•	Pedal
•	Axis orientation
A list of serial reprogrammable commands is as followed:
•	D – redefine number of detents
•	S – specify force threshold value for moving joystick into next position
•	P – redefine position feedback
•	Used for realigning joystick if moved out of center
•	C – redefine cursor delay value between each mouse movement
•	E – redefine cursor and pedal delay value
•	I – redefine pedal click between each left mouse click
•	K – toggle keyboard arrow key mode
•	M – toggle cursor movement
•	X – redefine X axis orientation
•	Y – redefine Y axis orientation

Additional Features:
Many additional features have been included beyond the design scope of the primary objectives for the joystick.  The addition of features allows for the user to select how the joystick operates to specific usage scenarios.  These customizations range in many different features.
The joystick has an optional mode of outputting keyboard  arrow key navigation with or without cursor support.  This feature can allow the joystick to function under conventional control—isolated to a single window and provide equal response with every on screen movement.  This feature can be used with mouse support as well and ported to any other form of input on a computer.
One large area of aided support is for delays.  Many delay controls are implemented throughout the code.  Some delay parameters work across multiple functions, ones that cannot operate at the same time, such as the five selectable mouse routines while others scale to each individual operation such as mouse, keyboard, or pedal delays.
A list of delay features available in the code are as followed:
•	Cursor
•	Keyboard
•	Pedal
•	Cursor & Pedal
•	Joystick linkage:
•	Movement between detents
•	Feedback:
•	Overshoot/undershoot
Five available feedback functions exist in the code.  As implied earlier, many of these features are provided to expand positioning, but also allow for customizable feel over joystick movements.  Each feature is modular and most are activated passively (with the exclusion of active feedback).  A description of each is provided as followed:
•	Time Delay
•	Position
•	Overshoot/undershoot
•	Stress
•	Active
In addition five mouse modes exist as well.  The purpose of the additional mouse modes is for users selection of what feels most comfortable/useable.  A list of mouse functions and brief overview is as followed:
•	Click with user reprogrammable delay
•	Click once until pedal is released
•	Click once and waits for reprogrammable delay and then outputs a long click under the pedal is released
•	Click once and waits for release and if within reprogrammable delay will output a long click
•	Click once and waits for release and if within reprogrammable delay will output a long click; Also works for a click and waits for reprogrammable delay and then outputs a long click under the pedal is released
