# awale game class

from Board import Board
from Player import Player


class Game:
    def __init__(self, NameP1: str, NameP2: str):
        self.players = [Player(NameP1, 0, 0), Player(NameP2, 0, 1)]
        self.player_turn = 0
        self.winner = None
        self.board = Board()

    def playable(self):
        return self.board.available_moves(self.player_turn)

    def play(self, case: int):
        if case not in self.playable():
            print("Case non jouable : " + str(case) + "playable : " + str(self.playable()))
            return self.board, False

        points = self.board.make_move(case)
        self.players[self.player_turn].score += points

        if (self.players[self.player_turn].score > 24
                or sum(self.board.table[0:5]) == 0
                or sum(self.board.table[6:11]) == 0):
            print("La partie est terminée !\n")
            return self.board, True

        self.player_turn = (self.player_turn + 1) % 2
        print(self.board)
        return self.board, None

    def get_current_board(self):
        return self.board.table, self.playable(), self.players[0].score, self.players[1].score

    def reset(self):
        self.players = [Player(self.players[0].name, 0, 0), Player(self.players[1].name, 0, 1)]
        self.player_turn = 0
        self.winner = None
        self.board = Board()

"""
    def play_turn(self):
        current_player = self.players[self.player_turn]
        print("C'est au tour de " + current_player.get_name() + " de jouer\n")
        print(self.board)
        chosen_case = current_player.choose_case(self.board)
        self.board = current_player.make_move(self.board, chosen_case)
        self.players_turn = (self.player_turn + 1) % 2

    def finish_game(self):
        if sum(self.board.table[0:5]) == 0 or sum(self.board.table[6:11]) == 0:
            self.players[0].score += sum(self.board.table[6:12])
            self.players[1].score += sum(self.board.table[0:6])
            print("La partie est terminée !\n")
            print("Score de " + self.players[0].get_name() + " : " + str(self.players[0].score) + " points\n")
            print("Score de " + self.players[1].get_name() + " : " + str(self.players[1].score) + " points\n")
            if self.players[0].score > self.players[1].score:
                self.winner = self.players[0]
            elif self.players[0].score < self.players[1].score:
                self.winner = self.players[1]
            else:
                print("Egalité !")
                return True
            print("Le gagnant est " + self.winner.get_name() + "! Félicitations !\n")
            return True
        elif self.board.list_moves[-1] == "-1":
            print("Abandon de la partie\n La partie est terminée !\n")
            return True

    def play_game(self):
        while self.finish_game() == False:
            self.play_turn()
        print("Fin de la partie")
        return True
"""