
// step 5: create 2 global arrays
var confLocs = [];
var confTheta = [];

let slider1, slider2;

function setup() {
    createCanvas(900, 800, WEBGL);
    angleMode(DEGREES);
    
    // step 5:
    for(var i = 0; i < 200; i++) {
        var rand_x = random(-500,500);
        var rand_y = random(-800,0);
        var rand_z = random(-500,500);
        var rand_vector = createVector(rand_x,rand_y,rand_z);
        confLocs.push(rand_vector);
        
        var rand_angle = random(0,360);
        confTheta.push(rand_angle);
    }
    
    // Implementation: Adding Sliders to canvas 
    push();
    text1 = createElement('p', 'Cube Size' );
    text1.style('color', 'white');
    text1.position(10,0);
    slider1 = createSlider(0, 800, 50, 10);
    slider1.position(80,15);
    
    text2 = createElement('p', 'Sine Wave' );
    text2.style('color', 'white');
    text2.position(10,35);
    slider2 = createSlider(0, 10, 1, 1);
    slider2.position(80,50);
    pop();
}

function draw() {
    background(125);
//  camera(800, -600, 800, 0, 0, 0, 0, 1, 0); step 1: point camera to location(800,-600,800)
    
    // step 4: change camera to fly in a circle around the structure
    var locX = cos(frameCount) * height;
    var locZ = sin(frameCount) * height;
    camera(locX, -600, locZ, 0, 0, 0, 0, 1, 0);
    
    // step 1: create nested for loop to generate grid of boxes
    for(var x = -400; x < 400; x+=50) {
        for(var z = -400; z < 400; z+=50) {
            push();
            // step 2: set material to normal
            // normalMaterial();
            
            // Implementation: Changing material and add lights to the grid structure
            ambientMaterial(250);
            directionalLight(255,255,0, 500, 0, 500);
            directionalLight(0,255,0, 400, 400, 400);
            // Implementation: getting the values of the slider 
            let cube_val = slider1.value();
            let sin_val = slider2.value();
            translate(x,0,z);
            // step 3: calculate distance and create variable length
            var distance = dist(0, 0, x, z) + (frameCount * sin_val); // Implementation: multiplying slider value to the framecount to make sine wave faster
            var length = map(sin(distance), -1, 1, 100, 300);
            // step 2: set stroke to be 0 and stroke weight to 2
            stroke(0);
            strokeWeight(2);
            // setting the box width and depth to be based on slider values
            box(cube_val, length, cube_val);
            pop();
            
        }
    }
    confetti();
}

// step 5:
function confetti() {
    for(var i = 0; i < confLocs.length; i++) {
        push();
        translate(confLocs[i].x,confLocs[i].y,confLocs[i].z);
        rotateX(confTheta[i]);
        plane(15,15);
        
        // step 6:
        confLocs[i].y+=1;
        confTheta[i]+=10;
        
        if(confLocs[i].y > 0) {
            confLocs[i].y = -800;
        }
        pop();
    }
}
