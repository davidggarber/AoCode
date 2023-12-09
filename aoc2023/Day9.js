function solve1() {
  var sum = 0;
  for (var line of lines) {
    var vals = line.split(' ').map(v => parseInt(v));
    var next = extrapolate(vals);
    sum += next[0];
    trace(next[0]);
  }

  print(sum);
}

function extrapolate(vals) {
  var allZeros = true;
  var diffs = [];
  var prev = null;
  for (var v of vals) {
    if (prev != null) {
      diffs.push(v - prev);
      allZeros &= (v - prev) == 0;
    }
    prev = v;
  }
  if (allZeros) {
    return [prev];
  }
  var grow = [];
  var exp = extrapolate(diffs);
  for (var e  of exp) {
    diffs.push(e);
    var next = prev + e;
    grow.push(next);
    prev = next;
  }
  return grow;
}

function solve2() {
  var sum = 0;
  for (var line of lines) {
    var vals = line.split(' ').map(v => parseInt(v));
    vals = vals.reverse();
    var next = extrapolate(vals);
    trace(next[0]);
    sum += next[0];
  }

  print(sum);
}
