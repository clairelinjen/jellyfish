# Jellyfish

### Materials
- ESP32
- Stepper Motor
- Stepper Motor Driver
- Servo
- Connecting Wires

### Setup
Connect the servo to GPIO pin #15, as well as to power and ground.
Connect the stepper motor to the driver, and connect the pins IN1, IN2, IN3, and IN4 on the driver to GPIO pins #25, #26, #27, and #14, respectively. Connect the driver to power and ground.
Attach hanging thread to the servo arm. Attach tendrils of jellyfish to stepper motor.
In jellyfish.ino, replace *ssid_Router with the ssid of a nearby public router.
Connect ESP32 to computer and upload jellyfish.ino to the device.
Download the [kinetic sculpture web API](https://github.com/mbennett12/kinetic-sculpture-webapi), install and run according to instructions.
