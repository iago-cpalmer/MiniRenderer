#include "main.h"

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(52, 41, red);
	image.flip_vertically(); // set the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

