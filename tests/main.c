#include "roughdraft.h"
#include <stdio.h>
#include <stdlib.h>

void linearTest(rds shape, const char* file);
void radialTest(rds shape, const char* file);

int main(int argc, char *argv[])
{

	linearTest(line, "./linearLine.png");
	linearTest(rectangle, "./linearRec.png");
	radialTest(circle, "./radialCircle.png");
	radialTest(arc, "./radialArc.png");
	return 0;
}

void linearTest(rds shape, const char* file){
	
	open_draft(500, 500);
	add_rde(shape, red, 8, 65, 65, 75, 65, _2D);
	add_rde(shape, blue, 7, 65, 65, 75, 75, _2D);
	add_rde(shape, green, 6, 65, 65, 65, 75, _2D);
	
	add_rde(shape, cyan, 5, 65, 65, 55, 75, _2D);
	add_rde(shape, brown, 4, 65, 65, 55, 65, _2D);
	
	add_rde(shape, yellow, 3, 65, 65, 55, 55, _2D);
	add_rde(shape, magenta, 2, 65, 65, 65, 55, _2D);
	
	add_rde(shape, black, 1, 65, 65, 75, 55, _2D);
	
	set_background_color(white);
	compile(file, png);
	close_draft();
}

void radialTest(rds shape, const char* file){
	
	open_draft(500, 500);
	
	add_rde(shape, brown, 8, 65, 65, 77, 73, _2D);
	add_rde(shape, brown, 8, 65, 65, 79, 65, _2D);

	add_rde(shape, red, 7, 65, 65, 51, 65, _2D);
	add_rde(shape, red, 7, 65, 65, 53, 73, _2D);	
	
	add_rde(shape, blue, 6, 65, 65, 51, 65, _2D);
	add_rde(shape, blue, 6, 65, 65, 53, 57, _2D);	
	
	add_rde(shape, green, 5, 65, 65, 79, 65, _2D);
	add_rde(shape, green, 5, 65, 65, 77, 57, _2D);	
	
	add_rde(shape, cyan, 4, 65, 65, 65, 75, _2D);
	add_rde(shape, cyan, 4, 65, 65, 75, 65, _2D);
	 
	add_rde(shape, magenta, 3, 65, 65, 55, 65, _2D);
	add_rde(shape, magenta, 3, 65, 65, 65, 75, _2D);	
	
	add_rde(shape, yellow, 2, 65, 65, 55, 65, _2D);
	add_rde(shape, yellow, 2, 65, 65, 65, 55, _2D);	
	
	add_rde(shape, black, 1, 65, 65, 65, 55, _2D);
	add_rde(shape, black, 1, 65, 65, 75, 65, _2D);
	
	set_background_color(white);
	compile(file, png);
	close_draft();
}