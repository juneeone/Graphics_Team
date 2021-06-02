//#pragma once
//#include "directxmath.h"
//#include <d3d11.h>
//#include <D3DX10math.h>
//
////D3DXVECTOR4 = XMFLOAT4
//
//class SkyDomeClass
//{
//private:
//	typedef struct
//	{
//		float x, y, z;
//		float tu, tv;
//		float nx, ny, nz;
//	} ModelType;
//
//	struct  VertexType
//	{
//		D3DXVECTOR3 position;				//x,y,z position
//	};
//
//public:
//	SkyDomeClass();
//	SkyDomeClass(const SkyDomeClass&);
//	~SkyDomeClass();
//
//	bool Initialize(ID3D11Device*);
//	void Shutdown();
//	void Render(ID3D11DeviceContext*);
//
//	int GetIndexCount();
//	D3DXVECTOR4 GetApexColor();
//	D3DXVECTOR4 GetCenterColor();
//
//private:
//	bool LoadSkyDomeModel(const char*);
//	void ReleaseSkyDomeModel();
//	bool InitializeBuffers(ID3D11Device*);
//	void ReleaseBuffers();
//	void RenderBuffers(ID3D11DeviceContext*);
//
//private:
//	ModelType* m_model = nullptr;
//	int m_vertexCount = 0;
//	int m_indexCount = 0;
//	ID3D11Buffer* m_vertexBuffer = nullptr;
//	ID3D11Buffer* m_indexBuffer = nullptr;
//	D3DXVECTOR4 m_ApexColor;
//	D3DXVECTOR4 m_CenterColor;
//
//};