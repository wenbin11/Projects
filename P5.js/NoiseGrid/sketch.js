var stepSize = 20;
var t = 0;

function setup() {
  createCanvas(500, 500);
}
///////////////////////////////////////////////////////////////////////
function draw() {
  background(125);

  colorGrid();
  compassGrid();
}
///////////////////////////////////////////////////////////////////////
function colorGrid(){
  // your code here
    fill(255);
    stroke(color(0));
    var blue = color(30,144,255);
    var yellow = color(255,216,0);
    // STEP 3:
    var factor = map(mouseX, 0, width, 0, 1);
    
    // STEP 1:
    for (var y = 0; y < 25; y++) {
    for (var x = 0; x < 25; x++) {
        // STEP 2: 
        var tx = (x*3 + frameCount*factor) * 0.01;
        var ty = (y*3 + frameCount*factor) * 0.01;
        var n = noise(tx, ty);
        var c = lerpColor(blue, yellow, n); // implementation: changing the color between blue and yellow instead
        fill(c);
        noStroke();
        rect(x * 20, y * 20, stepSize, stepSize);
    }
    }
    
    
}
///////////////////////////////////////////////////////////////////////
function compassGrid(){
        fill(255);
        stroke(color(0));
        strokeWeight(2);
        var red = color(255, 0, 0);
        var black = color(0);
        var factor = map(mouseX, 0, width, 0, 1);
        // STEP 4: 
        for(var x = 0; x < 25; x++){
        for( var y =0; y < 25; y ++){
            push();
            translate(x*20 + 10, y*20 +10);
            // STEP 6:
            var mRotate = map(mouseX, 0, width, 360, 0);
            // STEP 5:
            var tx = (x*3 + frameCount*factor) * 0.01;
            var ty = (y*3 + frameCount*factor) * 0.01;
            var n = noise(tx, ty);
            var angle = map(n, 0, 1, 0, 720);
            var length = map(n, 0, 1, 0, 30); 
            var c = lerpColor(red, black, n); // implementation: change compass lines color with mouseX
            rotate(radians(angle));
            stroke(color(c));
            line(0, -5, 0, length); // implementation: change compass lines length with mouseX
            pop();
        }
    }
    
}
