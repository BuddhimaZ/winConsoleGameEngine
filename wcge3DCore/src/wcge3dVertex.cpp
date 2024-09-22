#include <wcge3dVertex.hpp>

wcge::c3d::Vertex& wcge::c3d::Vertex::operator+= ( const wcge::c3d::Vertex& rhs ) {
	this->position += rhs.position;
	this->normal += rhs.normal;
	this->uv += rhs.uv;
	return *this;
}

wcge::c3d::Vertex& wcge::c3d::Vertex::operator-= ( const wcge::c3d::Vertex& rhs ) {
	this->position -= rhs.position;
	this->normal -= rhs.normal;
	this->uv -= rhs.uv;
	return *this;
}

wcge::c3d::Vertex& wcge::c3d::Vertex::operator*= ( const float rhs ) {
	this->position *= rhs;
	this->normal *= rhs;
	this->uv *= rhs;
	return *this;
}

wcge::c3d::Vertex& wcge::c3d::Vertex::operator/= ( const float rhs ) {
	this->position /= rhs;
	this->normal /= rhs;
	this->uv /= rhs;
	return *this;
}