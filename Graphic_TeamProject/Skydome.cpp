//#include "stdafx.h"
//#include "Skydome.h"
//
//#include<fstream>
//using namespace std;
//
//SkyDomeClass::SkyDomeClass()
//{
//}
//
//SkyDomeClass::SkyDomeClass(const SkyDomeClass& other)
//{
//}
//
//SkyDomeClass::~SkyDomeClass()
//{
//}
//
//bool SkyDomeClass::Initialize(ID3D11Device* device)
//{
//	//스카이돔 모델 정보를 불러오는 단계
//	if (!LoadSkyDomeModel("./date/skydome.txt"))
//	{
//		return false;
//	}
//
//	//정점에 로드하고 인덱스 버퍼를 로드
//	if (!InitializeBuffers(device))
//	{
//		return false;
//	}
//
//	//스카이 돔 꼭대기 색상을 설정
//	m_ApexColor = D3DXVECTOR4(0.0f, 0.15f, 0.66f, 1.0f);
//
//	//중심 색상을 설정
//	m_CenterColor = D3DXVECTOR4(0.81f, 0.38f, 0.66f, 1.0f);
//
//	return true;
//}
//
//void SkyDomeClass::Shutdown()
//{	//정점 및 인덱스 버퍼를 해제
//	ReleaseBuffers();
//
//	//
//	ReleaseSkyDomeModel();
//}
//
//void SkyDomeClass::Render(ID3D11DeviceContext* deviceContext)
//{	//스카이 돔을 렌더링
//	RenderBuffers(deviceContext);
//}
//
//int SkyDomeClass :: GetIndexCount()
//{
//	return m_indexCount;
//}
//
//D3DXVECTOR4 SkyDomeClass::GetApexColor()
//{
//	return m_ApexColor;
//}
//
//D3DXVECTOR4 SkyDomeClass::GetCenterColor()
//{
//	return m_CenterColor;
//}
//
//bool SkyDomeClass::LoadSkyDomeModel(const char* filename)
//{
//	ifstream fin;
//	fin.open(filename);
//
//	if (fin.fail())
//	{
//		return false;
//	}
//
//	char input = 0;
//	fin.get(input);
//	while (input != ':')
//	{
//		fin.get(input);
//	}
//
//	fin >> m_vertexCount;
//
//	m_indexCount = m_vertexCount;
//
//	m_model = new ModelType[m_vertexCount];
//	if (!m_model)
//	{
//		return false;
//	}
//
//	fin.get(input);
//	while (input != ':')
//	{
//		fin.get(input);
//	}
//	fin.get(input);
//	fin.get(input);
//
//	for (int i = 0; i < m_vertexCount; i++)
//	{
//		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
//		fin >> m_model[i].tu >> m_model[i].tv;
//		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
//	}
//
//	fin.close();
//
//	return true;
//}
//
//void SkyDomeClass::ReleaseSkyDomeModel()
//{
//	if (m_model)
//	{
//		delete[] m_model;
//		m_model = 0;
//	}
//}
//
//bool SkyDomeClass::InitializeBuffers(ID3D11Device* device)
//{
//	VertexType* vertices = new VertexType[m_vertexCount];
//	if (!vertices)
//	{
//		return false;
//	}
//
//	unsigned long* indices = new unsigned long[m_indexCount];
//	if (!indices)
//	{
//		return false;
//	}
//
//	for (int i = 0; i < m_vertexCount; i++)
//	{
//		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
//		indices[i] = i;
//	}
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = vertices;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
//	{
//		return false;
//	}
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//}