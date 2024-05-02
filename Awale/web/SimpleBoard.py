import numpy as np
import copy

LEN = 6


class SimpleBoard:

    def __init__(self):
        # firstSecond, a array 2x6 that contains first and second
        self.firstSecond: np.ndarray = np.array([4 * np.ones(LEN, dtype=int), 4 * np.ones(LEN, dtype=int)])
        # 0 : player 1 is playing ? 1 : player 2 is playing
        self.turn: bool = 0
        # The names of the 2 players
        self.playersNames: [str, str] = ["Player 1", "Player 2"]
        # Players scores
        self.playersScores: np.ndarray = np.array([0, 0])

    def __str__(self):
        line: str = ('─' * 18 + '\n' + '-' * 7 + str(" %02d " % (self.playersScores[1],)) + '-' * 7 + '\n')
        for i in self.firstSecond[1][::-1]:
            line += ("%02d " % (i,))
        line += "\n"
        for i in self.firstSecond[0]:
            line += ("%02d " % (i,))
        line += ("\n" + '-' * 7 + str(" %02d " % (self.playersScores[0],)) + '-' * 7 + '\n' + '─' * 18 + '\n')
        return line

    def getMoves(self) -> np.ndarray:
        """
        :return: numpy array, vector 1x6 that contains booleans
        """
        moves: np.ndarray = np.zeros(LEN, dtype=bool)
        for i in range(LEN):
            moves[i] = self.firstSecond[int(self.turn)][i] and not self.starvation(i)

        return moves

    def starvation(self, move_id: int) -> bool:
        if np.array_equal(self.firstSecond[int(not self.turn)], np.zeros(6, dtype=int)):
            # If the other player already have no seeds in his side, our play can't starve him
            # not self.turn == the other player
            return False

        nextBoard = copy.deepcopy(self)
        nextBoard.makeMove(move_id)
        # The other player is now playing, so it is "turn" and not "not turn"
        # Here the player is starving because he had seeds in his side and now no
        return np.array_equal(nextBoard.firstSecond[int(nextBoard.turn)], np.zeros(6, dtype=int))

    def makeMove(self, move_id: int) -> None:
        if self.turn:
            seeds = self.firstSecond[1, move_id]
            self.firstSecond[1, move_id] = 0
            move_id += 6
            played = move_id
        else:
            seeds = self.firstSecond[0, move_id]
            self.firstSecond[0, move_id] = 0
            played = move_id

        while seeds:
            move_id = (move_id + 1) % 12
            if move_id == played:
                move_id = (move_id + 1) % 12

            if move_id < 6:
                self.firstSecond[0, move_id] += 1
            else:
                self.firstSecond[1, move_id - 6] += 1
            seeds -= 1

        # Take the seeds if possible
        if self.turn and move_id < 6:
            move_id = move_id % 6
            while (n := self.firstSecond[0, move_id]) in [2, 3]:
                self.playersScores[1] += n
                self.firstSecond[0, move_id % 6] = 0

                if move_id == 0:
                    break

                move_id -= 1
        elif not self.turn and move_id >= 6:
            move_id = move_id % 6
            while (n := self.firstSecond[1, move_id]) in [2, 3]:
                self.playersScores[0] += n
                self.firstSecond[1, move_id] = 0

                if move_id == 0:
                    break

                move_id -= 1

        if np.array_equal(self.firstSecond[int(not self.turn)], np.zeros(6, dtype=int)):
            self.playersScores[int(self.turn)] += sum(self.firstSecond[int(self.turn)])
        self.turn = not self.turn

    def evaluate(self):
        # TODO
        pass

    def currentPlayer(self) -> str:
        return self.playersNames[self.turn]

    def isGameOver(self) -> bool:
        if self.playersScores[0] > 24 or self.playersScores[1] > 25:
            return True

        if np.array_equal(self.firstSecond[not self.turn], np.zeros(6, dtype=int)):
            return True

        return False


if __name__ == "__main__":
    board = SimpleBoard()


