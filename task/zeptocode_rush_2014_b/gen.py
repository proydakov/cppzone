width = 500
height = 500

f = open( str(width) + "x" + str(height) + ".txt", 'w' )

f.write(str(height))
f.write(' ')
f.write(str(width))
f.write(' ')
f.write(str(0))
f.write('\n')

for i in range(0, height):
    for j in range(0, width):
        f.write('.')
    f.write('\n')
