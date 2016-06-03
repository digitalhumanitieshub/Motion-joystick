#Motion-joystick

##Introduction

Motion-joystick is a simple Arduino Leonardo joystick/gamepad system that that uses a digital motion processor to set the joystick axes, with adjustable in-built motion filtering. In that sense it works in a similar way to an air-mouse where, for example, moving the circuit left or right changes the corresponding joystick axis value. We developed it at the Digital Humanities Hub because it has a wide range of uses in arts and especially for disabled artists. For instance, it can be used as a head-tracker for people with disabilities to control art or music software, as well as for game playing and gamified-training.

The system should work as any other joystick does in all operating systems that understand how to talk to a USB joystick including Windows, OSX and Linux, as well as many non-iOS phones and tablets. And all of those devices then benefit from motion-filtering without the need for bespoke code on each OS. Thanks to the HTML5 Gamepad API it can even be easily used to control web-content and web-apps on desktop and mobile browsers. There are other open source projects that can say the same too, but we decided to develop our own so that we could extend it to have useful features for use in arts, and particularly by disabled artists, such as:

* The ability to customise the system as an input device, rather than the more common gaming uses.
* Being able to maintain related software specifically for it, including mouse control and supplementary voice control.
* The opportunity to add filtering to motion such as to help people with involuntary muscle actions or spasms (including in hands such as may be the case for people with Parkinsons).

Motion-joystick is released under the GPL v2 License (see LICENSE.md). Any aspect of the system that is based on the work of others should be used in accordance with the licenses of those projects: basically you should refer to the project pages listed at the end of this file for the Arduino circuit diagram, as well as the Joystick and I2Cdevlib libraries we used.

##Using Motion-joystick

The design of our Motion-joystick was inspired by the open source EdTracker head-tracking system, details of which are provided in the credits section below. In fact, we used their circuit without modification, so construction details are available on their website and on many other web-pages. So, rather than duplicate their construction details please build your breadboarded or soldered-up EdTracker circuit based on the EdTracker instructions, and then upload the Arduino sketch provided here to use our software.

Once you've uploaded our code to your Arduino Leonardo circuit, it should automatically connect to your computer as a joystick or gamepad (you will probably see messages about installing a new device pop up). You can check that it's operating properly using a joystick checker (e.g. search for joy.cpl on Windows). Turning the circuit right or left should move the X-Axis value, rotating it up and down should change the Y-Axis value, and rolling it from side to side should change the Z-Axis value.

The button included in the EdTracker circuit is used for two purposes. First, if you hold it down for more than three seconds it will recenter the axes: i.e. zero on each of the three axes will be set to the position you hold the circuit when you release the button. That can be useful to remove any drift that happens over time. Second, it acts as Button0 on a joystick, so can be used as a control-button if you keep it pressed for less than three seconds.

##Controlling Motion-joystick over a serial link

As well as talking joystick-talk to a computer, Motion-Jostick also allows for a serial-USB link that can be used to change the behaviour of the circuit. In the simplest sense that means you can open a serial monitor in the Arduino coding environment, set the speed to 115200 baud, and send control-characters that make it change how the joystick works (just type them in the text box at the top of the serial monitor and click 'Send'). The controls we've implemented are listed below (note that 'v', 'z', 'a' and 'b' are for advanced use, so if you're new to this please just ignore them):

* 'v': The circuit responds to this command simply by returning the string "dhhtracker". It's useful if you want to code a program to control the joystick that needs to work out, by itself, which serial port the circuit is connected to.
* 'r': This resets the centre values of the joystick axes, in the same way that holding the button down for over three seconds does.
* 'z': This removes centering functionality, so shouldn't be used unless you want to use mode 'a' below. If you accidentally use this control then you can get back to normal by sending the 'r' command. This function is useful as with the 'a' mode you can make the axes centred around real-world yaw/pitch/roll values.
* 'a': This sets the joystick to scale the axes values to encompass a complete sphere. So, for example, fully left on the X-Axis is 180 degrees left of centre, and fully right is 180 degrees right of centre. You can use this mode to get full Yaw/Pitch/Roll data into the computer, such as to control a 3D model.
* 'b': This is the default operating mode, where the joystick axes are tied to rotations of +/- 45 degrees from the centred value. This is important for use as a control device: for instance, if you're controlling a mouse with a head tracker you only want to turn your head a small amount to move the cursor.
* '0': This turns off the motion filtering. It is off by default when the circuit boots up.
* '1' to '9': This sets motion filtering from light (1) to heavy (9). The higher the setting the harder it is to rapidly change the axis values, which is useful for filtering out trembling and spasms. A bit of trial and error testing should help establish the level of filtering needed for a particular person/context.

##A note about drifting values

All devices that measure orientation using accelerometers and gyroscopes are likely to suffer some drift issues, such as the axis values moving slowly away from zero. The motion-processor inside the MPU6050 chip used in Motion-joystick does a very good job of eliminating most of the gyroscope drift, so the system should be quite stable and can of course be re-centred using the button. However, if you still find drift a problem you could use a circuit with a magnetometer in it that tries to use compass measurements to remove the drift. For instance, you could look for the MPU9050-based EdTracker circuit online and adapt our code to use with it.

However, we thought it best to mention that simple motion processors take time to work out their orientation and calculate corrections for drift. That means that drift will be much more noticable when you first turn the circuit on and should hopefully reduce significantly over time. So, if drift is really annoying you then leaving the circuit in a static position for a few minutes before use should help. Then when you centre the axes you should find the results are much more stable and you don't need to re-centre so often. Hopefully that'll also mean the person using it isn't going to get as frustrated with it as they may do if they use it straight after switch-on.

##About the Digital Humanities Hub

The Digital Humanities Hub focuses on collaborative triple helix working across the Arts and Humanities with cultural and heritage organisations, digital and creative industries and academics to develop collaborative and innovative digital prototypes. It is based on the University of Birmingham's Edgbaston Campus in the UK and you can visit the Hub's website at http://www.birmingham.ac.uk/facilities/digitalhumanitieshub/index.aspx

##Credits

This project was greatly facilitated by the projects listed below, which we gratefully acknowledge. For instance, the EdTracker circuit was used, as well as a number of Arduino libraries generously placed in the open-source domain by their creators. Also, please note that the MPU6050 control code in our Arduino sketch was based on the examples provided in I2Cdevlib.

Details for the Arduino project can be found here: http://www.arduino.cc

The EdTracker website can be found here: http://edtracker.org.uk/

The I2Cdevlib website can be found here: http://www.i2cdevlib.com/

The Arduino Joystick library can be found here: https://github.com/MHeironimus

Concept and development: Dr Christopher Creed and Dr Andrew Thomas.

