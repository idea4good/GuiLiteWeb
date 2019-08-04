const cubeSize = 180;
const gain = 1.1;
const animationFrames = 300;
const transparency = 0.3;

let graphic;

function setup() {
  createCanvas(500, 500, WEBGL);
  angleMode(DEGREES);
  graphic = createGraphics(cubeSize, cubeSize);
}

const faces = [
  [0,   0, 0, '255, 0, 0'],
  [0,  90, 0, '0, 255, 0'],
  [0, 180, 0, '255, 0, 0'],
  [0, -90, 0, '0, 255, 0'],
  [90,  0, 0, '0, 0, 255'],
  [270, 0, 0, '0, 0, 255'],
  ];

function textureFace(face, index) {
  graphic.background(`rgba(${face[3]}, ${transparency})`);
  graphic.noFill();
  graphic.stroke(255);
  graphic.ellipse(mouseX - 150, mouseY - 75, 60, 60);
  texture(graphic);
}

function draw() {
  const progress = min(frameCount / animationFrames, 1);
  background('lightblue');
  noStroke();
  //rotateX(-30);
  rotateY(frameCount);
  faces.forEach((face, i) => {
    fill(`rgba(${face[3]}, ${transparency})`);
    //textureFace(face, i);

    push();
    rotateX(face[0] * progress);
    rotateY(face[1] * progress);
    rotateZ(face[2] * progress);
    translate(0, 0, cubeSize / 2 * gain * progress);
    plane(cubeSize);
    pop();
  });
}
