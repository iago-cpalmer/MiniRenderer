#include "Vector3f.h"
Vector3f::Vector3f() {

}
Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3f::DotProduct(Vector3f b) {
	return x*b.x + y*b.y + z*b.z;
}
float Vector3f::Magnitude() {
	return sqrt(x*x+y*y+z*z);
}

void Vector3f::Normalize() {
	float m = Magnitude();
	this->x = this->x/m;
	this->y=this->y/m;
	this->z = this->z/m;
}

Vector3f Vector3f::Cross(Vector3f b) {
	return {y*b.z-z*b.y,x*b.z-b.x*z, x*b.y-y*b.x};
}

float Vector3f::Distance(Vector3f b) {
	return sqrt(pow(x-b.x, 2) + pow(y-b.y, 2) + pow(z-b.z, 2));
}