width = 5;
height = 7;
pixelSize = 16.6;
baseThickness = 1.0;
wallThickness = 0.8;
wallHeight = 15;
stripThickness = 0.9;
ledCutoutWidth = 7;
ledCutoutHeight = 9;
ledBendspace = 4;

module pixelCell(){
    // base plate, moved to seperate file
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
        translate([pixelSize-wallThickness/2-2.9, 0, 0]){
            //cube([2.9, wallThickness, 8.0]);
            translate([-2.4, 0, 0]){
                cube([2.4, wallThickness, 4]);
            }
        }
    }
}
//pixelCell();

difference(){
for(i=[0:1:width-1], j=[0:1:height-1]){
    translate([pixelSize*i, pixelSize*j, 0]){
        if (j == 0){    
//            cube([pixelSize, pixelSize, wallHeight]);
                translate([pixelSize/2, pixelSize/2, 5]);
                difference() {
//                    
                    cube(center=true, [pixelSize,pixelSize,wallHeight]);
                    rotate([90,0,0]) translate([0,-5,10]) cylinder(r=ledCutoutHeight/2,h=pixelSize,$fn=100) ;
                }
//            cube([pixelSize, pixelSize, wallHeight]);
//             cylinder(h=ledCutoutHeight+baseThickness, r=ledCutoutWidth/2);  
//            difference() {
//                translate([pixelSize/2, pixelSize/2, -baseThickness]) { 
//                    difference() {
//                    rotate([90, 0,0]);
//                    cylinder(h=ledCutoutHeight+baseThickness, r=ledCutoutWidth/2);   
//                    }
//                }
//            }   
        }
        if(j%2==1){
            mirror([0,1,0]);
            translate([0,-pixelSize,0]);
            pixelCell();
        }
        else{
            pixelCell();
        }
    }
};
translate([-ledBendspace-wallThickness/2, 0, 0])
cube([ledBendspace, height*pixelSize+wallThickness, wallHeight]);
}