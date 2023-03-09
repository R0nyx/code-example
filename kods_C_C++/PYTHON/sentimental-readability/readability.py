import cs50
from cs50 import get_string
import math
import string


def main():

    text = get_string("Text:")
    chars = count_characters(text)
    words = count_words(text)
    sentances = count_sentances(text)

    print(f"ch: {chars} \nwords: {words}\nsentances: {sentances}")
    get_grade(chars, words, sentances)


def get_grade(characters, words, sentances):
    L = round((characters/words) * 100)
    S = round((sentances/words) * 100)
    grade = 0.0588 * L - 0.296 * S - 15.8
    print(f"L: {L}\nS: {S}")
    grade = round(grade)

    if grade >= 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


def count_characters(text):
    text = text.lower()
    count = 0
    for i in range(len(text)):
        if ord(text[i]) >= 97 and ord(text[i]) <= 122:
            count += 1
    return count


def count_words(text):
    words = 0
    if ord(text[0]) != " ":
        words += 1

    for ch in range(len(text)):
        if text[ch] == " ":
            words += 1
    return words


def count_sentances(text):
    count = 0
    for ch in range(len(text)):
        if text[ch] in [".", "?", "!"]:
            count += 1
    return count


main()
