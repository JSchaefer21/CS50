# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000

def main():
    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    # TODO: Read teams into memory from file
    teams = [] # lista de teams
    file = csv.DictReader(open(sys.argv[1]))
    """
    Para cada fila, he puesto row como podria haber puesto cualquier palabra
    con [] accedo al valor de una key dentro del diccionario
    y guardo la key con el valor convertido a int en un diccionario
    """
    for row in file:
        row["rating"] = int(row["rating"])   # creo un diccionario con la clave (nombre) y valor(ranking)
        teams.append(row)                      # añado cada discionario a la lista

    counts = {} # diccionario con:name and number of times a team won a tournament
    # TODO: Simulate N tournaments and keep track of win counts
    for x in range(N): # hago un loop de N veces
        winner = simulate_tournament(teams) #simulo partido con la lista de equipos
        if winner in counts:
            counts[winner] += 1     # si el anador ya esta en el diccionario le sumo 1
        else:
            counts[winner] = 1      # si no esta, lo añado con valor 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])
    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    winner = teams
    while len(winner) > 1:
        winner = simulate_round(winner)
    return winner[0]["team"]
    # como cada ganador es un diccionario, y quiero acceder al nombre del team
    # especifico ["teams"] para acceder solo al valor de la columna team del archivo csv
    # o al valor de la clave team dentro del diccionario


if __name__ == "__main__":
    main()
