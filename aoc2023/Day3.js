function solve1() {
  var sum = 0;
  var y = 0;
  for (var l of lines) {
    var matches = l.match(/[0-9]+/g) || [];
    var x = 0;
    for (var m of matches) {
      x = l.indexOf(m, x);
      var right = x + m.length;
      var part = hasNeighbor(y, x, right);
      if (part != null) {
        sum += parseInt(m);
        trace(m);
        parts.push([m, part]);  // [part#, coord]
      }
      x = right;
    }
    y++;
  }

  print(sum);
}

function hasNeighbor(row, left, right) {
  if (left > 0 && lines[row][left - 1] != '.') {
    return [left - 1, row];
  }
  if (right < lines[row].length - 1 && lines[row][right] != '.') {
    return [right, row];
  }

  left = Math.max(0, left - 1);
  var len = right + 1 - left;
  var above = row > 0 ? lines[row - 1].substring(left, left + len) : '';
  var below = row + 1 < lines.length ? lines[row + 1].substring(left, left + len) : '';
  if (above.search(/[^0-9\.]/) >= 0) {
    return [left + above.search(/[^0-9\.]/), row - 1];
  }
  if (below.search(/[^0-9\.]/) >= 0) {
    return [left + below.search(/[^0-9\.]/), row + 1];
  }
  return null;
}

var parts = [];
var gears = {};

function solve2() {
  var sum = 0;
  for (var p of parts) {
    var sym = lines[p[1][1]][p[1][0]];
    if (sym == '*') {
      if (p[1] in gears) {
        // 2nd hit of this gear
        sum += parseInt(gears[p[1]]) * parseInt(p[0]);
      }
      else {
        gears[p[1]] = p;
      }
    }
  }

  print(sum);
}
