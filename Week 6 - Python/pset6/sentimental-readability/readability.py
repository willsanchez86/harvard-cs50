

def main():
    text = input("Text: ")

    num_letters = letter_count(text)

    # Split string into list of words
    words_list = text.split(' ')

    num_words = word_count(words_list)
    num_sentences = sentence_count(words_list)

    # Initialize variables for formula
    L = (num_letters/num_words) * 100
    S = (num_sentences/num_words) * 100
    grade = round(0.0588 * L - 0.296 * S - 15.8)
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f'Grade {round(grade)}')


# Count all letters within text input
def letter_count(text):
    count = 0
    for char in text:
        if char.isalpha():
            count += 1
    return count

# Returns the word count via the length of the input list


def word_count(words_list):
    return len(words_list)

# Returns the number of sentences by checking the last digit of every word


def sentence_count(words_list):
    count = 0
    stop = ('...', '.', '?', '!', '!!!')
    for word in words_list:
        if word[-1] in stop:
            count += 1
    return count


if __name__ == "__main__":
    main()