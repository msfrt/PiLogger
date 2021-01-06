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
    data = request.args.get("data")

    if not ID or not data:
        return render_template("failure.html")

    send_message(ID, data)
    return render_template("success.html")
    

def send_message(ID, data):
    bus = can.interface.Bus(bustype=bustype, channel=channel)
    msg = can.Message(arbitration_id=ID,
                      data= [x for x in str(data)],
                      is_extended_id=True)
    bus.send(msg)
    
if __name__ == "__main__":
    app.run()