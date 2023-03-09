import cs50
from cs50 import get_float
import math


def main():

    change_dollars = get_cents()
    change = change_dollars * 100
    quarters = calculate_quarters(change)
    change = change - quarters * 25
    dimes = calculate_dimes(change)
    change = change - dimes * 10
    nickels = calculate_nickels(change)
    change = change - nickels * 5
    pennies = calculate_pennies(change)
    change = change - pennies * 1
    coins = quarters + dimes + nickels + pennies
    print(coins)


def get_cents():
    while True:
        try:
            change_dollars = get_float("Change owed: ")
            if change_dollars > 0:
                break
        except ValueError:
            print("Enter a positive integer")
    return change_dollars


def calculate_quarters(change):
    qrt = int(change / 25)
    return qrt


def calculate_dimes(change):
    dime = int(change / 10)
    return dime


def calculate_nickels(change):
    nick = int(change / 5)
    return nick


def calculate_pennies(change):
    penn = int(change)
    return penn


main()
