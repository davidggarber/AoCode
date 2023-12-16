function solve1() {
  print(followFrom([0,0,1,0], true));
}

function followFrom(ray, doTrace) {
  var seen = {};
  var been = {};
  var queue = [];
  
  seen[ray] = true;
  queue.push(ray);

  while (queue.length > 0) {
    ray = queue.pop();
    var at = [ray[0],ray[1]];
    been[at] = true;
    var next = move(ray, lines[ray[1]][ray[0]]);
    for (var n of next) {
      if (inBounds(n) && !(n in seen)) {
        seen[n] = true;
        queue.push(n);
      }
    }
  }

  if (doTrace) {
    for (var y = 0; y < lines.length; y++) {
      var str = '';
      for (var x = 0; x < lines[y].length; x++) {
        var pos = [x, y];
        str += (pos in been) ? '#' : '.';
      }
      trace(str);
    }
  }

  return Object.keys(been).length;
}

function inBounds(ray) {
  return ray[0] >= 0
    && ray[1] >= 0
    && ray[0] < lines[0].length
    && ray[1] < lines.length;
}

function move(ray, through) {
  var dest = [ray[0] + ray[2], ray[1] + ray[3], ray[2], ray[3]];
  var dest1 = [ray[0] + ray[3], ray[1] + ray[2], ray[3], ray[2]];
  var dest2 = [ray[0] - ray[3], ray[1] - ray[2], -ray[3], -ray[2]];
  if (through == '.') {
    return [dest];
  }
  else if (through == '/') {
    return [dest2];
  }
  else if (through == '\\') {
    return [dest1];
  }
  else if (through == '-' && ray[2] != 0) {
    return [dest];  // no-op
  }
  else if (through == '|' && ray[3] != 0) {
    return [dest];  // no-op
  }
  else {
    return [dest1, dest2];  // fork
  }
}
 


function solve2() {
  var record = {};
  var width = lines[0].length;
  var height = lines.length;
  for (var x = 0; x < width; x++) {
    var top = [x, 0, 0, 1];
    var bottom = [x, height - 1, 0, -1];
    record[top] = followFrom(top, false);
    record[bottom] = followFrom(bottom, false);
  }
  for (var y = 0; y < height; y++) {
    var left = [0, y, 1, 0];
    var right = [width - 1, y, -1, 0];
    record[left] = followFrom(left, false);
    record[right] = followFrom(right, false);
  }

  var best = Object.values(record).reduce((prev, cur) => Math.max(prev, cur));
  print(best);
}
