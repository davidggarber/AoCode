var allBlocks = [];

function solve1() {
  lines.push('');
  var sum = 0;
  var block = [];
  for (var line of lines) {
    if (line.length == 0) {
      var row = mirrorValue(block);
      var cols = pivot(block);
      var col = mirrorValue(cols);
      sum += 100 * row + col;
      for (var i = 1; i < row; i++) {
        trace(' ');
      }
      trace((row == 0 ? '  !row: ' : '<- row') + row);
      trace('   col: ' + col);
      for (var i = Math.max(row,1)+2; i < block.length + 2; i++) {
        trace(' ');
      }
      allBlocks.push(block);
      block = [];
    }
    else {
      block.push(line);
    }
  }

  print(sum);
}

function mirrorValue(rows) {
  var sum = 0;
  for (var y = 1; y < rows.length; y++) {
    if (rows[y] == rows[y - 1]) {
      if (isRowMirror(rows, y-2, y+1)) {  // mirror between y and y-1?
        sum += y;
      }
    }
  }
  return sum;
}

function isRowMirror(rows, up, down) {
  while (up >= 0 && down < rows.length) {
    if (rows[up--] != rows[down++]) {
      return false;
    }
  }
  return true;
}

function diffBits(a, b) {
  var diffs = 0;
  for (var i = 0; i < a.length; i++) {
    if (a[i] != b[i]) {
      diffs++;
    }
  }
  return diffs;
}

function smudgeValue(rows) {
  var sum = 0;
  for (var y = 1; y < rows.length; y++) {
    var db = diffBits(rows[y], rows[y - 1]);
    if (db <= 1) {
      var up = y - 2;
      var down = y + 1;
      while (db <= 1 && up >= 0 && down < rows.length) {
        db += diffBits(rows[up--], rows[down++]);
      }
      if (db == 0) {
        // smudge could be on outside edge
      }
      if (db == 1) {
        sum += y;
      }
    }
  }
  return sum;
}

function solve2() {
  var sum = 0;
  for (var block of allBlocks) {
    var row = smudgeValue(block);
    var cols = pivot(block);
    var col = smudgeValue(cols);
    sum += 100 * row + col;
    for (var i = 1; i < row; i++) {
      trace(' ');
    }
    trace((row == 0 ? '  !row: ' : '<- row') + row);
    trace('   col: ' + col);
    for (var i = Math.max(row,1)+2; i < block.length + 2; i++) {
      trace(' ');
    }
  }
  print(sum);
}
