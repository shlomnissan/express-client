from flask import Flask, request
from flask_httpauth import HTTPBasicAuth
from werkzeug.security import generate_password_hash, check_password_hash

import time

app = Flask(__name__)

@app.route('/', methods=['GET'])
def process_get_request():
    return 'Hello World!'

@app.route('/slow', methods=['GET'])
def process_slow_get_request():
    time.sleep(1)
    return 'Hello slow world!'

@app.route('/', methods=['POST'])
def process_post_request():
    first_name = request.form.get('firstName')
    last_name = request.form.get('lastName')
    return f'Hello {first_name} {last_name}!'

# Authenticated routes

users = {
    "aladdin": generate_password_hash("opensesame")
}

auth = HTTPBasicAuth()

@auth.verify_password
def verify_password(username, password):
    if username in users and \
            check_password_hash(users.get(username), password):
        return username

@app.route('/auth', methods=['GET'])
@auth.login_required
def process_get_with_auth():
    print(request.headers)
    return f'Hello {auth.current_user().capitalize()}!'

app.run()