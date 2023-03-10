from flask import Flask, jsonify, request

import time

app = Flask(__name__)

@app.route("/", methods=['GET'])
def process_get_request():
    return 'Hello World!'

@app.route("/slow", methods=['GET'])
def process_slow_get_request():
    time.sleep(1)
    return 'Hello slow world!'


@app.route("/", methods=['POST'])
def process_post_request():
    first_name = request.form.get("firstName")
    last_name = request.form.get("lastName")
    return f'Hello {first_name} {last_name}!'

app.run()