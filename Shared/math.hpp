#pragma once

#include "log.hpp"

struct Vector2
{
    float x, y;

    Vector2 operator+(Vector2 rhs) const
    {
        return Vector2 {x + rhs.x, y + rhs.y};
    }

    Vector2 operator-(Vector2 rhs) const
    {
        return Vector2 {x - rhs.x, y - rhs.y};
    }
    
    Vector2 operator*(Vector2 rhs) const
    {
        return Vector2 {x * rhs.x, y * rhs.y};
    }
    
    Vector2& operator+=(Vector2 rhs)
    {
        x += rhs.x;
        y += rhs.y;
        
        return *this;
    }

    Vector2& operator-=(Vector2 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        
        return *this;
    }

    Vector2 operator*(float scaler) const
    {
        return Vector2 {x * scaler, y * scaler};
    }

    Vector2 operator/(float scaler) const
    {
        return Vector2 {x / scaler, y / scaler};
    }

    Vector2& operator*=(float scaler)
    {
        x *= scaler;
        y *= scaler;
        
        return *this;
    }
    
    Vector2& operator/=(float scaler)
    {
        x /= scaler;
        y /= scaler;
        
        return *this;
    }
};

struct Vector3
{
    float x, y, z;

    Vector3 operator+(Vector3 rhs) const
    {
        return Vector3 {x + rhs.x, y + rhs.y, z + rhs.z};
    }   

    Vector3 operator-(Vector3 rhs) const
    {
        return Vector3 {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vector3 operator*(Vector3 rhs) const
    {
        return Vector3 {x * rhs.x, y * rhs.y, z * rhs.z};
    }

    Vector3& operator+=(Vector3 rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        
        return *this;
    }

    Vector3& operator-=(Vector3 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        
        return *this;
    }

    Vector3 operator*(float scaler) const
    {
        return Vector3 {x * scaler, y * scaler, z * scaler};
    }

    Vector3 operator/(float scaler) const
    {
        return Vector3 {x / scaler, y / scaler, z / scaler};
    }

    Vector3& operator*=(float scaler)
    {
        x *= scaler;
        y *= scaler;
        z *= scaler;
        
        return *this;
    }
    
    Vector3& operator/=(float scaler)
    {
        x /= scaler;
        y /= scaler;
        z /= scaler;
        
        return *this;
    }
};

struct Vector4
{
    float x, y, z, w;

    Vector4 operator+(Vector4 rhs) const
    {
        return Vector4 {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
    }

    Vector4 operator-(Vector4 rhs) const
    {
        return Vector4 {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
    }

    Vector4 operator*(Vector4 rhs) const
    {
        return Vector4 {x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w};
    }

    Vector4& operator+=(Vector4 rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        
        return *this;
    }

    Vector4& operator-=(Vector4 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        
        return *this;
    }

    Vector4 operator*(float scaler) const
    {
        return Vector4 {x * scaler, y * scaler, z * scaler, w * scaler};
    }

    Vector4 operator/(float scaler) const
    {
        return Vector4 {x / scaler, y / scaler, z / scaler, w / scaler};
    }

    Vector4& operator*=(float scaler)
    {
        x *= scaler;
        y *= scaler;
        z *= scaler;
        w *= scaler;
        
        return *this;
    }
    
    Vector4& operator/=(float scaler)
    {
        x /= scaler;
        y /= scaler;
        z /= scaler;
        w /= scaler;
        
        return *this;
    }
};

struct Matrix2x2
{
    float array[2][2];

    float* operator[](size_t index)
    {
        CZ_ASSERT(index < 2, "Index out of bounds");
        return array[index];
    }

    const float* operator[](size_t index) const
    {
        CZ_ASSERT(index < 2, "Index out of bounds");
        return array[index];
    }
};

struct Matrix3x3
{
    float array[3][3];

    float* operator[](size_t index)
    {
        CZ_ASSERT(index < 3, "Index out of bounds");
        return array[index];
    }

    const float* operator[](size_t index) const
    {
        CZ_ASSERT(index < 3, "Index out of bounds");
        return array[index];
    }
};

struct Matrix4x4
{
    float array[4][4];

    float* operator[](size_t index)
    {
        CZ_ASSERT(index < 4, "Index out of bounds");
        return array[index];
    }

    const float* operator[](size_t index) const
    {
        CZ_ASSERT(index < 4, "Index out of bounds");
        return array[index];
    }
};

inline Matrix3x3 matrix3x3_create_identity()
{
	return Matrix3x3{
		.array = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		}
	};
}

inline Matrix4x4 matrix4x4_create_identity()
{
	return Matrix4x4{
		.array = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		}
	};
}