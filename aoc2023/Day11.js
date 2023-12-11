var expansion_rows = [];
var expansion_cols = [];
var galaxies = [];

function setup() {
  for (var i = 0; i < lines.length; i++) {
    if (lines[i].indexOf('#') < 0) {
      expansion_rows.push(i);
      trace('blank');
    }
    else {
      var gal = lines[i].split('#');
      var x = 0;
      for (var g = 0; g < gal.length - 1; g++) {
        x += gal[g].length;
        galaxies.push([x++, i]);
      }
      trace(gal.length - 1);
    }
  }
  for (var i = 0; i < lines[0].length; i++) {
    if (column(i).indexOf('#') < 0) {
      expansion_cols.push(i);
    }
  }
}

function solve1() {
  setup();

  var sum = 0;

  var gals = [];
  for (var g of galaxies) {
    var dy = expansion_rows.filter(r => r < g[1]).length;
    var dx = expansion_cols.filter(c => c < g[0]).length;
    gals.push([g[0] + dx, g[1] + dy]);
  }

  for (var i = 0; i < gals.length; i++) {
    for (var j = i + 1; j < gals.length; j++) {
      sum += Math.abs(gals[j][1] - gals[i][1]);
      sum += Math.abs(gals[j][0] - gals[i][0]);
    }
  }

  print(sum);
}

function column(c) {
  var str = '';
  for (var l of lines) {
    str += l[c];
  }
  return str;
}


function solve2() {
  var expansion = 999999;  // 1 -> 1000000
  var gals = [];
  for (var g of galaxies) {
    var dy = expansion_rows.filter(r => r < g[1]).length;
    var dx = expansion_cols.filter(c => c < g[0]).length;
    gals.push([g[0] + dx * expansion, g[1] + dy * expansion]);
  }

  var sum = 0;
  for (var i = 0; i < gals.length; i++) {
    for (var j = i + 1; j < gals.length; j++) {
      sum += Math.abs(gals[j][1] - gals[i][1]);
      sum += Math.abs(gals[j][0] - gals[i][0]);
    }
  }

  print(sum);
}
