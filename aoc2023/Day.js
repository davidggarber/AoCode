var raw, output = [], debugs = [];
var lines = [];
var curPart = 0;

function run() {
  init();
  readInput();
  console.log('Part 1...');
  solve1();
  curPart = 1;
  console.log('Part 2...');
  solve2();
}

function init() {
  var body = document.getElementsByTagName('body')[0];

  var h = document.createElement('h3');
  h.appendChild(document.createTextNode('Part 1 '));
  output[0] = document.createElement('textarea');
  output[0].id = 'part1';
  h.appendChild(output[0]);
  body.appendChild(h);

  h = document.createElement('h3');
  h.appendChild(document.createTextNode('Part 2 '));
  output[1] = document.createElement('textarea');
  output[1].id = 'part2';
  h.appendChild(output[1]);
  body.appendChild(h);

  var pre = document.getElementById('pre');
  body.removeChild(pre);
  var text = pre.innerText;
  var table = document.createElement('table');
  var tr = document.createElement('tr');
  tr.innerHTML = '<th>Puzzle input</th><th>1</th><th>2</th>'
  table.appendChild(tr);
  tr = document.createElement('tr');
  var td = document.createElement('td');
  td.appendChild(pre);
  tr.appendChild(td);
  td = document.createElement('td');
  debugs.push(document.createElement('pre'));
  debugs[0].id = 'debug1';
  td.appendChild(debugs[0]);
  tr.appendChild(td);
  td = document.createElement('td');
  debugs.push(document.createElement('pre'));
  debugs[1].id = 'debug2';
  td.appendChild(debugs[1]);
  tr.appendChild(td);
  table.appendChild(tr);
  body.appendChild(table);
}

function print(answer) {
  answer = '' + answer;
  var p = document.getElementById('part' + (curPart + 1))
  p.value = answer;
  p.focus();
  p.setSelectionRange(0, answer.length);
  // navigator.clipboard.writeText(answer);
}

var debugLines = ['', ''];
function trace(text, flush) {
  if (debugLines[curPart].length > 0) {
    text = '\n' + text;
  }
  debugLines[curPart] += text;
  if (flush != false) {
    debugs[curPart].innerText = debugLines[curPart];
  }
}

function flush() {
  debugs[curPart].innerText = debugLines[curPart];
}

function readInput() {
  raw = document.getElementById('pre').innerText;
  // Trim final \n
  raw = raw.substring(0, raw.length - 1);   
  lines = raw.split('\n').map(l => l.trim());
  raw.innerText = lines.join('\n');  
}

function pivot(rows) {
  var columns = [];
  for (var x = 0; x < rows[0].length; x++) {
    var col = rows.map(r => r[x]).reduce((str, ch) => str += ch);
    if (col.trim().length > 0) {
      columns.push(col);
    }
  }
  return columns;
}

function height() {
  return lines.length;
}

function width() {
  return lines[0].length;
}

function charAt(x, y) {
  if (inBounds(x, y)) {
    return lines[y][x];
  }
  return '\0';
}

function inBounds(x, y) {
  return x >= 0 && y >= 0 && y < lines.length && x < lines[0].length;
}

var mapChars = {
  '.': 'dot',
  '#': 'hash',
  '>': 'angle',
  '<': 'angle',
  'v': 'angle',
  '^': 'angle',
  'o': 'ooo',
  'O': 'ooo',
}

function colorMap(id, moreMap) {
  id = id || 'pre';
  moreMap = moreMap || mapChars;
  var pre = document.getElementById(id);
  pre.innerHTML = '';
  var rows = id == 'debug1' ? debugLines[0].split('\n') : id == 'debug2' ? debugLines[1].split('\n') : lines;
  for (var line of rows) {
    var div = document.createElement('div');
    var str = '';
    var strcls = null;
    for (var i = 0; i <= line.length; i++) {
      var ch = i < line.length ? line[i] : '\0';
      var cls = (ch in mapChars) ? mapChars[ch] : (ch in moreMap) ? moreMap[ch] : 'other';
      if (cls != strcls) {
        if (strcls != null) {
          var span = document.createElement('span');
          if (strcls != null) {
            span.classList.add(strcls);
            span.innerText = str;
            div.appendChild(span);
          }
        }
        strcls = cls;
        str = '';
      }
      str += ch;
    }
    pre.appendChild(div);
  }

}