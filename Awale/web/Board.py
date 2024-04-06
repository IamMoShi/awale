# awale board class

class Board:
    def __init__(self):
        """
        Le plateau de jeu est une liste de 12 cases (6 pour chaque joueur)
        """
        self.table = [4] * 12
        self.list_moves = []

    def __str__(self):
        side0 = ""
        side1 = ""
        for i in range(0, 6):
            side0 += str(self.table[i]) + " "
            side1 += str(self.table[i + 6]) + " "
        return "\n" + side0 + "\n" + side1 + "\n"

    def __repr__(self):
        return self.__str__()

    def available_moves(self, side: bool):
        if side == 1:
            return [i for i in range(0, 6) if self.table[i] != 0]
        else:
            return [i for i in range(6, 12) if self.table[i] != 0]

    def capture(self, case: int) -> int:
        points = 0
        while self.table[case] in [2, 3]:  # Si la dernière graine tombe dans une case avec 2 ou 3 graines
            points += self.table[case]
            self.table[case] = 0
            case = (case - 1) % 12
        return points

    def make_move(self, case: int):
        number_of_seeds = self.table[case]  # On prend les graines de la case
        self.table[case] = 0
        current_case = case

        while number_of_seeds > 0:  # On redistribue les graines sur les cases suivantes
            current_case = (current_case + 1) % 12

            if current_case == case:
                continue

            self.table[current_case] += 1
            number_of_seeds -= 1

        points = self.capture(current_case)  # On capture les graines si nécessaire
        return points
