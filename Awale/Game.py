#awale game class

from Board import Board
from Player import Player

class Game :
    def __init__(self, NameP1, NameP2) :
        self.players = [Player(NameP1, 0, 0), Player(NameP2, 0, 1)]
        self.player_turn = 0
        self.winner = None
        self.board = Board()
        
        
    def play_turn(self) : 
        current_player = self.players[self.player_turn]
        print("C'est au tour de " + current_player.get_name() + " de jouer\n")
        print(self.board)
        chosen_case = current_player.choose_case(self.board)
        self.board = current_player.make_move(self.board, chosen_case)
        self.players_turn = (self.player_turn + 1) % 2
        
        
    def finish_game(self) :
        if sum(self.board.board[0:5]) == 0 or sum(self.board.board[6:11]) == 0 :
            self.players[0].score += sum(self.board.board[6:12])
            self.players[1].score += sum(self.board.board[0:6])
            print("La partie est terminée !\n")
            print("Score de " + self.players[0].get_name() + " : " + str(self.players[0].score) + " points\n")
            print("Score de " + self.players[1].get_name() + " : " + str(self.players[1].score) + " points\n")
            if self.players[0].score > self.players[1].score :
                self.winner = self.players[0]
            elif self.players[0].score < self.players[1].score :
                self.winner = self.players[1]
            else :
                print("Egalité !")
                return True
            print("Le gagnant est " + self.winner.get_name() + "! Félicitations !\n")
            return True
        elif self.board.list_moves[-1] == "-1":
            print("Abandon de la partie\n La partie est terminée !\n")
            return True
        
        
    def play_game(self) :
        while self.finish_game() == False :
            self.play_turn()
        print("Fin de la partie")
        return True

