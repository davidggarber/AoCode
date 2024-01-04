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
  Given:
    A: ax,ay,az + t(ix,iy,iz)
    B: bx,by,bz + u(jx,jy,jz)
    C: cx,cy,cz + v(kx,ky,kz)
  ...
  There exists a point on A, and a vector to a point on C (at a different time u!=t)
  Continuing on, it must then hit E.
    B: ax,ay,az + (u-t)(hx,hy,hz)
    C: ax,ay,az + (v-t)(hx,hy,hz)
    where t, u, v, hx, hy, hz are all unknown

    bx+u*jx == ax+(u-t)hx
    by+u*jy == ay+(u-t)hy
    bz+u*jz == az+(u-t)hz
    cx+v*kx == ax+(v-t)hx
    cy+v*ky == ay+(v-t)hy
    cz+v*kz == az+(v-t)hz
  
    hx = (bx-ax+u*jx)/(u-t) = (cx-ax+v*kx)/(v-t)
    hy = (by-ay+u*jy)/(u-t) = (cy-ay+v*ky)/(v-t)
    hz = (bz-az+u*jz)/(u-t) = (cz-az+v*kz)/(v-t)
    Three unknowns: t, u, v, where t != u != v

    Isolate v with x:
      (v-t)*(bx-ax+u*jx) = (cx-ax+v*kx)*(u-t)
      v*(bx-ax) - t*(bx-ax) + v*u*jx - t*u*jx = u*(cx-ax) - t*(cx-ax) + u*v*kx - t*v*kx
      v*(bx-ax) + v*u*jx - u*v*kx + t*v*kx = u*(cx-ax) - t*(cx-ax) + t*(bx-ax) + t*u*jx 
      v * ((bx-ax) + u*(jx-kx) + t*kx) = u*(cx-ax) - t*(cx-ax) + t*(bx-ax) + t*u*jx 
      v = (u*(cx-ax) + t*(bx-cx) + t*u*jx) / ((bx-ax) + u*(jx-kx) + t*kx)

    Isolate v with y:
      v = (u*(cy-ay) + t*(by-cy) + t*u*jy) / ((by-ay) + u*(jy-ky) + t*ky)

    Isolate u or t:
      (u*(cx-ax) + t*(bx-cx) + t*u*jx) * ((by-ay) + u*(jy-ky) + t*ky)
        = (u*(cy-ay) + t*(by-cy) + t*u*jy) * ((bx-ax) + u*(jx-kx) + t*kx)
      u^2*(cx-ax)*(jy-ky) + u^2*t*jx*(jy-ky) + t*u*((bx-cx)*(jy-ky) + ky*(cx-ax) + jx*(by-ay)) + t^2*u*jx*ky + t^2*(bx-cx)*ky + u*(cx-ax)*(by-ay) + t*(bx-cx)*(bx-ax)
        = u^2*(cy-ay)*(jx-kx) + u^2*t*jy*(jx-kx) + t*u*((by-cy)*(jx-kx) + kx*(cy-ay) + jy*(bx-ax)) + t^2*u*jy*kx + t^2*(by-cy)*kx + u*(cy-ay)*(bx-ax) + t*(by-cy)*(by-ay)
      u^2*((cx-ax)*(jy-ky)-(cy-ay)*(jx-kx)) + u^2*t*(jx*(jy-ky)-jy*(jx-kx)) + t*u*(((bx-cx)*(jy-ky) + ky*(cx-ax) + jx*(by-ay)) - ((by-cy)*(jx-kx) + kx*(cy-ay) + jy*(bx-ax))) + t^2*u*jx*ky + t^2*(bx-cx)*ky + u*(cx-ax)*(by-ay) + t*(bx-cx)*(bx-ax)
      
    *** This would be much simpler if any of the subtractions came out as zero:
        bx-cx, cy-ay, jx-kx, etc.
      The goal is to get some cubic terms (i.e. u^2*t) to drop away
    *** Similarly, if any vector component (i.e. ix) were zero, then the t*u partial component would be zero
      So... find samples with zero components in offset vectors
      then find repeated x/y/z elsewhere


*/


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