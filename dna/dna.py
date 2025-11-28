import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py")
        sys.exit(1)

    database_path = sys.argv[1]
    sequences_path = sys.argv[2]

    # TODO: Read database file into a variable
    people = []
    str_names = []

    with open(database_path, newline="") as file:
        #We use this line to read the CVS like a dictionary
        reader = csv.DictReader(file)

        str_names = [h for h in reader.fieldnames if h != "name"]

        for row in reader:
            for s in str_names:
                # Convert each STR count from strings to ints and then compare them numericlly
                row[s] = int(row[s])
            people.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(sequences_path, "r") as file:
        sequence = file.read().strip()

    counts = {s: longest_match(sequence, s) for s in str_names}

    for person in people:
        if all(person[s] == counts[s] for s in str_names):
            print(person["name"])
            return

    print("no match")
    # TODO: Find longest match of each STR in DNA sequence

def longest_match(sequence, subsequence):

    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    for i in range(sequence_length):
        count = 0
        while True:
            start = i + count * subsequence_length
            end = start + subsequence_length
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break

        #Retrun the max number of consecutive times  subsequence appears in sequence
        #Example: squence = AAGTAAGTAAGTG
        #         subseq  = AAGT
        #Returns 3
        longest_run = max(longest_run, count)

    # TODO: Check database for matching profiles

    return longest_run


if __name__ == "__main__":
    main()
