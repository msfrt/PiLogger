from flask import Flask, render_template, request, redirect, send_file
from querydb import output_csv

MEASUREMENTS = ['cpu','disk','diskio','kernel','mem','net','processes','swap',
                'system','temp','weather','wireless']

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def index():
    if request.method == 'GET':
        return render_template('index.html')
    else:
        values = [bool(request.form.get(m)) for m in MEASUREMENTS]
        output_csv(values)
        return send_file('data.csv', mimetype='text/csv', attachment_filename='your_data.csv', as_attachment = True)
    
if __name__ == "__main__":
    app.run()