function solve1() {
  var min = 200000000000000;
  var max = 400000000000000;

  var hail = [];
  var uniqueV = true;

  for (var line of lines) {
    var h = line.match(/[0-9\-]+/g);
    h = h.map(hh => parseFloat(hh));
    if (hail.filter(hh => hh[3] != h[3] || hh[4] != h[4]).length > 0) { uniqueV = false; }
    // h = h.map(hh => {'x':hh[0],'y':hh[1],'z':hh[2],x:dx[3],dy:hh[4],dz:hh[5]});
    hail.push(h);
  }

  var sum = 0;
  for (var i = 0; i < hail.length; i++) {
    for (var j = i + 1; j < hail.length; j++) {
      var x = intersect2d(hail[i], hail[j]);
      // console.log(hail[i].join(',') + ' crosses ' + hail[j].join(',') + ' at ' + x.join(','));
      if (x[0] != NaN && x[0] >= min && x[0] <= max && x[1] >= min && x[1] <= max) {
        sum++;
      }
    }
  }

  print(sum);
}

function intersect2d(a, b) {
  // ax + t*ax' == bx + u*bx'
  // ay + t*ay' == by + u*by'
  // t == (bx - ax)/ax' + u*bx'/ax'
  // t == (by - ay)/ay' + u*by'/ay'
  //  + u*bx'/ax' == (by - ay)/ay' + u*by'/ay'
  // u == ((by - ay)/ay' - (bx - ax)/ax') / (bx'/ax' - by'/ay')
  var u = ((b[1] - a[1])/a[4] - (b[0] - a[0])/a[3]) / (b[3]/a[3] - b[4]/a[4]);
  var t = (b[1] - a[1])/a[4] + u * b[4] / a[4];
  if (u >= 0 && t >= 0) {
    return [b[0]+b[3]*u, b[1] + b[4] * u];
  }
  return [NaN, NaN];
}

function solve2() {
  // find pairs of parallel rays
  var hail = [];
  var vecs = {};
  var parallels = [];
  for (var line of lines) {
    var h = line.match(/[0-9\-]+/g);
    h = h.map(hh => parseInt(hh));
    var v = h.slice(3, 6);
    v[0] = 0;  // ignore z
    var gcd = gcd3(v);
    v = v.map(vv => vv / gcd);
    if (v in vecs) {
      parallels[v] = [vecs[v], h];
    }
    vecs[v] = h;
  }

  print(parallels.length);
}

function gcd3(v) {
  return gcd2(gcd2(v[0],v[1]),v[2]);
}

function gcd2(a,b) {
  if (!b) {
    return a;
  }
  return gcd2(b, a % b);
}