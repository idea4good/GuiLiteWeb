let video;
let image;
let angle = 0;

function setup() {
  video = createVideo(['assets/mcu-wave.mp4']);
  video.elt.muted = true;
  video.loop();
  video.hide();
  image = loadImage('assets/background.jpg')
  createCanvas(800, 600, WEBGL);
}

function drawBoxWithVideo(){
  texture(video);
  box(200, 200, 200);
}

function drawBoxWithImage(){
  texture(image);
  box(200, 200, 200);
}

function drawCustomShap(){
  background('rgb(100, 100, 100)');

  fill(0)
  //texture(image);
  beginShape();
  vertex(0, 0, 50, 0, 0);
  vertex(100, 0, 50, 1, 0);
  vertex(100, 100, 50, 1, 1);
  vertex(0, 100, 50, 0, 1);
  endShape(CLOSE);
}

function draw() {
  rotateZ(0);
  rotateX(0.5);
  rotateY(angle * 0.1);

  //drawBoxWithImage();
  drawBoxWithVideo();
  //drawCustomShap();
  angle += 0.05;
}
