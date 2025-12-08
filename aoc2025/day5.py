data = open('day5.txt', mode='rt')
lines = data.readlines()

lines = [line.strip() for line in lines]
ranges = [line for line in lines if '-' in line]
gap = lines.index('')
ingredients = lines[gap+1:]

ranges = [r.split('-') for r in ranges]
ranges = [(int(r[0]), int(r[1])) for r in ranges]
ingredients = [int(i) for i in ingredients]

count_fresh = 0
for i in ingredients:
  fresh = False
  for r in ranges:
    if r[0] <= i <= r[1]:
      fresh = True
      break
  if fresh:
    count_fresh += 1


print('Part 1:', count_fresh)

count_fresh = 0
ranges = sorted(ranges, key=lambda x: (x[0], -x[1]))
fresh_ranges = []
prev = ranges[0]
for r in ranges:
  if r[0] > prev[1]:
    count_fresh += prev[1] - prev[0] + 1
    prev = r
  elif r[1] <= prev[1]:
    # ignore this nested range
    continue
  else:
    # overlapped range
    prev = (prev[0],r[1])
count_fresh += prev[1] - prev[0] + 1

print('Part 2:', count_fresh)
