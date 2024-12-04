import re
data = open('day3.txt', mode='rt')
lines = data.read()

def multiply(mul):
  matches = re.match(r'^\d+', mul)
  if not matches:
    return 0
  first = matches.group(0)
  if len(first) == 0 or len(first) > 3 or mul[len(first)] != ',':
    return 0
  a = int(first)
  mul2 = mul[len(first)+1:]

  matches = re.match(r'^\d+', mul2)
  if not matches:
    return 0
  first = matches.group(0)
  if len(first) == 0 or len(first) > 3 or mul2[len(first)] != ')':
    return 0
  b = int(first)
  print(str(a) + ' * ' + str(b) + ' = ' + str(a*b))
  return a * b

def shouldDo(mul, prev):
  splt = mul.split('do')
  if len(splt) == 1:
    return prev
  return splt[len(splt) - 1][:3] != "n't"

muls = lines.split('mul(')
do = shouldDo(muls[0], True)
muls.pop(0)
sum = 0
for mul in muls:
  if do:
    sum += multiply(mul)
  do = shouldDo(mul, do)
  print(sum)

print("part 1: " + str(sum))
#print("part 2: " + str(count + count1))
