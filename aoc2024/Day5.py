import re

data = open('day5.txt', mode='rt')
lines = data.readlines()

orders = []
updates = None
for line in lines:
  if updates != None:
    updates.append(line)
  elif line.strip() == '':
    updates = []
  else:
    matches = re.match(r'(\d+)\|(\d+)', line.strip())
    pair = [int(m) for m in matches.groups()]
    orders.append(pair)

befores = {}
afters = {}
for o in orders:
  if o[0] not in afters:
    afters[o[0]] = [p[1] for p in orders if p[0] == o[0]]
  if o[1] not in befores:
    befores[o[1]] = [p[0] for p in orders if p[1] == o[1]]

sum = 0
misordered = []
for up in updates:
  list = [int(u) for u in up.strip().split(',')]
  good = True
  for i in range (0, len(list) - 1):
    x = list[i]
    if x in befores and len([n for n in list[(i+1):] if n in befores[x]]) > 0:
      good = False
      break
  for i in range (1, len(list)):
    x = list[i]
    if x in afters and len([n for n in list[:i] if n in afters[x]]) > 0:
      good = False
      break
  if good:
    sum += list[int(len(list) / 2)]
  else:
    misordered.append(list)

print("part 1: " + str(sum))

sum2 = 0
for list in misordered:
  correct = False
  while not correct:
    correct = True
    for i in range(0, len(list) - 1):
      for j in range(i + 1, len(list)):
        b = list[i]
        a = list[j]
        if (b in befores and a in befores[b]) or (a in afters and b in afters[a]):
          list = list[:i] + [a] + list[i:j] + list[(j+1):]
          correct = False
          break
      if not correct:
        break
  sum2 += list[int(len(list) / 2)]

print("part 2: " + str(sum2))
