import cs50

from cs50 import get_int


def main():

    height = get_height()
    for i in range(height):
        print("#",end = "")
    print()


def get_height():
    while True:
        # height = get_int("Height: ")
        try:
            height = int(input("Height: "))
            if height > 0 & height < 9:
                break
        except ValueError:
            print("Not an int")

    return height


main()