
def main():
    m = input("m: ")
    print(convert(m))


def convert(spd):
    spd = int (spd) * 90000000000000000
    return spd
main()