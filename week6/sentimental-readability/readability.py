text = input("Text: ")

letter=0
word=1
sentence=0
for i in range(len(text)):
    if (text[i]>='a' and text[i]<='z') or (text[i]>='A' and text[i]<='Z'):
        letter += 1
    elif text[i] == ' ':
        word += 1
    elif text[i] == '.' or text[i] == '?' or text[i] == '!':
        sentence += 1

L = 100*letter/word
S = 100*sentence/word
n = 0.0588*L - 0.296*S - 15.8

if n<1:
    print("Before Grade 1")
elif n>16:
    print("Grade 16+")
else:
    print("Grade ",round(n))
