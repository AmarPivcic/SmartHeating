# SmartHeating

## Project Description:

The SmartHeatomg project is designed to simplify the management of home radiators with TRV valves. The goal of the project is to provide a web application for controlling temperature of a room via internet communication.

The project consists of three main components:

1. **Control Panel:** This is a web application (HTML, CSS and JS) accessible from a browser. It allows users to control the settings remotely.

2. **Execution Side:** This is implemented using a NodeMCU ESP-8266 with a DHT-22 temperature and humidity sensor and simple servo motor. The servo motor is used to set the valve to wanted position (from 0 to 6).

3. **Real-time Database:** A real-time database is created using Google's Firebase service. It enables communication between the control panel and the execution side.

## Functionality:

### Manual Control:

- The web application provides switch to enable or disable automatic control.

- If the user disables automatic control, the buttons for setting the desired temperature in automatic mode will be disabled. Instead, two buttons for manual control of valve position will be enabled.

- For example, by selecting a value of the radiator valve, the user can activate heating at a certain intensity. Changing the value will adjust the heating intensity or turn it on or off.

- Simple image that represents a radiator valve is displayed on the webpage for the user to understand what is happening and to select the desired setting.

### Automatic Climate Control:

- When using automatic climate control, the user can simply select their desired temperature using two buttons on the web application. In this case, the web application sends the desired temperature to the real-time database.

- The execution side checks this temperature value every 1000 milliseconds. If the desired temperature is higher than the current temperature, the servo will set the valve to position 6 (fully open). If the wanted temperature is lower than the current temperature, the servo will set the valve to position 0 (fully closed).

- After reaching the desired temperature, the climate control system will turn off. It will re-activate if there is a change in temperature (+/-0.5 degree tolerance) or if the user adjusts the desired temperature again.

### Reading Current Temperature and Humidity:

- The web application displays the current temperature and humidity values in the room where the radiator is located. These values refresh every 200 milliseconds (plus the delay on execution side - so it varies between 200ms and 1200ms).
