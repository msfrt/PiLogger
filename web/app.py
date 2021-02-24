from flask import Flask, render_template, request
# pip install flask-socketio
# pip install gevent-websocket
from flask_socketio import SocketIO
import can
import cantools

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app,cors_allowed_origins="*")

# Set up bus
bustype = 'socketcan'
channel = 'vcan0'

bus = can.interface.Bus(bustype=bustype, channel=channel)

# Get data from CAN2
db = cantools.database.load_file("can_data/CAN2.dbc")
user10_msg = db.get_message_by_name("USER_10")
user11_msg = db.get_message_by_name("USER_11")
user12_msg = db.get_message_by_name("USER_12")

@app.route("/")
def index():
    return render_template("index.html")

@socketio.on('update')
def update(message):
    user10_message(message['left'], message['right'], message['pump'], message['brake'])

@socketio.on('led_color')
def led_color(message):
    user11_message(message['color'])
    
@socketio.on('display_message')
def display_message(message):
    user12_message(message['text'])

@socketio.on('disconnect')
def terminate():
    print('Client disconnected')
    for i in range(3):
        user10_message(0, 0, 0, 0)

@app.route("/success")
def success():
    ID = request.args.get("id")
    
    data = []
    for i in range(1, 9):
        byte = request.args.get(f"data{i}")
        if not byte:
            continue
        data.append(byte)

    if not ID or not data:
        return render_template("failure.html")

    send_message(ID, data)
    return render_template("success.html")

@app.route("/success2")
def success2():
    user10_message(request.args.get("myRange1"), request.args.get("myRange2"), request.args.get("myRange3"), request.args.get("myRange4"))
    
    return render_template("success.html")

def send_message(ID, data):
    msg = can.Message(arbitration_id=int(ID),
                      data= [int(x, 16) for x in data],
                      is_extended_id=True)
    bus.send(msg)
    
def user10_message(left, right, pump, brake):
    left = int(left)
    right = int(right)
    pump = int(pump)
    brake = int(brake)
    
    data = user10_msg.encode({'USER_fanLeftOverride':left, 'USER_fanRightOverride':right, 'USER_wpOverride':pump, 'USER_brakeLightOverride':brake})
    msg = can.Message(arbitration_id=user10_msg.frame_id, data=data)
    bus.send(msg)

def user11_message(val):
    val = int(val)
    data = user11_msg.encode({'USER_driverSignal':val})
    msg = can.Message(arbitration_id=user11_msg.frame_id, data=data)
    bus.send(msg)
    
def user12_message(display):
    signals = ["USER_driverMessageChar0", "USER_driverMessageChar1", "USER_driverMessageChar2", "USER_driverMessageChar3",
               "USER_driverMessageChar4", "USER_driverMessageChar5", "USER_driverMessageChar6", "USER_driverMessageChar7"]
    encoded = {i: 0 if j == '0' else ord(j) for i, j in zip(signals, display.ljust(8,'0'))}
    
    data = user12_msg.encode(encoded)
    msg = can.Message(arbitration_id=user12_msg.frame_id, data=data)
    bus.send(msg)

if __name__ == "__main__":
    socketio.run(app)