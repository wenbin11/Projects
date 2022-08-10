////////////////////////////////////////////////////////////////
function setupGround(){
  ground = Bodies.rectangle(500, 600, 1000, 40, {
    isStatic: true, angle: 0
  });
  World.add(engine.world, [ground]);
}

////////////////////////////////////////////////////////////////
function drawGround(){
  push();
  fill(128);
  drawVertices(ground.vertices);
  pop();
}
////////////////////////////////////////////////////////////////
function setupPropeller(){
  // your code here
  // STEP 1:
  propeller = Bodies.rectangle(150, 480, 200, 15, {isStatic:true, angle: angle});
  World.add(engine.world, [propeller]);
}
////////////////////////////////////////////////////////////////
//updates and draws the propeller
function drawPropeller(){
  push();
  // your code here
  // STEP 2:
  Body.setAngle(propeller, angle);
  Body.setAngularVelocity(propeller, angleSpeed);
  drawVertices(propeller.vertices);
  angle += angleSpeed;
  pop();
}
////////////////////////////////////////////////////////////////
function setupBird(){
  var bird = Bodies.circle(mouseX, mouseY, 20, {friction: 0,
      restitution: 0.95 });
  Matter.Body.setMass(bird, bird.mass*10);
  World.add(engine.world, [bird]);
  birds.push(bird);
}
////////////////////////////////////////////////////////////////
function drawBirds(){
  push();
  //your code here
  // STEP 3:
  fill(255,0,0);
  for(var i = 0; i < birds.length; i++){
      drawVertices(birds[i].vertices);
      if(isOffScreen(birds[i])){
          removeFromWorld(birds[i]);
          birds.splice(i,1);
          i--;
      }
  }
  pop();
}
////////////////////////////////////////////////////////////////
//creates a tower of boxes
function setupTower(){
  //you code here
  //STEP 4:
    
  // create stacks of boxes
  boxes = Composites.stack(500,180,3,6,0,0, function(x,y) {
       
        return Bodies.rectangle(x,y,80,80, {render:{fillStyle: "green", strokeStyle:"black"}})
       
        });
    // random green shades of boxes
    for (var i =0; i < boxes.bodies.length; i++){
        var g = random(100,200);
        boxes.bodies[i].render.fillStyle = color(0,g,0);
    }
    // adding boxes to World
    World.add(engine.world,[boxes]);
}
////////////////////////////////////////////////////////////////
//draws tower of boxes
function drawTower(){
  push();
  //your code here
  // STEP 5:
  for(var i=0; i < boxes.bodies.length; i++){
      fill(boxes.bodies[i].render.fillStyle);
      stroke(boxes.bodies[i].render.strokeStyle);
      drawVertices(boxes.bodies[i].vertices);
      
      // Implementation -- check boxes left screen
      if(isOffScreen(boxes.bodies[i])){
          removeFromWorld(boxes.bodies[i]);
          boxes.bodies.splice(i,1);
          i--;
      }
  // Implementation -- Print Win when all boxes left the screen
  if(boxes.bodies.length == 0){
      youWin();
  }
}
    pop();
}
////////////////////////////////////////////////////////////////
function setupSlingshot(){
//your code here
// STEP 6:
slingshotBird = Bodies.circle(180, 180, 20, {restitution: 0.95, friction: 0, mass: 10});

var constraint_prop = {pointA: {x: 200, y: 200},
                      bodyB: slingshotBird,
                      pointB: {x:0, y:0},
                      stiffness: 0.01,
                      damping: 0.001};
    
slingshotConstraint = Constraint.create(constraint_prop);

World.add(engine.world, [slingshotBird, slingshotConstraint]);
    
}
////////////////////////////////////////////////////////////////
//draws slingshot bird and its constraint
function drawSlingshot(){
  push();
  // your code here
  // STEP 7:
  fill(255,0,0);
  drawVertices(slingshotBird.vertices);
  drawConstraint(slingshotConstraint);
  pop();
}
////////////////////////////////////////////////////////////
function setupMouseInteraction(){
  var mouse = Mouse.create(canvas.elt);
  var mouseParams = {
    mouse: mouse,
    constraint: { stiffness: 0.05 }
  }
  mouseConstraint = MouseConstraint.create(engine, mouseParams);
  mouseConstraint.mouse.pixelRatio = pixelDensity();
  World.add(engine.world, mouseConstraint);
}
////////////////////////////////////////////////////////////
function drawTimer(){
// Implementation -- Countdown Timer
if(frameCount %60 == 0 && timer > 0){
        timer--;
    }
if (timer < 60 && timer > 5){
    fill(255);
    textSize(40);
    text("Time: " + timer + "sec", 10, 40);
    }
// Set text color to be red if below 5 seconds
else if (timer <= 5){
    fill(255,0,0);
    textSize(40);
    text("Time: " + timer + "sec", 10, 40);
    }
// print GAME OVER when time = 0
if (timer == 0){
    gameOver();
    }
}