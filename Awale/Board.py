#awale board class
from Player import Player

class Board : 
    def __init__(self) :
        self.board = [4,4,4,4,4,4,4,4,4,4,4,4]
        self.list_moves = []
        
    def __str__(self) :
        side0 = ""
        side1 = ""
        for i in range(0,6) :
            side0 += str(self.board[i]) + " "
            side1 += str(self.board[i+6]) + " "
        return "\n" + side0 + "\n" + side1 + "\n"