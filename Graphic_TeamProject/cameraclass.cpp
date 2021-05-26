////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	//m_positionX = 0.0f;
	//m_positionY = 0.0f;
	//m_positionZ = 0.0f;

	//m_rotationX = 0.0f;
	//m_rotationY = 0.0f;
	//m_rotationZ = 0.0f;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


//void CameraClass::SetPosition(float x, float y, float z)
//{
//	m_positionX = x;
//	m_positionY = y;
//	m_positionZ = z;
//	return;
//}
//
//
//void CameraClass::SetRotation(float x, float y, float z)
//{
//	m_rotationX = x;
//	m_rotationY = y;
//	m_rotationZ = z;
//	return;
//}

void CameraClass::SetPosition(D3DXVECTOR3 pos) {
	m_position = pos;
}
void CameraClass::SetRotation(D3DXVECTOR3 rot) {
	m_rotation = rot;
}
D3DXVECTOR3 CameraClass::GetPostion() {
	return m_position;
}
D3DXVECTOR3 CameraClass::GetRotation() {
	return m_rotation;
}

void CameraClass::Render()
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	float radians;
	
	D3DXMATRIX rotationMaxtrix;
	float yaw, pitch, roll;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	//position.x = m_positionX;
	//position.y = m_positionY;
	//position.z = m_positionZ;
	position = m_position;

	// Calculate the rotation in radians.
	//radians = m_rotationY * 0.0174532925f;
	radians = m_rotation.y * 0.0174532925f;

	// Setup where the camera is looking.
	//lookAt.x = sinf(radians) + m_position.x;
	//lookAt.y = m_position.y;
	//lookAt.z = cosf(radians) + m_position.z;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMaxtrix, yaw, pitch, roll);
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMaxtrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMaxtrix);

	D3DXVec3Add(&lookAt, &position, &lookAt);
	
	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void CameraClass::RenderReflection(float height)
{
	D3DXVECTOR3 up, position, lookAt;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	//position.x = m_positionX;
	//position.y = -m_positionY + (height * 2.0f);
	//position.z = m_positionZ;

	position.x = m_position.x;
	position.y = -m_position.y + (height * 2.0f);
	position.z = m_position.z;
	// Calculate the rotation in radians.
	radians = m_rotation.y * 0.0174532925f;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + m_position.x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_position.z;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);

	return;
}


D3DXMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}