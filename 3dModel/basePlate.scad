width = 8;
height = 8;
pixelSize = 16.6;
baseThickness = 1.0;
wallThickness = 0.8;
wallHeight = 15;
stripThickness = 0.9;
ledCutoutWidth = 7;
ledCutoutHeight = 9;
ledBendspace = 4;



for(i=[0:1:width-1], j=[0:1:height-1]){
    translate([pixelSize*i, pixelSize*j, 0]){
        if(j%2==1){
            mirror([0,1,0])
            translate([0,-pixelSize,0])
              translate([-wallThickness/2, -wallThickness/2,0])
              cube([pixelSize+wallThickness, pixelSize+wallThickness, baseThickness]);
        }
        else{
            translate([-wallThickness/2, -wallThickness/2,0])
            cube([pixelSize+wallThickness, pixelSize+wallThickness, baseThickness]);
        }
    }
};
 