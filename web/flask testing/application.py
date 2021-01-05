from flask import Flask, render_template, request
import random

app = Flask(__name__)

@app.route("/")
def name():
    return render_template("index.html")

@app.route("/hello")
def hello():
    name = request.args.get("name")

    if not name:
        return render_template("failure.html")

    return render_template("hello.html", name=name)