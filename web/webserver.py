from flask import Flask, jsonify

app = Flask(__name__)


@app.route('/')
app.run(port=9988)
