from collections import defaultdict
from collections import deque

data = open('day11.txt', mode='rt')
lines = data.readlines()

devices = {}

for line in lines:
  parts = line.strip().split(': ')
  dev = parts[0]
  parts = parts[1].split(' ')
  devices[dev] = set(parts)

def count_routes(from_device, to_device, exclude):
  found = defaultdict(int)
  followed = defaultdict(int)
  found[to_device] = 1
  followed[to_device] = 0

  queue = deque()
  queue.append(to_device)

  while len(queue) > 0:
    dest = queue.popleft()
    delta = found[dest] - followed[dest]
    if delta == 0:
      continue
    followed[dest] = found[dest]

    sources = [k for k,v in devices.items() if dest in v]
    for s in sources:
      if s in exclude:
        continue
      found[s] += delta
      queue.append(s)
      print(f'{s}-{dest}+{delta})')

  return found[from_device]

# print(f'Part 1: {count_routes('you', 'out', set())}')


dac_out = count_routes('dac', 'out', set(['fft', 'svr']))
fft_out = count_routes('fft', 'out', set(['dac', 'svr']))
dac_fft = count_routes('dac', 'fft', set(['out', 'svr']))
fft_dac = count_routes('fft', 'dac', set(['out', 'svr']))
svr_dac = count_routes('svr', 'dac', set(['fft', 'out']))
svr_fft = count_routes('svr', 'fft', set(['dac', 'out']))

print(f'Part 2: {svr_dac * dac_fft * fft_out + svr_fft * fft_dac * dac_out}')
