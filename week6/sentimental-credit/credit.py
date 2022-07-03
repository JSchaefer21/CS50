from cs50 import get_int

numero = get_int("Number: ")

digit = len(str(numero))
if digit < 13 or digit == 14 or digit > 16: # solo admite valores de 13, 15 o 16 digitos
    print("INVALID")
    exit()

num = numero//10
n = 0
while num > 0:
    r = 2*(num%10) #saco el resto, es decir el ultimo digito / IMPORTANTE el parentesis
    if r < 9:
        n += r
    else:
        n += 1
        n += r%10
    num = num//100 #saco el cociente, es decir quitarle un digito

num = numero
while num > 0:
    n += num%10
    num = num//100

if n%10 == 0:
    while numero > 100:
        numero = numero//10
    if numero == 34 or numero ==37:
        print("AMEX")
    elif numero > 50 and numero <56:
        print("MASTERCARD")
    else:
        numero = numero//10
        if numero == 4:
            print("VISA")
else:
    print("INVALID")
    exit()
