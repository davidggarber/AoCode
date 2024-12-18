import re

data = open('Day17.txt', mode='rt')
lines = data.readlines()

registers = []
output = ''

def initReg():
  global registers
  global output
  registers = []
  registers.append(int(lines[0].strip().split(': ')[1]))
  registers.append(int(lines[1].split(': ')[1]))
  registers.append(int(lines[2].split(': ')[1]))
  output = []

initReg()
program = lines[4].split(': ')[1].split(',')

mirror = True

def write(n):
  global output
  global mirror
  output.append(str(n))
  mirror = str(output) == str(program[:len(output)])
  # print(output)

ip = 0

def combo(n):
  global registers
  if n <= 3:
    return n
  return registers[n - 4]

def adv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[0] = int(div)
  return True

def bxl(operand):
  global registers
  registers[1] = registers[1] ^ operand
  return True

def bst(operand):
  global registers
  registers[1] = combo(operand) % 8
  return True

def jnz(operand):
  global ip
  global registers
  if registers[0] == 0:
    return True
  ip = operand
  return False

def bxc(operand):
  global registers
  registers[1] = registers[1] ^ registers[2]
  return True

def out(operand):
  calc = combo(operand) % 8
  write(calc)
  return True

def bdv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[1] = int(div)
  return True

def cdv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[2] = int(div)
  return True

funcs = [adv, bxl, bst, jnz, bxc, out, bdv, cdv]

while ip >= 0 and ip < len(program):
  op = int(program[ip])
  operand = int(program[ip + 1])
  ret = funcs[op](operand)
  if ret:
    ip += 2

print("part 1: " + str(output))

altA = 3
while True:
  initReg()
  registers[0] = altA
  mirror = True
  ip = 0
  while ip >= 0 and ip < len(program) and mirror:
    op = int(program[ip])
    operand = int(program[ip + 1])
    ret = funcs[op](operand)
    if ret:
      ip += 2
  if str(output) == str(program):
    print(f"part 2: {altA}")
    break
  altA += 1
  if (altA % 100) == 0:
    print(f"altA == {altA}")