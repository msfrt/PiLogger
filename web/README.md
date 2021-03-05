# User Overrides Web Panel
Communicate with the vehicle's CAN bus using a web application. Send custom or preset
messages through the interactive and user-friendly panel.

## Requirements
The web panel is ran through the Flask web framework, which comes bundled to
the default Python installation.

In addition, the application depends on several web sockets to communicate
with the server. Install them using:

```bash
pip install flask-socketio
pip install gevent-websocket
```

Version inter-compatibility is highly limited, which may lead to several difficulties
while running the web application. Visit the [version compatibility]
(https://flask-socketio.readthedocs.io/en/latest/#version-compatibility) if
encountering such errors.

The application uses [can_data/CAN2.dbc](https://github.com/msfrt/PiLogger/blob/web/web/can_data/CAN2.dbc)
as database to extract specifications of signals and messages.

## Server Set-Up
In order to set-up the web server, run the following command in the folder
where the app.py file is located at the RasPi.

```bash
flask run --host=0.0.0.0
```

## Usage
Once set-up, access the web application [here](http://147.92.111.86:5000/)

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
indicated by the input box (in ms). Modify the values concurrently by dragging the sliders
to the desired ones.

### Specifications
The values sent by the message change synchronously as the slider values
are updated, even while sending is enabled. Nonetheless, the period over
which the message is sent will not update if it's changed while sending is enabled.

In order to avoid unexpected behavior, if the client's connection is interrupted or lost,
the server will be notified and all values will be set to 0 automatically.
