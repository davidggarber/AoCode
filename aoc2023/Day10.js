var path = [];
var connectors = {};

function solve1() {
  var sum = 0;
  var start;
  for (var y = 0; y < lines.length; y++) {
    for (var x = 0; x < lines[y].length; x++) {
      var pos = coord(x, y);
      var c = lines[y][x];
      if (c == '.') {
        connectors[pos] = [];
      }
      else if (c == '|') {
        connectors[pos] = [coord(x, y-1), coord(x, y+1)];
      }
      else if (c == '-') {
        connectors[pos] = [coord(x-1, y), coord(x+1, y)];
      }
      else if (c == 'L') {
        connectors[pos] = [coord(x, y-1), coord(x+1, y)];
      }
      else if (c == '7') {
        connectors[pos] = [coord(x-1, y), coord(x, y+1)];
      }
      else if (c == 'J') {
        connectors[pos] = [coord(x-1, y), coord(x, y-1)];
      }
      else if (c == 'F') {
        connectors[pos] = [coord(x+1, y), coord(x, y+1)];
      }
      else if (c == 'S') {
        start = [x, y];
      }
    }
  }

  // neighbors of S
  var next = null;
  var x = start[0];
  var y = start[1];
  var s = coord(x, y);
  var east = coord(x+1, y);
  var west = coord(x-1, y);
  var north = coord(x, y-1);
  var south = coord(x, y+1);
  if (!(east in connectors) || connectors[east].indexOf(s) < 0) {
    east = null;
  }
  if (!(west in connectors) || connectors[west].indexOf(s) < 0) {
    west = null;
  }
  if (!(north in connectors) || connectors[north].indexOf(s) < 0) {
    north = null;
  }
  if (!(south in connectors) || connectors[south].indexOf(s) < 0) {
    south = null;
  }

  connectors[start] = [];
  if (east) { connectors[start].push(east); }
  if (west) { connectors[start].push(west); }
  if (north) { connectors[start].push(north); }
  if (south) { connectors[start].push(south); }

  var ss = (east && west) ? '-'
    : (north && south) ? '|'
    : (north && east) ? 'L'
    : (north && west) ? 'J'
    : (south && east) ? 'F'
    : (south && west) ? '7' : '?';
  lines[y] = lines[y].substring(0, x) + ss + lines[y].substring(x + 1);

  next = connectors[start][0];
  path = [next];
  var prev = s;
  while (next != s) {
    var nn = connectors[next][0] == prev ? connectors[next][1] : connectors[next][0];
    path.push(next);
    prev = next;
    next = nn;
  }

  print(path.length / 2);
}

var path = [];


function coord(x, y) {
  return ''+x+','+y;
}


function solve2() {
  var sum = 0;
  for (var y = 0; y < lines.length; y++) {
    for (var x = 0; x < lines[y].length; x++) {
      var pos = coord(x, y);
      if (path.indexOf(pos) < 0 && isInside(x, y)) {
        sum++;
      }
    }
    trace(sum);
  }

  print(sum);
}


function isInside(xx, yy) {
  var part = null;
  var vinside = false;
  var zig = '';
  for (var y = 0; y < yy; y++) {
    var c = lines[y][xx];
    var pos = coord(xx, y);
    if (path.indexOf(pos) < 0) {
      zig = '';
    }
    else if (c == '-') { vinside = !vinside; }
    else if (c == '|') {}  // zig-zag?
    else if (c == 'J' && zig == 'F') { vinside = !vinside; }
    else if (c == 'L' && zig == '7') { vinside = !vinside; }
    else if (c == 'F' || c == '7') { zig = c; }
    else {
      // throw new error('???');
    }
  }

  var hinside = false;
  zig = '';
  var line = lines[yy];
  for (var x = 0; x < xx; x++) {
    var c = line[x];
    var pos = coord(x, yy);
    if (path.indexOf(pos) < 0) {
      zig = '';
    }
    else if (c == '|') { hinside = !hinside; }
    else if (c == '-') {}  // zig-zag?
    else if (c == 'J' && zig == 'F') { hinside = !hinside; }
    else if (c == '7' && zig == 'L') { hinside = !hinside; }
    else if (c == 'F' || c == 'L') { zig = c; }
    else {
      // throw new error('???');
    }
  }

  return vinside && hinside;
}