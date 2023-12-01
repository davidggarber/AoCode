function solve1() {
  var sum = 0;
  for (var x of lines) {
    x = x.replace(/[a-z]/gi, '');
    sum += parseInt(x[0] + x[x.length - 1]);
  }

  print(1, sum);
}

var digits = {
  'zero':'z0o',
  'one':'o1e',
  'two':'t2o',
  'three':'t3e',
  'four':'f4r',
  'five':'f5e',
  'six':'s6x',
  'seven':'s7n',
  'eight':'e8t',
  'nine':'n9e'
};

function solve2() {
  var sum = 0;
  for (var x of lines) {
    for (var j of Object.keys(digits)) {
      x = x.replaceAll(j, digits[j]);
    }
    x = x.replace(/[a-z]/gi, '');
    sum += parseInt(x[0] + x[x.length - 1]);
    trace(2, x);
  }
  
  print(2, sum);
}
