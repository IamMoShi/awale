from flask import Flask, render_template, jsonify, redirect, url_for
from SimpleBoard import SimpleBoard
import json

app = Flask(__name__)

game = SimpleBoard()


@app.route("/reset")
def reset():
    game.reset()
    return redirect(url_for('hello_world'))


@app.route("/")
def hello_world():
    return render_template('index.html')


@app.route("/current_board")
def curren_board():
    board = [int(i) for i in game.firstSecond[0]] + [int(i) for i in game.firstSecond[1]]
    score1, score2 = list(game.playersScores)
    if game.turn:
        playable = [0] * 6 + [1 if i else 0 for i in game.getMoves()]
    else:
        playable = [1 if i else 0 for i in game.getMoves()] + [0] * 6
    dict = {
        "board": board,
        "playable": playable,
        "score1": int(score1),
        "score2": int(score2),
    }
    return jsonify(dict)


@app.route("/current_board_proba")
def current_board_proba():
    dict = curren_board().get_json()
    dict["proba"] = game.evaluate()
    return jsonify(dict)


@app.route("/play/<int:playable>")
def play(playable):
    playable = playable % 6
    if not game.getMoves()[playable]:
        return curren_board()

    game.makeMove(playable)

    return curren_board()


if __name__ == '__main__':
    app.debug = True
    app.run()
