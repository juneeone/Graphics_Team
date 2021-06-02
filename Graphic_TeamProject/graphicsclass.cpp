////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "BitmapClass.h"
#include "textureshaderclass.h"

#include "stdafx.h"
#include "d3dclass.h"
#include "modelclass.h"
#include "cameraclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_CubeModel = 0;
	m_LightShader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;

	m_Position = 0;
	m_Input = 0;
	m_Timer = 0;
	
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

//����� ���� �߰��� �Լ�
void GraphicsClass::SetDevice(ID3D11Device* device)
{
	m_Device = device;
}

void GraphicsClass::SetBackground(std::string path)
{

	//D3DX11CreateTextureFromFileA(m_Device, path.c_str(), &m_Background);
}

//����
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd,HINSTANCE hinstance)
{
	bool result;
	
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	GetClientRect(hwnd, &rect);
	POINT p1, p2;
	p1.x = rect.left;
	p1.y = rect.top;
	p2.x = rect.right;
	p2.y = rect.bottom;
	ClientToScreen(hwnd, &p1);
	ClientToScreen(hwnd, &p2);

	rect.left = p1.x;
	rect.top = p1.y;
	rect.right = p2.x;
	rect.bottom = p2.y;

	centerPT.x = (p1.x + p2.x) / 2;
	centerPT.y = (p1.y + p2.y) / 2;

	SetCursorPos(centerPT.x, centerPT.y);

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.0f;

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}
	// 2D ����� �������̽� �������� ���� ī�޶�� �⺻ �� ����� �ʱ�ȭ�մϴ�.
	D3DXMATRIX baseViewMatrix;
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// ī�޶��� �ʱ� ��ġ�� �����մϴ�.
	D3DXVECTOR3 camera = D3DXVECTOR3(-100.0f, 2.0f, 75.0f); //��ŸƮ ����Ʈ
	//D3DXVECTOR3 camera = D3DXVECTOR3(-0.0f, 2.0f, -15.0f);		//�׽�Ʈ �� ��ǥ
	m_Camera->SetPosition(camera);

	m_Timer = new TimerClass;
	if (!m_Timer) {
		return false;
	}
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}
	m_Position = new PositionClass;
	if (!m_Position) {
		return false;
	}
	m_Position->SetPosition(camera);

	//+)�ؽ��Ľ����� ��ü ����
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Textureshader object.", L"Error", MB_OK);
		return false;
	}

	//+) ��Ʈ�� Ŭ���� �߰�
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	
	WCHAR BACK[] = L"./data/Night.jpg";					//��Ʈ�� ���� �̹�����
	//��Ʈ�� ��ü �ʱ�ȭ
	if (!m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, BACK, 800, 600))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	// �� ������Ʈ�� ����. Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.			Plane(�ٴ�)
	WCHAR stone[] = L"./data/GrassPatch.jpg";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char plane[] = "./data/Ground.txt";

	result = m_Model->Initialize(m_D3D->GetDevice(), stone, plane);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//0. ��ī�� �ڽ� ��(����)
	m_SkyBox = new ModelClass;
	if (!m_SkyBox)
	{
		return false;
	}
	WCHAR Sky[] = L"./data/Night.jpg";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char SKYBOX[] = "./data/wall.txt";
	result = m_SkyBox->Initialize(m_D3D->GetDevice(), Sky, SKYBOX);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//1.CUBE ��
	m_CubeModel = new ModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	WCHAR Cubetex[] = L"./data/GrassPatch.jpg";
	char CUBE[] = "./data/wall.txt";
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), Cubetex, CUBE);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//2.Ǯ��� ��
	m_GrassModel = new ModelClass;
	if (!m_GrassModel)
	{
		return false;
	}
	WCHAR Grasstex[] = L"./data/GrassPatch.jpg";
	char GRASS[] = "./data/GrassSeperate.txt";
	result = m_GrassModel->Initialize(m_D3D->GetDevice(), Grasstex, GRASS);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//3.��ī�̵�(��)
	m_SkyDome = new ModelClass;
	if (!m_SkyDome)
	{
		return false;
	}
	WCHAR Skytex[] = L"./data/Moon.png";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char SKY[] = "./data/skydome.txt";
	result = m_SkyDome->Initialize(m_D3D->GetDevice(), Skytex, SKY);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//4. ���ε� ��
	m_Lightpole = new ModelClass;
	if (!m_Lightpole)
	{
		return false;
	}

	WCHAR LTex[] = L"./data/Lightpole.png";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char LPOLE[] = "./data/Lightpole.txt";
	result = m_Lightpole->Initialize(m_D3D->GetDevice(), LTex, LPOLE);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//5. ��ġ ��
	m_Bench = new ModelClass;
	if (!m_Bench)
	{
		return false;
	}

	WCHAR BenchTex[] = L"./data/Bench.jpg";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char BENCH[] = "./data/Bench.txt";
	result = m_Bench->Initialize(m_D3D->GetDevice(), BenchTex, BENCH);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	//����Ʈ ��
	m_GateModel = new ModelClass;
	if (!m_GateModel)
	{
		return false;
	}

	// Initialize the model object.			Plane(�ٴ�)
	//WCHAR GateTex[] = L"./data/seafloor.dds";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	//char GATE[] = "./data/Gate.txt";				//����
	WCHAR GateTex[] = L"./data/GrassPatch.jpg";
	char GATE[] = "./data/Gate.txt";

	result = m_GateModel->Initialize(m_D3D->GetDevice(), GateTex, GATE);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the first light object.
	m_Light1 = new LightClass;
	if(!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	m_Light1->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 1.0f, 3.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if(!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 1.0f, 3.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if(!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light3->SetPosition(-3.0f, 1.0f, -3.0f);

	// Create the fourth light object.
	m_Light4 = new LightClass;
	if(!m_Light4)
	{
		return false;
	}

	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(3.0f, 1.0f, -3.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	//m_Bitmap ��ü ��ȯ
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light objects.
	if(m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if(m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if(m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if(m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	if (m_Position) {
		delete m_Position;
		m_Position = 0;
	}
	if (m_Timer) {
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}


	if (m_Input) {
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	result = m_Input->Frame();
	if (!result) {
		return false;
	}

	if (m_Input->IsEscapePressed() == true) {
		return false;
	}
	m_Timer->Frame();
	result = HandleInput(m_Timer->GetTime());
	if (!result) {
		return false;
	}

	// Set the position of the camera.
	//m_Camera->SetPosition(0.0f, 2.0f, -12.0f);
	//m_Camera->SetPosition(D3DXVECTOR3(0.0f, 2.0f, -12.0f));


	// Render the scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}
bool GraphicsClass::HandleInput(float frameTime) {
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	POINT cur;
	GetCursorPos(&cur);
	SetCursorPos(centerPT.x, centerPT.y);

	m_Position->SetFrameTime(frameTime);

	//���콺�������� ȸ��
	m_Position->TurnLeft(cur.x < centerPT.x);
	m_Position->TurnRight(cur.x > centerPT.x);
	m_Position->LookUpward(cur.y < centerPT.y);
	m_Position->LookDownward(cur.y > centerPT.y);
	//Ű����� �̵�
	m_Position->MoveForward(m_Input->IsWPressed());
	m_Position->MoveLeft(m_Input->IsAPressed());
	m_Position->MoveRight(m_Input->IsDPressed());
	m_Position->MoveBackward(m_Input->IsSPressed());

	m_Position->GetPosition(pos);
	m_Position->GetRotation(rot);

	m_Camera->SetPosition(pos);
	m_Camera->SetRotation(rot);

	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX	viewMatrix, projectionMatrix,orthoMatrix;

	D3DXMATRIX PlaneScale, PlaneRot, PlaneMove;				//��� ��Ʈ�������� = ������ �����ϸ� ���ÿ� ó���� �ȵǱ� ������ ���� ������ �����ϰ� �Ŀ� world=planerot*planemove�� ó��
	D3DXMATRIX worldMatrix;											//Plane�� �� ���� ��Ʈ����

	//�ܺ��� ��Ʈ����
	D3DXMATRIX Wall01_Scale, Wall01_Move;
	D3DXMATRIX Wall02_Scale, Wall02_Move, Wall03_Scale, Wall03_Move, Wall04_Scale, Wall04_Move, Wall04_Rot;

	D3DXMATRIX Wall01_Matrix, Wall02_Matrix, Wall03_Matrix, Wall04_Matrix; 

	//�̷� ������ �������� ��Ʈ����(����)
	D3DXMATRIX Miro01_Scale, Miro01_Move;
	D3DXMATRIX Miro02_Scale, Miro02_Move, Miro03_Scale, Miro03_Move;

	D3DXMATRIX Miro01_Matrix, Miro02_Matrix, Miro03_Matrix;

	//�̷� ������ �������� ��Ʈ����(����)
	D3DXMATRIX Miro04_Scale, Miro04_Move;
	D3DXMATRIX Miro05_Scale, Miro05_Move, Miro06_Scale, Miro06_Move;

	D3DXMATRIX Miro04_Matrix, Miro05_Matrix, Miro06_Matrix;

	D3DXMATRIX Grim_Matrix, Grim_Rot, Grim_Move;

	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];
	D3DXVECTOR4 Lightp1[4];
	bool result;


	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();

	//�׽�Ʈ ����
	m_Light1->SetPosition(-3.0f, 25.0f,3.0f);
	m_Light2->SetPosition(3.0f, 25.0f, 3.0f);
	m_Light3->SetPosition(-3.0f, 25.0f, -3.0f);
	m_Light4->SetPosition(3.0f, 25.0f, -3.0f);

	Lightp1[0] = m_Light1->GetPosition();
	Lightp1[1] = m_Light2->GetPosition();
	Lightp1[2] = m_Light3->GetPosition();
	Lightp1[3] = m_Light4->GetPosition();

	// Clear the buffers to begin the scene. ���� �׸��� ���� ���۸� �����.
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);												//���⼭ ����� RGB�� ���� ����

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//��� 2D �������� �����ϱ� ���ؼ� Z���۸� �����Ѵ�.
	m_D3D->TurnZBufferOff();

	D3DXMatrixRotationX(&Grim_Rot, (-90.0));
	D3DXMatrixTranslation(&Grim_Move, 0.0f, 35.0f, 15.0f);
	Grim_Matrix = Grim_Move* Grim_Rot;
	//��Ʈ���� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸� �غ�
	if (!m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0))
	{
		return false;
	}

	if (!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), Grim_Matrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	{
		return false;
	}
		
	//��� 2D �������� �Ϸ�Ǿ����� Z���۸� �Ҵ�. ����: ������ �������� �ϸ� ���̴��� ��������. �ݵ�� 2D ���ķ� ��ġ
	m_D3D->TurnZBufferOn();

	D3DXMATRIX Sky_Matrix, Sky_Move, Sky_Scale;

	//Cube. ��ī�� �� ���
	D3DXMatrixTranslation(&Sky_Move, 45.5f, 25.5f, 100.5f);
	D3DXMatrixScaling(&Sky_Scale, 5.5f, 5.5f, 5.5f);

	Sky_Matrix = Sky_Move * Sky_Scale;
	m_SkyDome->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), Sky_Matrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	D3DXMATRIX SB_Matrix, SB_Move, SB_Scale;
	//�ӽ÷� ������
	LightClass* ex_light;
	ex_light = 0;
	ex_light = new LightClass;
	ex_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	ex_light->SetPosition(0.0f, -2.0f, 3.0f);
	D3DXVECTOR4 ex_diffuseColor = ex_light->GetDiffuseColor();
	D3DXVECTOR4 ex_lightPosition = ex_light->GetPosition();
	//��ī�̹ڽ�. �ϴ� ǳ�� �ӽ� ����
	D3DXMatrixScaling(&SB_Scale, 315.0f, 3.0f, 315.0f);
	D3DXMatrixTranslation(&SB_Move, 0.0f, 27.5f, 200.5f);
	SB_Matrix = SB_Scale*SB_Move;
	//worldMatrix2 = HedgeScale * HedgeMove;
	m_SkyBox->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_SkyBox->GetIndexCount(), SB_Matrix, viewMatrix, projectionMatrix,
		m_SkyBox->GetTexture(), &ex_diffuseColor, &ex_lightPosition);

	if (!result)
	{
		return false;
	}

	//Cube. �׽�Ʈ ������ �߾ӿ� ����� ť��
	D3DXMatrixTranslation(&Wall02_Matrix, -3.5f, -0.5f, -0.5f);
	D3DXMatrixScaling(&Wall02_Matrix, 1.0f, 1.0f, 1.0f);

	//worldMatrix2 = HedgeScale * HedgeMove;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall02_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall01. ���� �ܺ�
	D3DXMatrixTranslation(&Wall02_Move, 145.5f, 10.5f, 175.5f);				//��ġ	
	D3DXMatrixScaling(&Wall02_Scale, 10.0f, 40.0f, 250.0f);					//ũ��

	Wall02_Matrix = Wall02_Scale * Wall02_Move;										//��Ʈ���� = ũ�� * ��ġ. ������ �ѹ��� ��� ����
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall02_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall02. ���� �ܺ�
	D3DXMatrixTranslation(&Wall03_Move, -145.5f, 10.5f, 200.5f);		//��ġ
	D3DXMatrixScaling(&Wall03_Scale, 10.0f, 40.0f, 300.0f);				//ũ��

	Wall03_Matrix = Wall03_Scale * Wall03_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall03_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall03. ���� �ܺ�
	D3DXMatrixTranslation(&Wall04_Move, 0.0f, 10.5f, 350.5f);			//��ġ
	D3DXMatrixScaling(&Wall04_Scale, 300.0f, 40.0f, 10.0f);				//ũ��
	
	Wall04_Matrix = Wall04_Scale * Wall04_Move;									//
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall04_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall04. ���� �ܺ�
	D3DXMatrixTranslation(&Wall04_Move, 0.0f, 10.5f, 50.5f);			//��ġ
	D3DXMatrixScaling(&Wall04_Scale, 300.0f, 40.0f, 10.0f);				//ũ��

	Wall04_Matrix = Wall04_Scale * Wall04_Move;									//
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall04_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}



	/* ����(����) */
	//Miro01. ����
	D3DXMatrixTranslation(&Miro01_Move, -75.0f, 10.5f, 300.5f);		//��ġ
	D3DXMatrixScaling(&Miro01_Scale, 10.0f, 40.0f, 100.0f);				//ũ��

	Miro01_Matrix = Miro01_Scale * Miro01_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro01_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro02. ����
	D3DXMatrixTranslation(&Miro02_Move, 0.0f, 10.5f, 150.5f);		//��ġ
	D3DXMatrixScaling(&Miro02_Scale, 10.0f, 40.0f, 200.0f);				//ũ��

	Miro02_Matrix = Miro02_Scale * Miro02_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro02_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro03. ����
	D3DXMatrixTranslation(&Miro03_Move, 75.0f, 10.5f, 265.5f);		//��ġ
	D3DXMatrixScaling(&Miro03_Scale, 10.0f, 40.0f, 175.0f);				//ũ��

	Miro03_Matrix = Miro03_Scale * Miro03_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro03_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	/* ����(����) */
	//Miro04. ����
	D3DXMatrixTranslation(&Miro04_Move, -105.0f, 4.5f, 105.5f);		//��ġ
	D3DXMatrixScaling(&Miro04_Scale, 70.0f, 50.0f, 10.0f);				//ũ��

	Miro04_Matrix = Miro04_Scale * Miro04_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro04_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro05. ����
	D3DXMatrixTranslation(&Miro05_Move, -40.0f, 4.5f, 180.5f);		//��ġ
	D3DXMatrixScaling(&Miro05_Scale, 72.5f, 50.0f, 10.0f);				//ũ��

	Miro05_Matrix = Miro05_Scale * Miro05_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro05_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro06. ����
	D3DXMatrixTranslation(&Miro06_Move, 105.0f, 4.5f, 105.5f);		//��ġ
	D3DXMatrixScaling(&Miro06_Scale, 70.0f, 50.0f, 10.0f);				//ũ��

	Miro06_Matrix = Miro06_Scale * Miro06_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro06_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	////Miro06. ����. �������� ��� ���� ����
	//D3DXMatrixTranslation(&Miro06_Move, 75.0f, -5.5f, 255.5f);		//��ġ
	//D3DXMatrixScaling(&Miro06_Scale, 300.0f, 5.0f, 10.0f);				//ũ��

	//Miro06_Matrix = Miro06_Scale * Miro06_Move;
	//m_CubeModel->Render(m_D3D->GetDeviceContext());

	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro06_Matrix, viewMatrix, projectionMatrix,
	//	m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	//if (!result)
	//{
	//	return false;
	//}

	//������
	ex_light->SetPosition(1.0f, 0.0f, 0.0f);
	ex_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	ex_lightPosition = ex_light->GetPosition();
	ex_diffuseColor = ex_light->GetDiffuseColor();
	//������
	//Plane. WorldMatrix=�ٴ� ������Ʈ
	D3DXMatrixScaling(&PlaneScale, 6.0f, 6.0f, 6.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixRotationX(&PlaneRot, D3DXToRadian(90.0));		//(�ٲ� ������Ʈ�� ��Ʈ����, ȸ������ �� ����)
	D3DXMatrixTranslation(&PlaneMove, 0.0f, -15.0f, 200.0f);

	//worldMatrix = PlaneScale * PlaneRot * PlaneMove; //��������

	worldMatrix = PlaneScale * PlaneMove;
	
	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader and the light arrays.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), &ex_diffuseColor, &ex_lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass ��Ʈ����
	D3DXMATRIX GrassField01_Matrix, GrassField02_Matrix, GrassField01_Scale, GrassField01_Move, GrassField02_Scale, GrassField02_Move;
	D3DXMATRIX GrassField03_Matrix, GrassField04_Matrix, GrassField03_Scale, GrassField03_Move, GrassField04_Scale, GrassField04_Move;
	D3DXMATRIX GrassField05_Matrix, GrassField06_Matrix, GrassField05_Scale, GrassField05_Move, GrassField06_Scale, GrassField06_Move;
	D3DXMATRIX GrassField07_Matrix, GrassField08_Matrix, GrassField07_Scale, GrassField07_Move, GrassField08_Scale, GrassField08_Move;

	/*Grass*/
	////Grass.���� ����� ��ġ
	//D3DXMatrixScaling(&GrassField01_Scale, 2.0f, 0.5f, 2.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	//D3DXMatrixTranslation(&GrassField01_Move, 0.0f, -10.0f, 200.0f);
	//GrassField01_Matrix = GrassField01_Scale * GrassField01_Move;

	//m_GrassModel->Render(m_D3D->GetDeviceContext());

	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField01_Matrix, viewMatrix, projectionMatrix,
	//	m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	//if (!result)
	//{
	//	return false;
	//}

	//Grass01.
	D3DXMatrixScaling(&GrassField01_Scale, 3.0f, 0.5f, 0.3f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField01_Move, -80.0f, -10.0f, 57.5f);
	GrassField01_Matrix = GrassField01_Scale * GrassField01_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField01_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass02.
	D3DXMatrixScaling(&GrassField02_Scale, 3.0f, 0.5f, 0.3f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField02_Move, 80.0f, -10.0f, 57.5f);
	GrassField02_Matrix = GrassField02_Scale * GrassField02_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField02_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass03.
	D3DXMatrixScaling(&GrassField03_Scale, 3.0f, 0.5f, 0.3f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField03_Move, 80.0f, -10.0f, 337.5f);
	GrassField03_Matrix = GrassField03_Scale * GrassField03_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField03_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass04.
	D3DXMatrixScaling(&GrassField04_Scale, 3.0f, 0.5f, 0.3f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField04_Move, -80.0f, -10.0f, 337.5f);
	GrassField04_Matrix = GrassField04_Scale * GrassField04_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField04_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass05.
	D3DXMatrixScaling(&GrassField05_Scale, 2.0f, 0.5f, 0.9f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField05_Move, -100.0f, -10.0f, 105.5f);
	GrassField05_Matrix = GrassField05_Scale * GrassField05_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField05_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass06.
	D3DXMatrixScaling(&GrassField06_Scale, 2.0f, 0.5f, 0.9f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField06_Move, 100.0f, -10.0f, 105.5f);
	GrassField06_Matrix = GrassField06_Scale * GrassField06_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField06_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass07.
	D3DXMatrixScaling(&GrassField07_Scale, 2.0f, 0.5f, 0.9f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField07_Move, -50.0f, -10.0f, 180.5f);
	GrassField07_Matrix = GrassField07_Scale * GrassField07_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField07_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass08.
	D3DXMatrixScaling(&GrassField07_Scale, 0.9f, 0.5f, 2.5f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField07_Move, -75.0f, -10.0f, 300.5f);
	GrassField07_Matrix = GrassField07_Scale * GrassField07_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField07_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass09.
	D3DXMatrixScaling(&GrassField07_Scale, 0.9f, 0.5f, 4.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField07_Move, 0.0f, -10.0f, 165.5f);
	GrassField07_Matrix = GrassField07_Scale * GrassField07_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField07_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass10.
	D3DXMatrixScaling(&GrassField07_Scale, 0.9f, 0.5f, 3.5f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField07_Move, 75.0f, -10.0f, 250.5f);
	GrassField07_Matrix = GrassField07_Scale * GrassField07_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField07_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass11.
	D3DXMatrixScaling(&GrassField01_Scale, 0.3f, 0.5f, 3.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField01_Move, -135.0f, -10.0f, 125.5f);
	GrassField01_Matrix = GrassField01_Scale * GrassField01_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField01_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass12.
	D3DXMatrixScaling(&GrassField02_Scale, 0.3f, 0.5f, 3.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField02_Move, 135.0f, -10.0f, 125.5f);
	GrassField02_Matrix = GrassField02_Scale * GrassField02_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField02_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass13.
	D3DXMatrixScaling(&GrassField01_Scale, 0.3f, 0.5f, 3.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField01_Move, -135.0f, -10.0f, 250.5f);
	GrassField01_Matrix = GrassField01_Scale * GrassField01_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField01_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass14.
	D3DXMatrixScaling(&GrassField02_Scale, 0.3f, 0.5f, 3.0f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixTranslation(&GrassField02_Move, 135.0f, -10.0f, 225.5f);
	GrassField02_Matrix = GrassField02_Scale * GrassField02_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField02_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//����Ʈ ��ġ��
	D3DXMATRIX Gate01_Matrix, Gate01_Scale, Gate01_Move, Gate_Rot;

	D3DXMatrixTranslation(&Gate01_Move, -300.5f, -6.5f, 150.5f);
	D3DXMatrixScaling(&Gate01_Scale, 0.3f, 0.3f, 0.2f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(90));

	Gate01_Matrix = Gate01_Scale * Gate01_Move*Gate_Rot;
	m_GateModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GateModel->GetIndexCount(), Gate01_Matrix, viewMatrix, projectionMatrix,
		m_GateModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	D3DXMatrixTranslation(&Gate01_Move, -325.5f, -6.5f, 150.5f);
	D3DXMatrixScaling(&Gate01_Scale, 0.3f, 0.3f, 0.2f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(90));

	Gate01_Matrix = Gate01_Scale * Gate01_Move * Gate_Rot;
	m_GateModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GateModel->GetIndexCount(), Gate01_Matrix, viewMatrix, projectionMatrix,
		m_GateModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//��ġ ��ġ
	D3DXMATRIX Bench_Matrix,Bench_Move,Bench_Scale;

	D3DXMatrixTranslation(&Bench_Move, -65.0f, -17.0f, 325.0f);
	D3DXMatrixScaling(&Bench_Scale, 15.0f, 15.0f, 15.0f);
	Bench_Matrix = Bench_Move + Bench_Scale;

	m_Bench->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bench->GetIndexCount(), Bench_Matrix, viewMatrix, projectionMatrix,
		m_Bench->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	D3DXMATRIX Bench02_Matrix, Bench03_Matrix;

	D3DXMatrixTranslation(&Bench_Move, -150.5f, -17.0f, 495.0f);
	D3DXMatrixScaling(&Bench_Scale, 15.0f, 15.0f, 15.0f);
	Bench02_Matrix = Bench_Move + Bench_Scale;

	m_Bench->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bench->GetIndexCount(), Bench02_Matrix, viewMatrix, projectionMatrix,
		m_Bench->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	D3DXMatrixTranslation(&Bench_Move, -75.0f, -17.0f, -125.0f);
	D3DXMatrixScaling(&Bench_Scale, 15.0f, 15.0f, 15.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(180));
	Bench_Matrix = (Bench_Move + Bench_Scale)*Gate_Rot;

	m_Bench->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bench->GetIndexCount(), Bench_Matrix, viewMatrix, projectionMatrix,
		m_Bench->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}



	//���ε� ��ġ
	D3DXMATRIX LightPole_Matrix, LightPole_Move, LightPole_Scale;

	//1��°(�ϼ��� ������)
	D3DXMatrixTranslation(&LightPole_Move, -12.0f, -1.0f, 34.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	LightPole_Matrix = LightPole_Move * LightPole_Scale;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//�ȳ���
	D3DXMatrixTranslation(&LightPole_Move, 8.0f, -1.0f, -10.0f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(180));
	LightPole_Matrix = LightPole_Move * LightPole_Scale*Gate_Rot;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}


	//2��°(�߾�)
	D3DXMatrixTranslation(&LightPole_Move, 1.75f, -1.0f, -17.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(180));
	LightPole_Matrix = LightPole_Move * LightPole_Scale* Gate_Rot;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//2��°(�߾� ����)
	D3DXMatrixTranslation(&LightPole_Move, 1.75f, -1.0f, -22.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(180));
	LightPole_Matrix = LightPole_Move * LightPole_Scale * Gate_Rot;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//3��°
	D3DXMatrixTranslation(&LightPole_Move, -5.5f, -1.0f, -36.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(180));
	LightPole_Matrix = LightPole_Move * LightPole_Scale * Gate_Rot;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//3~4�� ������ �߾�
	D3DXMatrixTranslation(&LightPole_Move, 1.5f, -1.0f, 20.0f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	LightPole_Matrix = LightPole_Move * LightPole_Scale;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}
	
	//�������� ���ε�
	D3DXMatrixTranslation(&LightPole_Move, 8.0f, -1.0f, 34.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	LightPole_Matrix = LightPole_Move * LightPole_Scale;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//4��°
	D3DXMatrixTranslation(&LightPole_Move, -8.5f, -1.0f, 12.5f);
	D3DXMatrixScaling(&LightPole_Scale, 11.0f, 11.0f, 9.0f);
	D3DXMatrixRotationY(&Gate_Rot, D3DXToRadian(90));
	LightPole_Matrix = LightPole_Move * LightPole_Scale*Gate_Rot;

	m_Lightpole->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Lightpole->GetIndexCount(), LightPole_Matrix, viewMatrix, projectionMatrix,
		m_Lightpole->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}


	//������ Ȯ���� ���� ����
	D3DXMatrixTranslation(&Gate01_Move, 0.5f, 2.5f, -0.5f);
	D3DXMatrixScaling(&Gate01_Scale, 3.0f, 3.0f, 3.0f);
	D3DXMatrixRotationX(&Gate_Rot, D3DXToRadian(90));
	Gate01_Matrix = Gate01_Move* Gate01_Scale * Gate01_Move*Gate_Rot;
	m_GateModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GateModel->GetIndexCount(), Gate01_Matrix, viewMatrix, projectionMatrix,
		m_GateModel->GetTexture(), diffuseColor, Lightp1);

	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}