#pragma once

#include <Math/wcgeMath.hpp>

namespace wcge {
	namespace c3d {
		struct Vertex {
			Math::Vector4 position;
			Math::Vector3 normal;
			Math::Vector2 uv;

			inline Vertex( 
				const Math::Vector4& position = Math::Vector4(),
				const Math::Vector3& normal = Math::Vector3(),
				const Math::Vector2& uv = Math::Vector2()
			) : position( position ), normal( normal ), uv( uv ) {}

			inline Vertex operator+ ( const Vertex& rhs ) const { return wcge::c3d::Vertex( this->position + rhs.position, this->normal + rhs.normal, this->uv + rhs.uv ); }
			inline Vertex operator- ( const Vertex& rhs ) const { return wcge::c3d::Vertex( this->position - rhs.position, this->normal - rhs.normal, this->uv - rhs.uv ); }
			inline Vertex operator* ( const float rhs ) const { return wcge::c3d::Vertex( this->position * rhs, this->normal * rhs, this->uv * rhs ); }
			inline Vertex operator/ ( const float rhs ) const { return wcge::c3d::Vertex( this->position / rhs, this->normal / rhs, this->uv / rhs ); }

			Vertex& operator+= ( const Vertex& rhs );
			Vertex& operator-= ( const Vertex& rhs );
			Vertex& operator*= ( const float rhs );
			Vertex& operator/= ( const float rhs );
		};
	}
}