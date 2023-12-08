function solve1() {
  var sum = 0;
  var instr = lines[0];
  var map = {};
  for (var line of lines) {
    var eq = line.match(/[A-Z]+/g);
    if (eq && eq.length == 3) {
      map[eq[0]] = [eq[1], eq[2]];
    }
  }

  var pos = 'AAA';
  var i = 0;
  var steps = 0;
  var max = 1000000;
  while (pos != 'ZZZ') {
    if (i >= instr.length) {
      i = 0;
    }
    var left = instr[i++] == 'L';
    pos = map[pos][left ? 0 : 1];
    // trace(1, pos, false);
    if (++steps > max) {
      break;
    }
  }

  //flush(1);
  print(1, steps);
}


function solve2() {
  var sum = 0;
  var instr = lines[0];
  var map = {};
  var paths = [];
  for (var line of lines) {
    var eq = line.match(/[A-Z]+/g);
    if (eq && eq.length == 3) {
      map[eq[0]] = [eq[1], eq[2]];
      if (eq[0][2] == 'A') {
        paths.push(eq[0]);
      }
    }
  }

  var progress = {};
  var allStops = {};
  var loops = [];

  for (var p of paths) {
    var stops = [];
    allStops[p] = stops;
    var dest = {};
    progress[p] = dest;
    var steps = 0;
    var pos = p;
    var done = false;
    var left;
    while (!done) {
      while (pos[2] != 'Z') {
        left = instr[steps % instr.length] == 'L';
        steps++;
        pos = map[pos][left ? 0 : 1];
      }  
      trace(2, p + ' -> ' + pos + ' : ' + steps);
      if (!(pos in dest)) {
        dest[pos] = [];
      }
      else {
        for (var d of dest[pos]) {
          if (d % instr.length == steps % instr.length) {
            loops.push([d, steps]);
            done = true;
            break;  // We've looped  
          }
        }
      }
      if (!done) {
        stops.push(steps);
        dest[pos].push(steps);
        trace(2, p + ' -> ' + pos + ' : ' + dest);
        left = instr[steps % instr.length] == 'L';
        steps++;
        pos = map[pos][left ? 0 : 1];
      }
    }
  }

  // Each start hits only one end (unexpected, but easier)
  // Each loop pair is the first time that path hit an end, then the second time it hit the same end.
  // find the least common multiple of A+Bi for all 6 A/B/i triples

  var a = loops[0][0];
  var x = loops[0][1] - a;
  for (var l = 1; l < 6; l++) {  // loops.length
    var b = loops[l][0];
    var y = loops[l][1] - b;

    // a + ix == b + jy
    // j == (a - b)/y + i*x/y, where all values are integers
    // (a-b + i*x) % y == 0
    // (i*x) % y == y-(a-b)%y
    var yab = y - (a - b);
    while (yab % x != 0) {
      // yab % x + ny % x == 0 % x
      var n = x - (yab % x);
      n = Math.ceil(n / (y % x));
      yab += y * n;
    }
    var i = yab / x;
    var j = (a - b + i * x) / y;

    a = a + i * x;
    b = b + j * y;  // test
    x = x * y / gcd(x, y);
  }

  print(2, a);
}

function lcm(a, b) {
  return a * b / gcd(a, b);
}

function gcd(a, b) {
  if (!b) {
    return a;
  }
  
  return gcd(b, a % b);
}