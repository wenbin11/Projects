class Spaceship {

  constructor(){
    this.velocity = new createVector(0, 0);
    this.location = new createVector(width/2, height/2);
    this.acceleration = new createVector(0, 0);
    this.maxVelocity = 5;
    this.bulletSys = new BulletSystem();
    this.size = 50;
  }

  run(){
    this.bulletSys.run();
    this.draw();
    this.move();
    this.edges();
    this.interaction();
  }

  draw(){
    fill(211,211,211);
    triangle(this.location.x - this.size/2, this.location.y + this.size/2,
        this.location.x + this.size/2, this.location.y + this.size/2,
        this.location.x, this.location.y - this.size/2);
    fill(255,0,0);
    rect(this.location.x -20, this.location.y + 25, 10, 10);
    rect(this.location.x +10, this.location.y + 25, 10, 10);
  }

  move(){
    // YOUR CODE HERE (4 lines)
    // STEP 3:
    this.velocity.add(this.acceleration);
    this.location.add(this.velocity);
    this.velocity.limit(this.maxVelocity);
    this.acceleration.mult(0);
  }

  applyForce(f){
    this.acceleration.add(f);
  }

  interaction(){
      // STEP 2:
      if (keyIsDown(LEFT_ARROW)){
        this.applyForce(createVector(-0.1, 0));
        // Implementation -- Jet Thrusters 
        fill(255,191,0);
        rect(this.location.x + 26 , this.location.y +5, 20, 7);
      }
      if (keyIsDown(RIGHT_ARROW)){
      // YOUR CODE HERE (1 line)
          this.applyForce(createVector(0.1, 0));
          // Implementation -- Jet Thrusters 
          fill(255,191,0);
          rect(this.location.x - 48 , this.location.y +5, 20, 7);
      }
      if (keyIsDown(UP_ARROW)){
      // YOUR CODE HERE (1 line)
          this.applyForce(createVector(0, -0.1));
          // Implementation -- Jet Thrusters 
          fill(255,191,0);
          rect(this.location.x - 20 , this.location.y +35, 10, 15);
          rect(this.location.x + 10 , this.location.y +35, 10, 15);
      }
      if (keyIsDown(DOWN_ARROW)){
      // YOUR CODE HERE (1 line)
          this.applyForce(createVector(0, 0.1));
          // Implementation -- Jet Thrusters 
          fill(255,191,0);
          rect(this.location.x + 10 , this.location.y -25, 10, 12);
          rect(this.location.x - 20 , this.location.y -25, 10, 12);
      }
  }

  fire(){
    this.bulletSys.fire(this.location.x, this.location.y);
  }

  edges(){
    if (this.location.x<0) this.location.x=width;
    else if (this.location.x>width) this.location.x = 0;
    else if (this.location.y<0) this.location.y = height;
    else if (this.location.y>height) this.location.y = 0;
  }

  setNearEarth(){
    //YOUR CODE HERE (6 lines approx)
    // STEP 9:
    var gravity = createVector(0,0.05);
    var friction = this.velocity.copy();
    friction.mult(-1);
    friction.normalize();
    friction.mult(1/30);
    spaceship.applyForce(friction);
    spaceship.applyForce(gravity);
  }
}
