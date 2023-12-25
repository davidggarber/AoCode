function solve1() {
  colorMap();

  var sum = 0;
  var cols = pivot(lines);

  for (var col of cols) {
    var fixed = col.match(/\#|[^\#]+/g);
    var row = col.length;
    var load = 0;
    for (var f of fixed) {
      if (f == '#') {
        row--;
      }
      else {
        var rocks = f.match(/O/g)?.length ?? 0;
        load += (row + (row - (rocks - 1))) * rocks / 2;
        row -= f.length;
      }
    }
    sum += load;
    trace(load);
  }

  print(sum);
}

function roll(block, up) {
  var rolled = [];
  for (var row of block) {
    var fixed = row.match(/\#|[^\#]+/g);
    var str = '';
    for (var f of fixed) {
      if (f == '#') {
        str += '#';
      }
      else {
        var rocks = f.match(/O/g)?.length ?? 0;
        if (up) {
          str += Array(rocks + 1).join('O');
        }
        str += Array(f.length + 1 - rocks).join('.');
        if (!up) {
          str += Array(rocks + 1).join('O');
        }
      }
    }
    rolled.push(str);
  }
  return rolled;
}

function spin(grid) {
  var cols = pivot(grid);
  var rolled = roll(cols, true);
  var rows = pivot(rolled);
  rolled = roll(rows, true);
  cols = pivot(rolled);
  rolled = roll(cols, false);
  rows = pivot(rolled);
  rolled = roll(rows, false);
  return rolled;
}

function load(grid) {
  var sum = 0;
  var row = grid.length;
  for (var line of grid) {
    var rocks = line.match(/O/g)?.length ?? 0;
    sum += rocks * row--;
  }
  return sum;
}

function solve2() {
  var seen = {};
  var grid = lines;
  var count = 0;
  var freq = 0;
  var target = 1000000000;
  while (count++ < target) {
    grid = spin(grid);
    var h = grid.join('');
    if (h in seen) {
      freq = count - seen[h];
      break;
    }
    seen[h] = count;
  }
  
  var ff = (target - count) % freq;
  while (ff-- > 0) {
    grid = spin(grid);
  }

  print(load(grid));
}
