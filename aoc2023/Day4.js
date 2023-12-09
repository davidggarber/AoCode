function solve1() {
  var sum = 0;
  for (var line of lines) {
    var parts = line.split(/\:|\|/);  // split on : or |
    var wins = parts[1].match(/[^ ]+/g);  // split, clumping all non-spaces
    var tix = ' ' + parts[2] + ' ';
    var pts = 0;
    var c = 0;
    var d = ' ';
    for (var w of wins) {
      if (tix.match(' ' + w + ' ') != null) {
        pts = pts == 0 ? 1 : (pts * 2);
        c++;
        d += w + ' ';
      }
    }
    trace(c + ': ' + d);
    sum += pts;
  }

  print(sum);
}


function solve2() {
  var sum = 0;
  var i = 0;
  var dupes = new Array(lines.length).fill(0);
  for (var line of lines) {
    var parts = line.split(/\:|\|/);
    var wins = parts[1].match(/[^ ]+/g);
    var tix = ' ' + parts[2] + ' ';
    var c = 0;
    dupes[i]++;
    for (var w of wins) {
      if (tix.match(' ' + w + ' ') != null) {
        c++;
      }
    }
    for (; c > 0; c--) {
      dupes[i + c] += dupes[i];
    }
    trace(dupes[i]);
    sum += dupes[i];
    i++;
  }

  print(sum);
}
