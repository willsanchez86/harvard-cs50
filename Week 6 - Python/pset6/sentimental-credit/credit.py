def credit_check():
    # Get all integer input from user
    while True:
        cc_number = input('Number: ')
        try:
            cc_number = int(cc_number)
            break
        except ValueError:
            pass

# Convert integer into list
    list_cc = [int(d) for d in str(cc_number)]

    # Check if cc_number is valid according to Lunh's Algorithm
    if not luhns_algo(list_cc):
        print('INVALID')

    # Run the provider check and print the results
    print(provider_check(list_cc))


def luhns_algo(list_cc):
    temp_list = list_cc.copy()
    temp_list.reverse()

    for i in range(1, len(temp_list), 2):
        product = temp_list[i] * 2
        if product < 10:
            temp_list[i] = product
        else:
            temp_list[i] = product//10
            temp_list.append(product % 10)

    return sum(temp_list) % 10 == 0


def provider_check(list_cc):
    print(len(list_cc))
    # American Express uses 15 digits and always starts with either 34 or 37
    if list_cc[0] == 3 and len(list_cc) == 15:
        return 'AMEX' if list_cc[1] == 4 or list_cc[1] == 7 else 'INVALID'
    # Mastercard has uses 16 digits and always starts with 51, 52, 53, 54, or 55
    if list_cc[0] == 5 and len(list_cc) == 16:
        return 'MASTERCARD' if list_cc[1] >= 1 and list_cc[1] <= 5 else 'INVALID'
    # Visa uses either 13, or 16 digits and always starts with 4
    if list_cc[0] == 4:
        return 'VISA' if len(list_cc) == 13 or len(list_cc) == 16 else 'INVALID'

    return 'INVALID'


if __name__ == "__main__":
    credit_check()