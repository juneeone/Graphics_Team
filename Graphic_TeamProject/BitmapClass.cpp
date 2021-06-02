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
	//ȭ�� ũ�⸦ ������ ���� 
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;

	//�������� ��Ʈ���� �ȼ��� ũ�⸦ ����
	m_bitmapHeight = bitmapHeight;
	m_bitmapWidth = bitmapWidth;

	//���� ��������ġ�� ������ �ʱ�ȭ
	m_previousPosX = -1;
	m_previousPosY = -1;

	//���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�
	if (!InitializeBuffers(device))
	{
		return false;
	}

	//�� ���� �ؽ��ĸ� �ε��մϴ�.
	return LoadTexture(device, TextureFilename);
}

void BitmapClass::Shutdown()
{
	//�� �ؽ��ĸ� ��ȯ�Ѵ�
	ReleaseTexture();

	//���ؽ� �� �ε��� ���۸� �����Ѵ�
	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	//�׸��� �غ�
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

	//�����ͷ� �ε��� �迭�� �ε��Ѵ�
	for (int i = 0; i < m_IndexCount; i++)
	{
		indices[i] = i;
	}

	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//���� ���ҽ� ������ ���� �����Ϳ� ���� �����͸� �����Ѵ�
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//���� ���۸� �����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer))) 
	{
		return false;
	}

	//���� �ε��� ������ ����ü�� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//�ε��������͸� ����Ű�� ���� ���ҽ� ����ü �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//�ε��� ���۸� ����
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
	{
		return false;
	}

	//�����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� ����
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	//�ε��� ���۸� �����Ѵ�
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

	//��ġ�� ����� ��� ������ �Ǵ� ��ġ�� ������Ʈ �Ѵ�.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//��Ʈ �� ����, ������, ���, �ϴ��� ȭ�� ��ǥ�� ����Ѵ�.
	left = (float)((m_ScreenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_ScreenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	//���� �迭�� ����ϴ�.
	vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	//���� �迭�� �����͸� �ε��մϴ�
	//ù��° �ﰢ��
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//�»��
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);	//���ϴ�
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//���ϴ�
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//�ι�° �ﰢ��
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);	//�»��
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);	//����
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);	//���ϴ�
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//���ؽ� ���۸� �� �� �ֵ��� ��ٴ�.
	result = deviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//���� ������ �����͸� ����Ű�� �����͸� ��´�
	verticesPtr = (VertexType*)mappedResource.pData;

	//�����͸� ���� ���ۿ� �����մϴ�
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_VertexCount));

	deviceContext->Unmap(m_VertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//���� ������ ������ ������ ����
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	//���ο� �ؽ��� ������Ʈ�� �����Ѵ�.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//�ؽ��� ������Ʈ�� �ʱ�ȭ
	return m_Texture->Initialize(device, filename);

}

void BitmapClass::ReleaseTexture()
{
	//�ؽ��� ������Ʈ�� �������Ѵ�
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}