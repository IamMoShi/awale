from flask import Flask, render_template, jsonify

app = Flask(__name__)


@app.route("/")
def hello_world():
    return render_template('index.html')


@app.route("/current_board")
def curren_board():
    dict = {
        "board": [5] * 12,
        "playable": [1] * 6 + [0] * 6,
        "score1": 0,
        "score2": 0,
    }
    return jsonify(dict)


@app.route("/play/<int:playable>")
def play(playable):
    dict = {
        "board": [playable] * 12,
        "playable": [0] * 6 + [1] * 6,
        "score1": playable + 1,
        "score2": playable ,
    }
    return jsonify(dict)
