data = open('day2.txt', mode='rt')
lines = data.readlines()
reports = []

for line in lines:
  reports.append([int(num) for num in line.split()])
print("found " + str(len(reports)) + " reports")

def isSafe(rep):
  first = rep[0]
  if rep[1] == first:
    return False
  if rep[1] > first:
    for r in rep[1:]:
      if r <= first or r - first > 3:
        return False
      first = r
  elif rep[1] < first:
    for r in rep[1:]:
      if r >= first or r - first < -3:
        return False
      first = r
  return True

count = 0
count1 = 0
for rep in reports:
  if isSafe(rep):
    count += 1
  else:
    for i in range(0, len(rep)):
      rep2 = rep[:i] + rep[(i+1):]
      if isSafe(rep2):
        count1 += 1
        break

print("part 1: " + str(count))
print("part 2: " + str(count + count1))
