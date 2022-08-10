var spaceship;
var asteroids;
var atmosphereLoc;
var atmosphereSize;
var earthLoc;
var earthSize;
var starLocs = [];
var score = 0;

//////////////////////////////////////////////////
function setup() {
  createCanvas(1200,800);
  spaceship = new Spaceship();
  asteroids = new AsteroidSystem();

  //location and size of earth and its atmosphere
  atmosphereLoc = new createVector(width/2, height*2.9);
  atmosphereSize = new createVector(width*3, width*3);
  earthLoc = new createVector(width/2, height*3.1);
  earthSize = new createVector(width*3, width*3);
}

//////////////////////////////////////////////////
function draw() {
  background(0);
  sky();

  spaceship.run();
  asteroids.run();

  drawEarth();
  ScoreBoard();

  checkCollisions(spaceship, asteroids); // function that checks collision between various elements
}

//////////////////////////////////////////////////
//draws earth and atmosphere
function drawEarth(){
  noStroke();
  //draw atmosphere
  fill(173, 216, 230, 100);
  ellipse(atmosphereLoc.x, atmosphereLoc.y, atmosphereSize.x,  atmosphereSize.y);
  //draw earth
  fill(0,0,255,75);
  ellipse(earthLoc.x, earthLoc.y, earthSize.x, earthSize.y);
}

//////////////////////////////////////////////////
//checks collisions between all types of bodies
function checkCollisions(spaceship, asteroids){

    //spaceship-2-asteroid collisions
    //YOUR CODE HERE (2-3 lines approx)
    // STEP 5:
    for(var i=0; i < asteroids.locations.length;i++){
    if(isInside(spaceship.location, spaceship.size, asteroids.locations[i], asteroids.diams[i]) == true){
        gameOver();
    }
    
    }
    //asteroid-2-earth collisions
    //YOUR CODE HERE (2-3 lines approx)
    // STEP 6:
    for(var i=0; i < asteroids.locations.length;i++){
        if(isInside(asteroids.locations[i], asteroids.diams[i], earthLoc, earthSize.y) == true){
            gameOver();
        }
    }
    
    
    //spaceship-2-earth
    //YOUR CODE HERE (1-2 lines approx)
    // STEP 7:
    if(isInside(spaceship.location, spaceship.size, earthLoc, earthSize.y) == true){
        gameOver();
    }
    //spaceship-2-atmosphere
    //YOUR CODE HERE (1-2 lines approx)
    // STEP 8:
    if(isInside(spaceship.location, spaceship.size, atmosphereLoc, atmosphereSize.y) == true){
        spaceship.setNearEarth();
    }

    //bullet collisions
    //YOUR CODE HERE (3-4 lines approx)
    // STEP 10:
    var bulletSys = spaceship.bulletSys;
    var bullets = bulletSys.bullets;
    for (var i = 0; i < bullets.length; i++){
        for(var j = 0; j < asteroids.locations.length; j++){
            var check = isInside(asteroids.locations[j], asteroids.diams[j], bullets[i], bulletSys.diam);
            if(check == true){
                asteroids.destroy(j);
                // Implementation -- increment scores in ScoreBoard
                score = score+1;
            }
        }
    }
}

//////////////////////////////////////////////////
//helper function checking if there's collision between object A and object B
function isInside(locA, sizeA, locB, sizeB){
    // YOUR CODE HERE (3-5 lines approx)
    // STEP 4:
    if(dist(locA.x,locA.y,locB.x,locB.y) < sizeA/2 + sizeB/2){
        return true;
    }else return false;
}

//////////////////////////////////////////////////
function keyPressed(){
  if (keyIsPressed && keyCode === 32){ // if spacebar is pressed, fire!
    spaceship.fire();
  }
}

//////////////////////////////////////////////////
// function that ends the game by stopping the loops and displaying "Game Over"
function gameOver(){
  fill(255);
  textSize(80);
  textAlign(CENTER);
  text("GAME OVER", width/2, height/2)
  noLoop();
}

//////////////////////////////////////////////////
// function that creates a star lit sky
function sky(){
  push();
  while (starLocs.length<300){
    starLocs.push(new createVector(random(width), random(height)));
  }
  fill(255);
  for (var i=0; i<starLocs.length; i++){
    rect(starLocs[i].x, starLocs[i].y,2,2);
  }

  if (random(1)<0.3) starLocs.splice(int(random(starLocs.length)),1);
  pop();
}
///////////////////////////////////////////////
function ScoreBoard(){
    // Implementation -- ScoreBoard draw Function
    fill(255);
    textSize(25);
    text("ScoreBoard: ", 0, 30);
    text(score, 150, 30);
}




