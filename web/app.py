from flask import Flask, render_template, request
import can
import cantools

app = Flask(__name__)

# Set up bus
bustype = 'socketcan'
channel = 'vcan0'

bus = can.interface.Bus(bustype=bustype, channel=channel)

# Get data from CAN2
db = cantools.database.load_file("can_data/CAN2.dbc")
user10_msg = db.get_message_by_name("USER_10")

@app.route("/")
def index():
    return render_template("index.html")

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
    switch = request.args.get("switch")
    delay = request.args.get("delay")
    
    left = int(request.args.get("myRange1"))
    right = int(request.args.get("myRange2"))
    pump = int(request.args.get("myRange3"))
    brake = int(request.args.get("myRange4"))
    
    data = user10_msg.encode({'USER_fanLeftOverride':left, 'USER_fanRightOverride':right, 'USER_wpOverride':pump, 'USER_brakeLightOverride':brake})
    
    msg = can.Message(arbitration_id=user10_msg.frame_id, data=data)
    bus.send(msg)
    
    return render_template("success.html")

def send_message(ID, data):
    msg = can.Message(arbitration_id=int(ID),
                      data= [int(x, 16) for x in data],
                      is_extended_id=True)
    bus.send(msg)
    
if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)