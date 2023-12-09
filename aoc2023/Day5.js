var maps = {};
var chain = {};

function solve1() {
  var seeds = lines[0].match(/[0-9]+/g);
  var trips = null;
  for (var line of lines) {
    var parts = line.split(/\-to\-|\smap\:/);  // split on -to- or map:
    if (parts.length == 3) {
      trips = [];
      chain[parts[0]] = parts[1];
      maps[parts[0]] = trips;
    }
    else if (line.length == 0) {
      trips = null;
    }
    else if (trips) {
      trips.push(line);
    }
  }

  var min = -1;
  for (var s of seeds) {
    var src = parseInt(s);
    var loc = seed2Location(src);

    if (min < 0 || loc < min) {
      min = loc;
    }
  }

  print(min);
}

function map(src, mapper) {
  // src: number
  // mapper: number[3][]
  for (var m of mapper) {
    var trip = m.match(/[0-9]+/g);
    var s0 = parseInt(trip[1]);
    var len = parseInt(trip[2]);
    if (src >= s0 && src < s0 + len) {
      var d0 = parseInt(trip[0]);
      return d0 + (src - s0);
    }
  }
  return src;
}

function seed2Location(src) {
  var type = 'seed';
  while (type != 'location') {
    var dest = map(src, maps[type]);
    type = chain[type];
    src = dest;
  }
  return src;
}

function solve2() {
  var seeds = lines[0].match(/[0-9]+/g);
  var ranges = [];
  for (var i = 0; i < seeds.length; i += 2) {
    var s0 = parseInt(seeds[i]);
    ranges.push([s0, parseInt(seeds[i + 1])]);
  }

  // Rearrange maps to be sorted
  var leading = '000000000000'
  for (var k of Object.keys(maps)) {
    var list = maps[k];
    var sorted = [];
    for (var l of list) {
      var dsl = l.split(' ');
      var sld = leading.substring(0, 12 - dsl[1].length) + dsl[1] + ' ' + dsl[2] + ' ' + dsl[0];
      sorted.push(sld);
    }
    sorted.sort();
    maps[k] = sorted;
  }

  var type = 'seed';
  while (type != 'location') {
    var dest = mapRanges(ranges, maps[type]);
    type = chain[type];
    ranges = dest;
  }

  var min = ranges[0][0];
  for (r of ranges) {
    min = Math.min(min, r[0]);
  }

  print(min);
}

function mapRanges(ranges, mapper) {
  var dest = [];
  for (r of ranges) {
    var r0 = r[0];
    var r1 = r0 + r[1];
    for (var m of mapper) {
      var trip = m.match(/[0-9]+/g);
      var s0 = parseInt(trip[0]);
      var s1 = s0 + parseInt(trip[1]);
      var i0 = Math.max(s0, r0);
      var i1 = Math.min(s1, r1);
      if (i0 > r0) {
        // Unmapped sub-range
        var b1 = Math.min(i0, r1);
        if (b1 > r0) {
          dest.push([r0, b1 - r0]);
          r0 = b1;  
        }
      }
      if (i0 < i1) {
        // Mapped intersection
        var d0 = parseInt(trip[2]);
        dest.push([d0 + i0 - s0, i1 - i0]);
        r0 = i1;
      }
    }
    if (r0 < r1) {
        // Unmapped end-range
        dest.push([r0, r1 - r0]);
    }
  }
  return dest;
}
