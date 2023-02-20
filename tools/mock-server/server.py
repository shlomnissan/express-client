from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route("/", methods=['GET'])
def process_get_request():
    headers = request.headers
    print(headers)
    return 'Hello GET World!'

@app.route("/", methods=['POST'])
def process_post_request():
    headers = request.headers
    print(headers)
    print("First name: " + request.form.get("firstName"))
    print("Last name: " + request.form.get("lastName") + "\n")
    return 'Hello POST World!'

app.run()