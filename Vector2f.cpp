#include "Vector2f.h"
Vector2f::Vector2f() {

}
Vector2f::Vector2f(float x, float y) {
	this->x = x;
	this->y = y;
}

float Vector2f::DotProduct(Vector2f b) {
	return x*b.x + y*b.y;
}
float Vector2f::Magnitude() {
	return sqrt(x*x+y*y);
}

void Vector2f::Normalize() {
	float m = Magnitude();
	this->x = this->x/m;
	this->y=this->y/m;
	this->z = this->z/m;
}

float Vector2f::Distance(Vector2f b) {
	return sqrt(pow(x-b.x, 2) + pow(y-b.y, 2));
}