import cs50
from cs50 import get_int


def main():

    height = get_height()

    for x in range(height):
        for spaces in range(height-1, x, -1):
            print(" ", end="")

        for hash_left in range(x+1):
            print("#", end="")

        # Middle space
        print("  ", end="")

        for hash_right in range(x+1):
            print("#", end="")

        print()


def get_height():
    while True:
        # height = get_int("Height: ")
        try:
            height = int(input("Height: "))
            if height > 0 and height < 9:
                break
        except ValueError:
            print("Not an int")

    return height


main()
