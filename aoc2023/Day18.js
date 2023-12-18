function solve1() {
  var pos = {x:0, y:0};
  var area = 0;
  var perim = 0;
  for (var line of lines) {
    var splt = line.split(' ');
    var dir = splt[0];
    var dist = parseInt(splt[1]);
    perim += dist;

    var p = {
      x:pos.x + (dir == 'L' ? -dist : dir == 'R' ? dist : 0),
      y:pos.y + (dir == 'U' ? -dist : dir == 'D' ? dist : 0)
    };
    area += p.x * pos.y - p.y * pos.x;
    pos = p;
  }

  area = Math.abs(area) / 2;
  area += perim / 2 + 1;
  
  print(area);
}


function solve2() {
  var pos = {x:0, y:0};
  var area = 0;
  var perim = 0;
  for (var line of lines) {
    var dir = parseInt(line.match(/.(?=\))/)[0]);  // 0123 == RULD
    var dist = parseInt(line.match(/(?<=\#)...../)[0], 16);
    perim += dist;

    var p = {
      x:pos.x + (~dir & 1) * (1 - (dir & 0x2)) * dist,
      y:pos.y + (dir & 1) * ((dir & 0x2) - 1) * dist
    };
    area += p.x * pos.y - p.y * pos.x;
    pos = p;
  }

  area = Math.abs(area) / 2;
  area += perim / 2 + 1;
  
  print(area);
}
