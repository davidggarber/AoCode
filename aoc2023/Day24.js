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


/*
  Reviewing sample data, there are pairs of hail vectors with matching dx, dy, or dz.
  The rock must traverse from one to the other in a whole number of seconds.
  So the rock's dx/dy/dz comes from the subset of (shared delta) +/- (integer factors of starting point offsets)

  The most limiting case in my sample data: 
    2 hail vectors have dy=25 and y=252498326441926
      (but vectors' x and z don't otherwise intersect)
      Since hail starting offset = 0, rock's dy = 25 +/- 0 = 25
    dx and dy can be determined by intersections of possible options
  
  Once we've established the rock's dx/dy/dz, take any pair of hail vectors to see where they must align.
  Look back along that vector to get the starting point.
 */

function solve2() {
  var vecs = [];
  for (var line of lines) {
    var h = line.match(/[0-9\-]+/g);
    h = h.map(hh => parseInt(hh));
    vecs.push(h);
  }

  var delta = [0,1,2].map(d => deltaFromSynchronizedVectors(vecs, d));

  var start = startFromAlignment(vecs[0], vecs[1], delta);
  trace('Start = ' + start.join(', '));
  print(start[0] + start[1] + start[2]);
}

function deltaFromSynchronizedVectors(vecs, axis) {
  // Reduce to one axis and its delta
  var simp = vecs.map(v => [v[axis],v[axis+3]]);
  var rates = {};
  for (var v of simp) {
    if (!(v[1] in rates)) {
      rates[v[1]] = [];
    }
    rates[v[1]].push(v[0]);
  }

  var deltas = [];  // [rate, [list of offsets]]

  for (var r of Object.keys(rates)) {
    if (rates[r].length > 1) {
      // trace(rates[r].length + ' vectors all moving at ' + r);
      var rr = rates[r].sort();
      var diffs = [];
      for (var i = 1; i < rates[r].length; i++) {
        diffs.push(rr[i] - rr[i-1]);
      }
      var g = gcdN(diffs);
      var facts = allFactors(g);
      // trace('  gcd= ' + g);
      deltas.push([parseInt(r), facts]);
    }
  }

  var slopes = null;
  for (var d of deltas) {
    var s = {};
    for (var f of d[1]) {
      s[d[0] + f] = d[0] + f;
      s[d[0] - f] = d[0] - f;
    }
    // trace('and ' + Object.values(s).join(', '));
    slopes = intersectSets(slopes, s);
    // trace('  = ' + Object.values(slopes).join(', '));
  }

  trace('Possible slopes: ' + Object.keys(slopes).join(', '));

  // Should be a singleton
  return Object.values(slopes)[0];
}

function intersectSets(a, b) {
  if (a == null) {
    return b;
  }
  var c = {};
  for (var k of Object.keys(a)) {
    if (k in b) {
      c[k] = parseInt(k);
    }
  }
  return c;
}

function gcdN(v) {
  var prev = v[0];
  for (var i = 1; i < v.length; i++) {
    prev = gcd2(prev, v[i]);
  }
  return prev;
}


function gcd2(a,b) {
  if (!b) {
    return a;
  }
  return gcd2(b, a % b);
}

function primeFactors(n) {
  var primes = []; // [1, n];
  if (n < 0) {
    n = -n;
  }
  var f = 2;
  while (f * f <= n) {
    if (n % f == 0) {
      primes.push(f);
      n /= f;
      // factors.push(n);
    }
    else {
      f += f == 2 ? 1 : 2;  // 2, 3, odds...
    }
  }
  if (n != 1) {
    primes.push(n);
  }
  return primes;
}

function allFactors(n) {
  if (n == 0) {
    return [0];
  }
  var primes = primeFactors(n);
  var factors = {'1':1};
  for (var i = 1; i < (1 << primes.length); i++) {
    var f = 1;
    for (var b = 0; b < primes.length; b++) {
      if ((i & (1 << b)) == (1 << b)) {
        f *= primes[b];
      }
    }
    factors[f] = f;
  }
  return Object.values(factors);
}

function startFromAlignment(a, b, d) {
  // Define a plane from the vectors a' and d, including point a.
  // Where does ray b pass through that plane?

  var plane = [a[4]*d[2] - a[5]*d[1], a[5]*d[0] - a[3]*d[2], a[3]*d[1] - a[4]*d[0]];
  plane.push(a[0] * plane[0] + a[1] * plane[1] + a[2] * plane[2]);  // Ax + By + Cz = D

  trace('Given hail ' + a.join(', '));
  trace('  and rock vector ' + d.join(', '));
  trace('  plane is ' + plane[0] + 'x + ' + plane[1] + 'y + ' + plane[2] + 'z = ' + plane[3]);

  // Confirm for a at t=1 (a + a')
  // var aa = [a[0] + a[3], a[1] + a[4], a[2] + a[5]];
  // trace('D1= ' + (aa[0] * plane[0] + aa[1] * plane[1] + aa[2] * plane[2]));
  // // Confirm for a + d
  // aa = [a[0] + d[0], a[1] + d[1], a[2] + d[2]];
  // trace('D2= ' + (aa[0] * plane[0] + aa[1] * plane[1] + aa[2] * plane[2]));


  // A(bx + tbx') + B(by + tby') + C(bz + tbz') = D
  // t = (D - Abx - Bby - Cbz) / (Abx' + Bby' + Cbz');
  // REVIEW: why is round required? Javascript bit depth?
  var t = Math.round((plane[3] - plane[0] * b[0] - plane[1] * b[1] - plane[2] * b[2])
    / (plane[0] * b[3] + plane[1] * b[4] + plane[2] * b[5]));
  var bt = [b[0] + t*b[3], b[1] + t*b[4], b[2] + t*b[5]];
  trace('At time ' + t + ', hail ' + b.join(', ') + ' intersects the plane at ' + bt.join(','));
  // trace('DB= ' + (bt[0] * plane[0] + bt[1] * plane[1] + bt[2] * plane[2]));
  return [bt[0] - t*d[0], bt[1] - t*d[1], bt[2] - t*d[2]];
}
