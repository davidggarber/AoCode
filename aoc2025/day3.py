data = open('day3.txt', mode='rt')
lines = data.readlines()

def biggest2(str):
  a = 0
  for i in range(1, len(str)-1):
    c = str[i]
    if c > str[a]:
      a = i
  b = a + 1
  for i in range(a+2, len(str)):
    c = str[i]
    if c > str[b]:
      b = i
  print(str[a] + str[b])
  return int(str[a] + str[b])

def biggest(joltage, n):
  s = ''
  prev = -1
  for b in range(0, n):
    a = prev + 1
    for i in range(a + 1, len(joltage) - n + b + 1):
      c = joltage[i]
      if c > joltage[a]:
        a = i
    s += joltage[a]
    prev = a
  print(s)
  return int(s)

sum = 0
for line in lines:
  sum += biggest(line.strip(), 12)
print(sum)