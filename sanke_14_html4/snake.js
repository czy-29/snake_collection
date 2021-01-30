//snake.js

function CreatePoint (x,y)
{
var pt = new Object;
pt.x = x;
pt.y = y;
return pt;
}

var snake = new Array();
var input = new Array(0);
var map   = new Array (20);
var food  = CreatePoint (0,0);
var cur_dir;
var bFast = false,bPause = false,bStart = false;
var iTimer,nTimerCount = 0;
var score;
var i,j;

for (i = 0;i < 20;i ++) map[i] = new Array (15);

function CreateTimer (delay)
{
iTimer = window.setInterval ("GameLoop()",delay);
nTimerCount ++;
}

function KillTimer ()
{
window.clearInterval (iTimer);
nTimerCount --;
}

function NewGame ()
{
if (bStart && !bPause) KillTimer();

snake.length = 3;
snake[0] = CreatePoint (10,10);
snake[1] = CreatePoint (11,10);
snake[2] = CreatePoint (12,10);

input = new Array (0);

for (i = 0;i < 20;i ++)
for (j = 0;j < 15;j ++)
map[i][j] = 0;

cur_dir = 37;
score = 0;
bFast = false;
bPause = false;
RandFood();
bStart = true;

CreateTimer(500);
PrintGame();
}

function RandFood ()
{
var pt = CreatePoint(0,0);

do
{
pt.x = Math.floor (Math.random() * 20);
pt.y = Math.floor (Math.random() * 15);
}
while (CheckCover (pt,0))

food = pt;
}

function CheckCover (pt,num)
{
for (i = num;i < snake.length;i ++)
{
if (pt.x == snake[i].x && pt.y == snake[i].y)
return true;
}

return false;
}

function GamePause ()
{
if (bStart)
{
if (bPause)
{
CreateTimer (500);
bPause = false;
}
else
{
KillTimer();
bPause = true;
}
}
}

function StopGame ()
{
if (!bPause && bStart) KillTimer ();
PrintBlank();
bStart = false;
}


function SpacePress ()
{
if (window.event.keyCode == 32) GamePause ();
}

function PushKey (num1,num2)
{
if (input.length == 0 && (cur_dir == num1 || cur_dir == num2))
input.push (window.event.keyCode);
else if (input.length > 0)
{
if (input[input.length - 1] == num1 || input[input.length - 1] == num2)
input.push (window.event.keyCode);
}
}

function ChnDir ()
{
if (bStart && !bPause)
{
if (window.event.keyCode == cur_dir && input.length == 0 && !bFast)
{
KillTimer();
CreateTimer (50);
bFast  = true;
}
else
{
if (bFast && (window.event.keyCode != cur_dir || input.length > 0))
{
KillTimer();
CreateTimer (500);
bFast = false;
}

switch (window.event.keyCode)
{
case 37:
PushKey (38,40);
break;

case 38:
PushKey (37,39);
break;

case 39:
PushKey (38,40);
break;

case 40:
PushKey (37,39);
break;
}
}
}
}

function SlowDown ()
{
if (bStart && bFast && window.event.keyCode >= 37 && window.event.keyCode <= 40)
{
KillTimer ();
CreateTimer (500);
bFast = false;
}
}

function SnakeMove (x,y)
{
var pt = CreatePoint (0,0);

pt.x = snake[0].x + x;
pt.y = snake[0].y + y;

if (pt.x < 0) pt.x = 19;
else if (pt.y <  0)  pt.y = 14;
else if (pt.x >= 20) pt.x = 0;
else if (pt.y >= 15) pt.y = 0;

snake.unshift (pt);
}

function GameLoop ()
{
if (input.length > 0) cur_dir = input.shift();

switch (cur_dir)
{
case 37:
SnakeMove (-1,0);
break;

case 38:
SnakeMove (0,-1);
break;

case 39:
SnakeMove (1,0);
break;

case 40:
SnakeMove (0,1);
break;
}

if (snake[0].x == food.x && snake[0].y == food.y)
{
RandFood();
score++;
}
else snake.pop();

if (CheckCover (snake[0],3))
{
KillTimer();
window.alert ("Game End! \nYour score: " + score);
PrintBlank();
bStart = false;
}
else PrintGame();
}

function PrintBlank ()
{
var id = document.getElementById ("Main");
var str = "";

str += "\n<table border = \"0\" cellspacing = \"0\" cellpadding = \"0\">\n";

for (j = 0;j < 15;j ++)
{
str += "<tr>\n";
for (i = 0;i < 20;i ++) str += "<td width = \"32\" height = \"32\"> &nbsp </td>\n";
str += "</tr>\n";
}

str += "</table>\n";

id.innerHTML = str;
}

function PrintGame ()
{
var id = document.getElementById ("Main");
var str = "";

for (i = 0;i < 20;i ++)
for (j = 0;j < 15;j ++)
map[i][j] = 0;

map[snake[0].x][snake[0].y] = 1;

for (i = 1;i < snake.length;i ++)
map[snake[i].x][snake[i].y] = 2;

map[food.x][food.y] = 3;

str += "\n<table border = \"0\" cellspacing = \"0\" cellpadding = \"0\">\n";

for (j = 0;j < 15;j ++)
{
str += "<tr>\n";

for (i = 0;i < 20;i ++)
{
str += "<td width = \"32\" height = \"32\" ";

switch (map[i][j])
{
case 1:
str += "bgcolor = \"red\"";
break;

case 2:
str += "bgcolor = \"green\"";
break;

case 3:
str += "bgcolor = \"blue\"";
break;
}

str += "> &nbsp </td>\n";
}

str += "</tr>\n";
}

str += "</table>\n";

id.innerHTML = str;
}
