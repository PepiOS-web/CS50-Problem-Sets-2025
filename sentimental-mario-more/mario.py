from cs50 import get_int, get_string

# Ask the user for a valid height between 1 and 8
while True:
    try:
        answer = int(input("Height: "))

        if 1 <= answer <= 8:
            break

    except ValueError:
        print("Enter a correct valuea")

# Outer loop -> controls each row
for i in range(1, answer + 1):

    #Print spaces on the left side (decrease each row)
    for s in range(answer - i):
        print(" ", end = "")

    #Print left pyramid blocks (#)
    for j in range(i):
        print("#", end = "")

    #Print the gap between the two pyramids (always two spaces)
    print("  ", end = "")

    #Print right pyramid blocks (#)
    for j in range(i):
        print("#", end = "")
    #Move to the next line after finishing one row
    print()
