#include "stdafx.h"
#include "textureclass.h"
#include "BitmapClass.h"

BitmapClass::BitmapClass()
{
}

BitmapClass::BitmapClass(const BitmapClass& other) 
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, int ScreenWidth, int ScreenHeight, WCHAR* TextureFilename, int bitmapWidth, int bitmapHeight)
{
	//화면 크기를 변수에 저장 
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;

	//렌더링할 비트맵의 픽셀의 크기를 저장
	m_bitmapHeight = bitmapHeight;
	m_bitmapWidth = bitmapWidth;

	//이전 렌더링위치를 음수로 초기화
	m_previousPosX = -1;
	m_previousPosY = -1;

	//정점 및 인덱스 버퍼를 초기화합니다
	if (!InitializeBuffers(device))
	{
		return false;
	}

	//이 모델의 텍스쳐를 로드합니다.
	return LoadTexture(device, TextureFilename);
}

void BitmapClass::Shutdown()
{
	//모델 텍스쳐를 반환한다
	ReleaseTexture();

	//버텍스 및 인덱스 버퍼를 종료한다
	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	//그리기 준비
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	m_IndexCount = m_VertexCount = 6;

	VertexType* vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_VertexCount));

	unsigned long* indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		return false;
	}

	//데이터로 인덱스 배열을 로드한다
	for (int i = 0; i < m_IndexCount; i++)
	{
		indices[i] = i;
	}

	//정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//서브 리소스 구조에 정점 데이터에 대한 포인터를 제공한다
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//정점 버퍼를 만든다
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer))) 
	{
		return false;
	}

	//정적 인덱스 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//인덱스데이터를 가르키는 보조 리소스 구조체 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//인덱스 버퍼를 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
	{
		return false;
	}

	//생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	//인덱스 버퍼를 해제한다
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}

}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	//위치가 변경된 경우 렌더링 되는 위치를 업데이트 한다.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//비트 맵 왼쪽, 오른쪽, 상단, 하단의 화면 좌표를 계산한다.
	left = (float)((m_ScreenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_ScreenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	//정점 배열을 만듭니다.
	vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	//정점 배열에 데이터를 로드합니다
	//첫번째 삼각형
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//좌상단
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);	//우하단
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//좌하단
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//두번째 삼각형
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);	//좌상단
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);	//우상단
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);	//우하단
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//버텍스 버퍼를 쓸 수 있도록 잠근다.
	result = deviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//정점 버퍼의 데이터를 가르키는 포인터를 얻는다
	verticesPtr = (VertexType*)mappedResource.pData;

	//데이터를 정점 버퍼에 복사합니다
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_VertexCount));

	deviceContext->Unmap(m_VertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//정점 버퍼의 단위와 오프셋 설정
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	//새로운 텍스쳐 오브젝트를 생성한다.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//텍스쳐 오브젝트를 초기화
	return m_Texture->Initialize(device, filename);

}

void BitmapClass::ReleaseTexture()
{
	//텍스쳐 오브젝트를 릴리즈한다
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}