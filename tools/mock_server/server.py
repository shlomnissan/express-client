## Copyright 2023 Betamark Pty Ltd. All rights reserved.
## Author: Shlomi Nissan (shlomi@betamark.com)

from flask import Flask, request, cli
from flask_httpauth import HTTPBasicAuth
from werkzeug.security import generate_password_hash, check_password_hash

app = Flask(__name__)
auth = HTTPBasicAuth()

# GET

@app.route('/', methods=['GET'])
def process_get_request():
    return 'Hello World!'

@app.route('/secured', methods=['GET'])
@auth.login_required
def process_get_with_auth():
    return f'Hello {auth.current_user().capitalize()}!'

# POST

@app.route('/', methods=['POST'])
def process_post_request():
    first_name = request.form.get('firstName')
    last_name = request.form.get('lastName')
    return f'Hello {first_name} {last_name}!'

@app.route('/slow', methods=['POST'])
def process_slow_post_request():
    time.sleep(1) 
    return 'Hello World!'

# Helpers

@auth.verify_password
def verify_password(username, password):
    print(username)
    print(password)
    if username == "aladdin" and password == "opensesame":
        return username


app.run()