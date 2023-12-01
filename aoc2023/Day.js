var raw, output = [], debugs = [];
var lines = [];

function run() {
  init();
  readInput();
  solve1();
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

function print(part, answer) {
  answer = '' + answer;
  var p = document.getElementById('part' + part)
  p.value = answer;
  p.focus();
  p.setSelectionRange(0, answer.length);
  // navigator.clipboard.writeText(answer);
}

var debugLines = ['', ''];
function trace(part, text) {
  part--;
  if (debugLines[part].length > 0) {
    text = '\n' + text;
  }
  debugLines[part] += text;
  debugs[part].innerText = debugLines[part];
}

function readInput() {
  raw = document.getElementById('pre').innerText;
  // Trim final \n
  raw = raw.substring(0, raw.length - 1);   
  lines = raw.split('\n');
  var indent = raw.match(/\s+/);
  if (indent && indent.length > 0 && indent.index == 0) {
    indent = indent[0].length;
    for (var i = 0; i < lines.length; i++) {
      lines[i] = lines[i].substring(indent);  // remove indent
    }
    raw.innerText = lines.join('\n');  
  }
}

function solve1() {
  var sum = 0;
  var debug = [];
  for (var a of lines) {
    debug.push(a);
  }

  partial(1, debug.join('\n'));
  print(1, sum);
}

function solve2() {
  var sum = 0;
  var debug = [];
  for (var a of lines) {
    debug.push(a);
  }

  // partial(2, debug.join('\n'));
  // print(2, sum);
}
