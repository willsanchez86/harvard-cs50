# Request pyramid_height from user
try:
    pyramid_height = int(input("Please enter a number between 1-8 "))
except ValueError:
    pyramid_height = int(input("Please enter a number between 1-8 "))

# Continue prompting user for valid number if input is not between 1-8
while pyramid_height < 1 or pyramid_height > 8:
    pyramid_height = int(input("Please enter a number between 1-8 "))

# Print Pyramid
dashes = 1
spaces = ' ' * (pyramid_height - dashes)

while dashes <= pyramid_height:
    print(f'{spaces}{dashes * "#"}  {dashes * "#"}')
    dashes += 1
    spaces = ' ' * (pyramid_height - dashes)