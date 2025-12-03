data = open('day2.txt', mode='rt')
lines = data.readlines()

ranges = [range.split('-') for range in lines[0].split(',')]

print(ranges)

def repeat(pref, target_len):
  p = str(pref)
  return int(p * (target_len // len(p)))

def containsRepeat(n):
  s = str(n)
  l = len(s)
  for i in range(1, l // 2 + 1):
    if l % i == 0:
      a = s[0:i]
      aa = a * (l // i)
      if aa == s:
        return True
  return False

# for each range, what are the possible starting sequences, up to 1/2 length?
def evalRange(r):
  sum = 0

  r0 = int(r[0])
  r1 = int(r[1])
  lowest = r[0]
  highest = r[1]
  short = len(lowest)
  long = len(highest)

  # In case r0 and r1 are different lengths, subdivide the range into same-length chunks
  for i in range(short, long + 1):
    # 1-9, 10-99, 100-999, ...
    first = 10 ** (i - 1)
    last = (10 ** i) - 1
    if (r0 > first):
      first = r0
    if (r1 < last):
      last = r1
    
    # Now that the range is all one length, try each prefix length
    for h in range(1, 1 + i // 2):
      if i % h == 0:  # part 2
      # if h * 2 == i:  # part 1
        # Find the first (a) and last (b) prefix that repeats to create a number in the sub-range
        a = int(str(first)[0:h])
        aa = repeat(a, i)
        if aa < r0:
          a += 1
        b = int(str(last)[0:h])
        bb = repeat(b, i)
        if (bb > r1):
          b -= 1
        
        print(f'found {1+b-a} prefixes from {a} to {b}')
        for c in range(a, b + 1):
          if not containsRepeat(c):  # part 2
            cc = repeat(c, i)
            sum += cc
          else:
            print(f'skipping {c} because it is a repeat')

  # return the sum of invalid IDs in this range
  return sum

part1 = 0
for r in ranges:
  part1 += evalRange(r)
print(f'part 1: {part1}')