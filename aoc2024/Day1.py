data = open('day1.txt', mode='rt')
lines = data.readlines()
first = []
second = []
for line in lines:
  numbers = [int(num) for num in line.split() if num.isdigit()]
  first.append(numbers[0])
  second.append(numbers[1])

first.sort()
second.sort()
counts = {}

diffs = 0
for i in range(0, len(first)):
  diffs += abs(first[i] - second[i])
  if second[i] in counts:
    counts[second[i]] += 1
  else:
    counts[second[i]] = 1
  if first[i] not in counts:
    counts[first[i]] = 0

print("part 1: " + str(diffs))

sum = 0
for f in first:
  sum += f * counts[f]

print("part 2: " + str(sum))
