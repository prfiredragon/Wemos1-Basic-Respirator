# Wemos1-Basic-Respirator
Respirador Basico Wemos D1/ Wemos D1 Basic Respirator

This is a small project that anyone can make. To build your own Respirator. The Relay used on this project support up to 250 Volt and 10 Amps. You can connect the desired air pump to this system. Like a 12v pump or a 120v pump.

## **Hardware:**

* [Relay Shield](https://amzn.to/2VbcQOp)
* [Wemos D1 mini or clone](https://amzn.to/2xWvK3I)

## **Software:**

* [Arduino IDE](https://www.arduino.cc/en/Main/Software)

### **Software Instructions**
* Download and install the arduino IDE
* Download and open the code
* Setup the variable "segundos_encendido" to the desired activation time in seconds.
* Setup the variable "segundos_apagado" to the desired stop time in seconds.
* Upload the code.
* Conect the air pump to the circuit.

### **WIFI Version Instructions**
* After setup the "segundos_apagado" variable you most setup the APSSID and the APPSK.
* After connect all you can connect to the Access Point and go to http://192.168.4.1 on the browser.
* Add or remove time to the ON or OFF period.
