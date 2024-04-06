# awale player class
import Board


class Player:
    def __init__(self, name: str, score: int, side: int):
        self.name = name
        self.score = score
        self.side = side

    # -----------------------------------------------------------------------------------------------------------------#
    # Getters and Setters
    # -----------------------------------------------------------------------------------------------------------------#

    def get_name(self):
        return self.name

    def get_score(self):
        return self.score

    def get_side(self):
        return self.side

    def set_score(self, score):
        self.score = score

    # -----------------------------------------------------------------------------------------------------------------#
    # Python class methods
    # -----------------------------------------------------------------------------------------------------------------#

    def __str__(self):
        return self.name + " " + "(" + str(self.side) + ") : " + str(self.score) + " points\n"







