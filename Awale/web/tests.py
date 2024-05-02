import unittest

import numpy as np

from SimpleBoard import SimpleBoard


class TestSimpleBoard(unittest.TestCase):

    def test_building(self):
        board: SimpleBoard = SimpleBoard()
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 4, 4, 4, 4, 4], [4, 4, 4, 4, 4, 4]])))
        self.assertTrue(np.array_equal(board.playersScores, np.array([0, 0])))
        self.assertTrue(board.turn == 0)
        self.assertEqual(board.playersNames, ["Player 1", "Player 2"])

    def test_getMoves(self):
        board: SimpleBoard = SimpleBoard()
        self.assertTrue(np.array_equal(board.getMoves(), np.array([1, 1, 1, 1, 1, 1])))

    def test_playing(self):
        board: SimpleBoard = SimpleBoard()
        board.makeMove(0)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[0, 5, 5, 5, 5, 4], [4, 4, 4, 4, 4, 4]])))
        self.assertTrue(np.array_equal(board.playersScores, np.array([0, 0])))
        self.assertTrue(board.turn == 1)
        self.assertEqual(board.playersNames, ["Player 1", "Player 2"])
        self.assertTrue(np.array_equal(board.getMoves(), np.array([1, 1, 1, 1, 1, 1])))

        board = SimpleBoard()
        board.makeMove(1)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 0, 5, 5, 5, 5], [4, 4, 4, 4, 4, 4]])))

        board = SimpleBoard()
        board.makeMove(2)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 4, 0, 5, 5, 5], [5, 4, 4, 4, 4, 4]])))

        board = SimpleBoard()
        board.makeMove(3)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 4, 4, 0, 5, 5], [5, 5, 4, 4, 4, 4]])))

        board = SimpleBoard()
        board.makeMove(4)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 4, 4, 4, 0, 5], [5, 5, 5, 4, 4, 4]])))

        board = SimpleBoard()
        board.makeMove(5)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[4, 4, 4, 4, 4, 0], [5, 5, 5, 5, 4, 4]])))

    def test_capture(self):
        board: SimpleBoard = SimpleBoard()
        board.firstSecond = np.array([[0, 1, 1, 0, 0, 1], [2, 0, 0, 0, 0, 2]])
        self.assertTrue(np.array_equal(board.getMoves(), np.array([0, 1, 1, 0, 0, 1])))
        board.makeMove(5)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[0, 1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 2]])))
        self.assertEqual(board.playersScores[0], 3)
        self.assertEqual(board.playersScores[1], 0)

        board.makeMove(5)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[1, 0, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0]])))
        self.assertEqual(board.playersScores[0], 3)
        self.assertEqual(board.playersScores[1], 2)

    def test_multiple_capture(self):
        board: SimpleBoard = SimpleBoard()
        board.firstSecond = np.array([[1, 1, 1, 0, 0, 2], [2, 1, 0, 0, 0, 2]])
        self.assertTrue(np.array_equal(board.getMoves(), np.array([1, 1, 1, 0, 0, 1])))
        board.makeMove(5)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[1, 1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 2]])))
        self.assertEqual(board.playersScores[0], 5)
        self.assertEqual(board.playersScores[1], 0)

        board.makeMove(5)
        self.assertTrue(np.array_equal(board.firstSecond, np.array([[0, 0, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0]])))
        self.assertEqual(board.playersScores[0], 5)
        self.assertEqual(board.playersScores[1], 4)

    def test_starvation(self):
        board: SimpleBoard = SimpleBoard()
        board.firstSecond = np.array([[0, 0, 0, 0, 0, 1], [2, 0, 0, 0, 0, 0]])
        self.assertTrue(np.array_equal(board.getMoves(), np.array([0, 0, 0, 0, 0, 0])))

        board: SimpleBoard = SimpleBoard()
        board.firstSecond = np.array([[0, 0, 1, 0, 1, 1], [2, 0, 0, 0, 0, 0]])
        self.assertTrue(np.array_equal(board.getMoves(), np.array([0, 0, 1, 0, 1, 0])))