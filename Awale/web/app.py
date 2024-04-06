from flask import Flask, render_template, jsonify
from Game import Game

app = Flask(__name__)

game = Game("Player1", "Player2")

@app.route("/reset")
def reset():
    game.reset()
    return "Game reset"

@app.route("/")
def hello_world():
    return render_template('index.html')


@app.route("/current_board")
def curren_board():
    board, playable, score1, score2 = game.get_current_board()
    playable = [1 if i in playable else 0 for i in range(12)]
    dict = {
        "board": board,
        "playable": playable,
        "score1": score1,
        "score2": score2,
    }
    return jsonify(dict)


@app.route("/play/<int:playable>")
def play(playable):
    print("player wants to play : " + str(playable))
    game.play(playable)
    board, playable, score1, score2 = game.get_current_board()
    print("next playable cases : " + str(playable))
    playable = [1 if i in playable else 0 for i in range(12)]
    dict = {
        "board": board,
        "playable": playable,
        "score1": score1,
        "score2": score2,
    }
    return jsonify(dict)
