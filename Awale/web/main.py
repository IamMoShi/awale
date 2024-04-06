from web.Game import Game


if __name__ == "__main__":
    name1 = input("Entrez le nom du joueur 1 : ")
    name2 = input("Entrez le nom du joueur 2 : ")
    game = Game(name1, name2)
    game.play_game()