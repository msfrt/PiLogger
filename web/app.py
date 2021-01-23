from flask import Flask, render_template, request
import can

app = Flask(__name__)

bustype = 'socketcan'
channel = 'vcan0'

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
    USER_10_ID = 0x2C6
    switch = request.args.get("switch")
    delay = request.args.get("delay")
    
    if switch == "on":
        for i in range(1, 5):
            value = request.args.get(f"myRange{i}")
            print(value)
        
        #send_message(USER_10_ID, data)
    
    return render_template("success.html")

def send_message(ID, data):
    bus = can.interface.Bus(bustype=bustype, channel=channel)
    msg = can.Message(arbitration_id=int(ID),
                      data= [int(x, 16) for x in data],
                      is_extended_id=True)
    bus.send(msg)
    
if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)