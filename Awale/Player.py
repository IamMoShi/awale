#awale player class

from Board import Board

class Player :
    def __init__(self, name, score, side) :
        self.name = name
        self.score = score
        self.side = side


    def get_name(self) :
        return self.name

    def get_score(self) :
        return self.score

    def get_side(self) :
        return self.side

    def set_score(self, score) :
        self.score = score

    def __str__(self) :
        return self.name + " "+ "("+self.side + ") : " + str(self.score) + " points\n"
    
    def make_move(self, board, case) :
        number_of_seeds = board[case] #On prend les graines de la case
        board[case] = 0
        while number_of_seeds > 0 : #On redistribue les graines sur les cases suivantes
            case = (case + 1) % 12
            board[case] += 1
            number_of_seeds -= 1
            
        board = self.capture(board, case) #On capture les graines si nécessaire
        return board
    
    
    def capture(self, board, case) :
        while board[case] == 2 or board[case] == 3 : #Si la dernière graine tombe dans une case avec 2 ou 3 graines
            self.score += board[case]
            board[case] = 0
            case = (case - 1) % 12
        return board
    
    
    def choose_case(self, board) :
        while True :
            try :
                case = int(input("Choisissez une case ou -1 pour quitter la partie : "))
                if case == -1 :
                    board.list_moves.append("-1")
                if self.side == 0 and case < 6 and case >= 0 and board[case] != 0 :
                    board.list_moves.append(str(case))
                    return case
                elif self.side == 1 and case < 12 and case >= 6 and board[case] != 0 :
                    board.list_moves.append(str(case))
                    return case
            except ValueError :
                print("Veuillez entrer un nombre valide (côté \"0\" : 0 à 5 et côté \"1\" : 6 à 11)\n")
            
            