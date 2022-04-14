import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py FILENAME FILENAME")

    # Read database file into a variable
    with open(sys.argv[1]) as data:
        reader = csv.reader(data)
        database = [row for row in reader]

    # Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as file:
        sequence = file.read()

    # Find longest match of each STR in DNA sequence
    longest_str = [longest_match(sequence, subsequence) for subsequence in database[0][1:]]

    # Check database for matching profiles
    matches = []
    for row in database[1:]:
        check_match(longest_str, row, matches)

    if len(matches) < 1:
        print('No match')
    else:
        print(matches[0])
    return


def check_match(longest_matches, database_row, matches):
    for i in range(len(longest_matches)):
        if longest_matches[i] != int(database_row[i+1]):
            return
    matches.append(database_row[0])


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
