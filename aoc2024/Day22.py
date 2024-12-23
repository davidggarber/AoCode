import re

data = open('Day22.txt', mode='rt')
lines = data.readlines()

def mixSecret(prev, num):
  return prev ^ num

def pruneSecret(num):
  return num % 16777216  # 1 << 24

def nextSecret(num):
  next = pruneSecret(mixSecret(num, num * 64))  # << 6
  next = pruneSecret(mixSecret(next, int(next / 32)))  # >> 5
  next = pruneSecret(mixSecret(next, next * 2048))  # << 12
  return next

def fastSecret(num, count):
  for i in range(count):
    num = nextSecret(num)
  return num

def test10(num):
  print(f"10 secrets after {num} are...")
  for i in range(10):
    num = nextSecret(num)
    print(num)

def test2000(num):
  for i in range(2000):
    num = nextSecret(num)
  return num

# test10(123)

part1 = 0
for line in lines:
  part1 += test2000(int(line.strip()))

print(f"part 1: {part1}")

payoff = {}
def scanSequences(num):
  global payoff
  prev = num % 10
  first = {}
  prev4 = []
  for i in range(2000):
    num = nextSecret(num)
    ones = num % 10
    delta = ones - prev
    prev = ones
    prev4.append(delta)
    if len(prev4) > 4:
      prev4.pop(0)
    key = str(prev4)
    if len(prev4) == 4 and key not in first:
      first[key] = ones
      if key in payoff:
        payoff[key] += ones
      else:
        payoff[key] = ones

for line in lines:
  scanSequences(int(line.strip()))

part2 = max(payoff.values())
print(f"part 2: {part2}")
