width = 20;
height = 7;
pixelSize = 16.6;
baseThickness = 1.0;
wallThickness = 0.8;
wallHeight = 15;
stripThickness = 0.8;
ledCutoutWidth = 7;
ledCutoutHeight = 9;
ledBendspace = 4;

module pixelCell(){
    // base plate
    translate([-wallThickness/2, -wallThickness/2,0])
    cube([pixelSize+wallThickness, pixelSize+wallThickness, baseThickness]);
    
    // main walls around cell
    translate([0, -wallThickness/2, 0]){
        cube([pixelSize, wallThickness, wallHeight]);
        translate([-wallThickness/2, 0, 0]){
            difference(){
                cube([wallThickness, pixelSize-stripThickness-2.0, wallHeight]);
                // cut out hole for bypassing led strip on the end
                translate([0,pixelSize/2+wallThickness/2-ledCutoutWidth/2, baseThickness])
                cube([wallThickness, ledCutoutWidth, ledCutoutHeight]);
            }
            translate([-ledBendspace, pixelSize, 0])
            cube([pixelSize+wallThickness, wallThickness, wallHeight]);
            
        }
        translate([pixelSize-wallThickness/2, 0, 0])
        cube([wallThickness, pixelSize-stripThickness-2.0, wallHeight]);
    }
    
    // walls to hold led in place
    translate([0, pixelSize-stripThickness-wallThickness*1.5,0]){
        translate([wallThickness/2+2.9, 0, 0]){
            //rotate([0,0,-135])
            //translate([0, wallThickness, 0])
            //cube([2.9, wallThickness, 8.0]);
            cube([2.4, wallThickness, 4]);
        }
        /*translate([pixelSize-wallThickness/2-2.9, 0, 0]){
            cube([2.9, wallThickness, 8.0]);
            translate([-2.4, 0, 0]){
                cube([2.4, wallThickness, 4]);
            }
        }*/
    }
}
pixelCell();
/*
difference(){
for(i=[0:1:width-1], j=[0:1:height-1]){
    translate([pixelSize*i, pixelSize*j, 0]){
        if(j%2==1){
            mirror([0,1,0])
            translate([0,-pixelSize,0])
            pixelCell();
        }
        else{
            pixelCell();
        }
    }
};
translate([-ledBendspace-wallThickness/2, 0, 0])
cube([ledBendspace, height*pixelSize+wallThickness, wallHeight]);
}*/