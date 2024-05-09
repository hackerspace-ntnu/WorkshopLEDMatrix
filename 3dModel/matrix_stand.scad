width = 100;
height = width*2;
baseThickness = 4;
pegHeight = 8;
pegRadius = 5;
padding = width/12;
holeWidth = 3*padding;
holeHeight = width-4*padding;
holeCornerRadius = 2.5;

// from https://gist.github.com/tinkerology/ae257c5340a33ee2f149ff3ae97d9826
module roundedcube(xx, yy, height, radius, center=false, centerxy=false)
{
    translate([(center||centerxy)?-xx/2:0,(center||centerxy)?-yy/2:0,center?0:height/2])
    hull()
    {
        translate([radius,radius,0])
        cylinder(height,radius,radius,true);

        translate([xx-radius,radius,0])
        cylinder(height,radius,radius,true);

        translate([xx-radius,yy-radius,0])
        cylinder(height,radius,radius,true);

        translate([radius,yy-radius,0])
        cylinder(height,radius,radius,true);
    }
}

       
difference() {
    cube([width, height, baseThickness]);
    translate([padding*1.5, padding*1.5, 0]) {
        roundedcube(holeWidth, holeHeight, baseThickness, holeCornerRadius);                
    }   
    translate([width-padding*4.5, padding*1.5, 0]) {
        roundedcube(holeWidth, holeHeight, baseThickness, holeCornerRadius);                
    } 
    translate([width-padding*4.5, height-padding*10 ,0]) {
        roundedcube(holeWidth, holeHeight, baseThickness, holeCornerRadius);                
    } 
    translate([width-padding*10.5, height-padding*10, 0]) {
        roundedcube(holeWidth, holeHeight, baseThickness, holeCornerRadius);                
    } 
}

translate([width/2, height/2, baseThickness]) {
    cylinder(h=pegHeight, r=pegRadius);
}
translate([width/2, height-padding*4, baseThickness]) {
    cylinder(h=pegHeight, r=pegRadius);
}
translate([width/2, padding*4, baseThickness]) {
    cylinder(h=pegHeight, r=pegRadius);
}