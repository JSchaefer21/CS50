while True:
    try:
        altura = int (input("Height? "))
    except ValueError: # no deja pasar algo que no sea un int
        continue
    if altura < 1 or altura > 8: # solo admite valores entre 1 y 8
        continue
    else:
        break # sale del while

for i in range(altura):
    for j in range(altura-1-i):
        print(" ", end="")
    print("#"*(i+1), end="")
    print("  ", end="")
    print("#"*(i+1))