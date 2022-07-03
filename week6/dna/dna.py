import csv
import sys
# row = fila


def main():
    # TODO: Check for command-line usage

    if not len(sys.argv) == 3:              #si entran mas o menos de 3 variables envio mensaje error y salgo
        exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable

    file = csv.DictReader(open(sys.argv[1]))# dicReade convierte el artivo en diccionario usando la primera fila como claves fijas
    lista = list(file)                      # guardo el diccionario leido dentro de una lista
    str_list = []                           # creo una lista donde pasare solo la primera fila
    for i in lista[0]:                      # dentro de lista[0] cada una de las claves i las paso a una lista, solo las claves
        str_list.append(i)                  # paso solo las claves del primer diccionario de la lista, donde esta el nombre y los STR (no quiero nombres ni valores concretos del primer diccionario)
#    str_list.remove('name')                # no lo borro todavia, me va bien una listas de largo 4 para poder comparar mas tarde, sin tener en cuenta el nombre

    # TODO: Read DNA sequence file into a variable

    file2 = csv.reader(open(sys.argv[2]))   # abro el archivo de adn
    for line in file2:                      # y me guardo la unica linea que hay
        adn = line

    # TODO: Find longest match of each STR in DNA sequence

    dicSTR = {}                             # la str_list contiene nombre y los str de la base
    for i in str_list:                      # uso la i directamente como el valor que corresponde dentro de la lista
        if i != 'name':                     # en vez de borrar el nombre arrib,a no lo utilizo y ya
            n = longest_match(adn[0], i)    # sequence, subsequence
            dicSTR[i] = str(n)              # me guardo el el diccionario los STR enviados con su num en string
                                            # necesario en string para poder compararlo luego

    # TODO: Check database for matching profiles

    for i in range(0, len(lista), 1):       # paso por cada diccionario dentro de la lista
        temp_dic=lista[i]                   # guardo el disccionario en uno temporal
        ganador = temp_dic["name"]          # me guardo el nombre del posible ganador
        del temp_dic["name"]                # borro el nombre del diccionario para poder comparar
        if dicSTR == temp_dic:              # comparo con los datos sacados del ADN
            print(ganador)                  # si son iguales imprimo el nombre del ganador
            return 0                        # y salgo

    print("No match")               # si no encuentra ninguno imprimo no match
    return 0                        # y salgo


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()