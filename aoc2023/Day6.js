function solve1() {
  var time = lines[0].match(/[0-9]+/g);
  var dist = lines[1].match(/[0-9]+/g);
  
  var combos = 1;

  for (var i = 0; i < time.length; i++) {
    var t = parseInt(time[i]);
    var d = parseInt(dist[i]);
    for (var h = 1; h < t; h++) {
      if (h * (t - h) > d) {
        var x = t + 1 - 2 * h;
        combos *= x;
        break;
      }
    }
  }
  print(combos);
}


function solve2() {
  var time = lines[0].replace(/\s/gi, '').match(/[0-9]+/g);
  var dist = lines[1].replace(/\s/gi, '').match(/[0-9]+/g);
  
  var combos = 1;

  for (var i = 0; i < time.length; i++) {
    var t = parseInt(time[i]);
    var d = parseInt(dist[i]);
    for (var h = 1; h < t; h++) {
      if (h * (t - h) > d) {
        var x = t + 1 - 2 * h;
        combos *= x;
        break;
      }
    }
  }
  print(combos);
}
