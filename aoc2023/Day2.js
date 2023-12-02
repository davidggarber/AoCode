function solve1() {
  var sum = 0;
  for (var x of lines) {
    var colon = x.split(':');
    var semi = colon[1].trim().split(';');
    var valid = true;
    for (var s of semi) {
      var comma = s.trim().split(',');
      for (var c of comma) {
        var space = c.trim().split(' ');
        var count = parseInt(space[0]);
        if (space[1] == 'red' && count > 12) { valid = false; }
        if (space[1] == 'green' && count > 13) { valid = false; }
        if (space[1] == 'blue' && count > 14) { valid = false; }
      }
    }
    trace(1, valid);
    if (valid) {
      var game = parseInt(colon[0].split(' ')[1]);
      sum += game;
    }
  }

  print(1, sum);
}


function solve2() {
  var sum = 0;
  for (var x of lines) {
    var colon = x.split(':');
    var semi = colon[1].trim().split(';');
    var r = 1, g = 1, b  = 1;
    for (var s of semi) {
      var comma = s.trim().split(',');
      for (var c of comma) {
        var space = c.trim().split(' ');
        var count = parseInt(space[0]);
        if (space[1] == 'red' && count > r) { r = count; }
        if (space[1] == 'green' && count > g) { g = count; }
        if (space[1] == 'blue' && count > b) { b = count; }
      }
    }
    var power = r * g * b;
    trace(2, power);
    sum += power;
  }

  print(2, sum);
}
