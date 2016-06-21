## Safecracker

An autodialer device built as part of the [Ruxcon](https://ruxcon.org.au/) 2014 presentation: [Safecracking on a Budget by Luke Jahnke & Jay Davis](http://2014.ruxcon.org.au/assets/2014/slides/rux-safe-cracking.pdf)

### Requirements

#### Hardware:

- 3D printed parts as per the files provided in the folder 3d_parts
- Arduino and compatible stepper motor shield, for this project the [Arduino Mega 2560](https://www.arduino.cc/en/Main/ArduinoBoardMega2560) and [Arduino Motor Shield](https://www.arduino.cc/en/Main/ArduinoMotorShieldR3) were used.
- [Stepper motor](https://littlebirdelectronics.com.au/products/stepper-motor-68-oz-in-400-steps-rev)
- [Hall Effect Sensor](http://www.freetronics.com.au/pages/hall-effect-magnetic-and-proximity-sensor-module-quickstart-guide#.V2jBKbp9600) and magnet for detecting if the lock is open. 
- [Mounting Hub](https://www.pololu.com/product/1203) for connecting the lock coupler to the stepper motor
- [Clamping Hub](https://littlebirdelectronics.com.au/products/clamping-hub-6mm-bore) for connecting chassis components.
- [Suction Cups](http://www.ggrglass.co.uk/suction-mounts/7kg-suction-mount-with-1-4-long-stud/) for mounting.

#### Software:

The code uploaded was used for the 2014 presentation but can be modified to be stand alone. It uses a python script to communicate over the usb/serial connection to the arduino to feed it combinations found on twitter. 

### License

See the [LICENSE](LICENSE) file for license rights and limitations.
