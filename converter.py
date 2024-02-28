inputFile = 'inputConverter.txt'
outputFile = 'input.txt'

with open(inputFile, 'r') as file:
    data = file.read()

data = data.replace(',', '.')

with open(outputFile, 'w') as file:
    file.write(data)

