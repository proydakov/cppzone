array = []
array.append(0)
array.append(2)
array.append(3)

while len(array) <= 101:
    array_len = len(array)
    next_val = array[array_len - 2] + array[array_len - 1]
    array.append(next_val)


print("char* g_solutions[] = {")
for element in array:
    print('"' + str(element) + '",')
print("};")
