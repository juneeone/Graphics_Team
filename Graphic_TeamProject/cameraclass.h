////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	//void SetPosition(float, float, float);
	//void SetRotation(float, float, float);

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void RenderReflection(float);
	D3DXMATRIX GetReflectionViewMatrix();

private:
	D3DXMATRIX m_viewMatrix;
	//float m_positionX, m_positionY, m_positionZ;
	//float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_reflectionViewMatrix;

	////////////////////////////////////
public:
	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);
	D3DXVECTOR3 GetPostion();
	D3DXVECTOR3 GetRotation();
private:
	D3DXVECTOR3 m_position, m_rotation;
};

#endif