import time
from multiprocessing import Process

import numpy as np


def n_premiers_nombres_premiers(n):
    nombres_premiers = []
    nombre = 2  # Commencer la recherche à partir de 2, le premier nombre premier

    while len(nombres_premiers) < n:
        est_premier = True
        # Vérifier si le nombre est premier en le divisant par tous les nombres de 2 à sa racine carrée
        for i in range(2, int(nombre ** 0.5) + 1):
            if nombre % i == 0:
                est_premier = False
                break
        # Si le nombre est premier, l'ajouter à la liste des nombres premiers
        if est_premier:
            nombres_premiers.append(nombre)
        # Passer au nombre suivant
        nombre += 1

    return


def processing(numbers):
    # square(numbers)
    [n_premiers_nombres_premiers(x) for x in numbers]


if __name__ == '__main__':
    numbers = np.ones(10**4, dtype=int) * 2000

    # t = time.time()
    # [square(x) for x in numbers]
    # print(time.time() - t)
    #
    # t = time.time()
    # list(map(square, numbers))
    # print(time.time() - t)

    # t = time.time()
    # np.vectorize(n_premiers_nombres_premiers)(numbers)
    # print(time.time() - t)

    t = time.time()
    chunks = np.array_split(numbers, 16)
    processes = []
    for i in range(len(chunks)):
        process = Process(target=processing, args=(chunks[i],))
        process.start()
        processes.append(process)

    for process in processes:
        process.join()

    print(time.time() - t)

    t = time.time()
    chunks = np.array_split(numbers, 8)
    processes = []
    for i in range(len(chunks)):
        process = Process(target=processing, args=(chunks[i],))
        process.start()
        processes.append(process)

    for process in processes:
        process.join()

    print(time.time() - t)

