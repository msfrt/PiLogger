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
    bus = can.interface.Bus(bustype=bustype, channel=channel)
    msg = can.Message(arbitration_id=0xc0ffee,
                      data=[10, 0, 0, 1, 3, 1, 4, 1],
                      is_extended_id=False)
    bus.send(msg)
    
    return render_template("success.html")
    # name = request.args.get("name")

    # if not name:
    #     return render_template("failure.html")

    # return render_template("success.html", name=name)

def send_message(ID, data):
    bus = can.interface.Bus(bustype=bustype, channel=channel)
    msg = can.Message(arbitration_id=123,
                      data=[0, 5, 0, 1, 3, 1, 4, 1],
                      is_extended_id=True)
    
if __name__ == "__main__":
    app.run()