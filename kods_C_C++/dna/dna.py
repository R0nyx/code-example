import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: tournament.py FILENAME FILENAME")

    # TODO: Read database file into a variable
    people = []
    flag = 0
    file = sys.argv[1]
    with open(file) as f:
        reader = csv.DictReader(f) #If used csv.reader() Could skip first row with next(*row)
        for person in reader:
            person["AGATC"] = int(person["AGATC"])
            person["AATG"] = int(person["AATG"])
            person["TATC"] = int(person["TATC"])
            if "GATA" in person:
                flag = 1
                person["TCTG"] = int(person["TCTG"])
            people.append(person)

    # TODO: Read DNA sequence file into a variable

    DNA_FILE = open(sys.argv[2], "r")
    DNA_seq = DNA_FILE.readline()

    # TODO: Find longest match of each STR in DNA sequence

    results = {}  # Dictionary of the results
    if flag == 1:
        for STR in ["AGATC", "AATG", "TATC", "TCTG"]:  # EACH value of STR to be iterated over
            count = longest_match(DNA_seq, STR)
            results[STR] = count
    elif flag == 0:
        for STR in ["AGATC", "AATG", "TATC"]:  # EACH value of STR to be iterated over
            count = longest_match(DNA_seq, STR)
            results[STR] = count

    # TODO: Check database for matching profiles
    for person in people:
        if flag == 0:
            if results == {k: person[k] for k in ["AGATC", "AATG", "TATC"]}:
                print(person["name"])
                return
        elif flag == 1:
            if results == {k: person[k] for k in ["AGATC", "AATG", "TATC", "TCTG"]}:
                print(person["name"])
                return
    print("No match")
    return


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


main()
