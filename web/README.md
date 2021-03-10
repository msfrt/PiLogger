# User Overrides Web Panel
Communicate with the vehicle's CAN bus using a web application. Send custom or preset
messages through the interactive and user-friendly panel.

## Requirements
The web panel is ran through the Flask web framework, which comes bundled to
the default Python installation.

Additionally, the application depends on several web sockets to communicate
with the server. Install them using:

```bash
pip install flask-socketio
pip install gevent-websocket
```

Version inter-compatibility is highly limited, which may lead to several difficulties
while running the web application. Visit the [version compatibility](https://flask-socketio.readthedocs.io/en/latest/#version-compatibility)
if encountering such errors.

The application uses [this](https://github.com/msfrt/Electrical-SR20/tree/master/DBCs)
CAN file as database to extract specifications of signals and messages. The python-can
and cantools libraries handle the CAN data and message manipulation. Install them using:

```bash
pip install python-can
pip install cantools
```

## Setting-Up
### Configuration
The location of the CAN DBC file used to read messages and signals from is stored
in DBC_FILE global at [app.py](/app.py) -it currently points to the [CAN2.dbc](https://github.com/msfrt/Electrical-SR20/tree/master/DBCs) located in the SR-20 master branch-.

The global variables BUSTYPE and CHANNEL set the configuration for the CAN's interface
(declared at [app.py](/app.py)). Configure them to match the bus specifications:

```bash
BUSTYPE = 'socketcan'
CHANNEL = 'vcan0'
```
 
### Server
In order to set-up the web server, run the following command in the folder
where the app.py file is located at the RasPi.

```bash
flask run --host=0.0.0.0
```

## Usage
Once set-up, access the web application [<hostname>:5000](/)
The web application will display the following panel:
![](images/panel.png 'Overrides Application Panel')

### Sending a Message Manually
Input the ID of any message you want to send to the bus, and include the encoded
(max.) 8-bytes-long data in hexadecimal. Then click send.

### USER_11 Message
I.e., setting the display's LED color. Input 0 for default color, 1 for
blinking white, and 2 for blinking yellow. Then click send.

### USER_12 Message
I.e., show a message in the driver's display. Input a (max.) 8-character-long message
to show in the display. Then click send.

### USER_10 Message
I.e., set the left and right fan, water pump, and brake overrides.
By being enabled, the USER_10 message at the set values is sent over the period
provided in the input box (defaulted at 1000 ms). Modify the values concurrently by dragging the sliders
to the desired ones.

***Note:*** _The values sent by the message change synchronously as the slider values_
_are updated, even while sending is enabled. Nonetheless, the period over_
_which the message is sent will not update if it's changed while sending is enabled._

_In order to avoid unexpected behavior, if the client's connection is interrupted or lost,_
_the server will be notified and all values will be set to 0 automatically._
