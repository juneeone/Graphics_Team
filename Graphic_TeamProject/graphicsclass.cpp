////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


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
	// 2D 사용자 인터페이스 렌더링을 위해 카메라로 기본 뷰 행렬을 초기화합니다.
	D3DXMATRIX baseViewMatrix;
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// 카메라의 초기 위치를 설정합니다.
	D3DXVECTOR3 camera = D3DXVECTOR3(0.0f, 2.0f, -12.0f);
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

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.			Plane(바닥)
	WCHAR stone[] = L"./data/GrassPatch.jpg";	//데이터 타입에 맞게끔 변수 선언 WCHAR
	char plane[] = "./data/Ground.txt";

	result = m_Model->Initialize(m_D3D->GetDevice(), stone, plane);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//CUBE 모델
	m_CubeModel = new ModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	WCHAR Cubetex[] = L"./data/wall.jpg";	//데이터 타입에 맞게끔 변수 선언 WCHAR
	char CUBE[] = "./data/wall.txt";
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), Cubetex, CUBE);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//풀장식 모델
	m_GrassModel = new ModelClass;
	if (!m_GrassModel)
	{
		return false;
	}
	WCHAR Grasstex[] = L"./data/GrassPatch.jpg";	//데이터 타입에 맞게끔 변수 선언 WCHAR
	char GRASS[] = "./data/GrassSeperate.txt";
	result = m_GrassModel->Initialize(m_D3D->GetDevice(), Grasstex, GRASS);
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
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 1.0f, 3.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if(!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 1.0f, 3.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if(!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
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

	//마우스조작으로 회전
	m_Position->TurnLeft(cur.x < centerPT.x);
	m_Position->TurnRight(cur.x > centerPT.x);
	m_Position->LookUpward(cur.y < centerPT.y);
	m_Position->LookDownward(cur.y > centerPT.y);
	//키보드로 이동
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
	D3DXMATRIX	viewMatrix, projectionMatrix;

	D3DXMATRIX PlaneScale, PlaneRot, PlaneMove;				//평면 매트릭스값들 = 여러번 선언하면 동시에 처리가 안되기 때문에 각각 별개로 선언하고 후에 world=planerot*planemove로 처리
	D3DXMATRIX worldMatrix;											//Plane이 들어갈 월드 매트릭스

	//외벽의 매트릭스
	D3DXMATRIX Wall01_Scale, Wall01_Move;
	D3DXMATRIX Wall02_Scale, Wall02_Move, Wall03_Scale, Wall03_Move, Wall04_Scale, Wall04_Move, Wall04_Rot;

	D3DXMATRIX Wall01_Matrix, Wall02_Matrix, Wall03_Matrix, Wall04_Matrix; 

	//미로 내부의 가벽들의 매트릭스(세로)
	D3DXMATRIX Miro01_Scale, Miro01_Move;
	D3DXMATRIX Miro02_Scale, Miro02_Move, Miro03_Scale, Miro03_Move;

	D3DXMATRIX Miro01_Matrix, Miro02_Matrix, Miro03_Matrix;

	//미로 내부의 가벽들의 매트릭스(가로)
	D3DXMATRIX Miro04_Scale, Miro04_Move;
	D3DXMATRIX Miro05_Scale, Miro05_Move, Miro06_Scale, Miro06_Move;

	D3DXMATRIX Miro04_Matrix, Miro05_Matrix, Miro06_Matrix;

	//Grass 매트릭스
	D3DXMATRIX GrassField01_Matrix, GrassField02_Matrix, GrassField01_Scale, GrassField01_Move, GrassField02_Scale, GrassField02_Move;

	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];
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

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(255.0f, 0.0f, 0.0f, 1.0f);				//여기서 배경의 RGB값 조정 가능

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//Cube. 테스트 용으로 중앙에 띄워둔 큐브
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

	//Wall02. 동쪽 외벽
	D3DXMatrixTranslation(&Wall02_Move, 145.5f, -0.5f, 165.5f);				//위치	
	D3DXMatrixScaling(&Wall02_Scale, 10.0f, 20.0f, 225.0f);					//크기

	Wall02_Matrix = Wall02_Scale * Wall02_Move;										//매트릭스 = 크기 * 위치. 정보를 한번에 담기 위함
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall02_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall03. 서쪽 외벽
	D3DXMatrixTranslation(&Wall03_Move, -145.5f, -0.5f, 200.5f);		//위치
	D3DXMatrixScaling(&Wall03_Scale, 10.0f, 20.0f, 300.0f);				//크기

	Wall03_Matrix = Wall03_Scale * Wall03_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall03_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Wall04. 북쪽 외벽
	D3DXMatrixTranslation(&Wall04_Move, 0.0f, -0.5f, 350.5f);			//위치
	D3DXMatrixScaling(&Wall04_Scale, 300.0f, 20.0f, 10.0f);				//크기
	
	Wall04_Matrix = Wall04_Scale * Wall04_Move;									//
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Wall04_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	/* 가벽(세로) */
	//Miro01. 가벽
	D3DXMatrixTranslation(&Miro01_Move, -75.0f, -0.5f, 300.5f);		//위치
	D3DXMatrixScaling(&Miro01_Scale, 10.0f, 20.0f, 100.0f);				//크기

	Miro01_Matrix = Miro01_Scale * Miro01_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro01_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro02. 가벽
	D3DXMatrixTranslation(&Miro02_Move, 0.0f, -0.5f, 150.5f);		//위치
	D3DXMatrixScaling(&Miro02_Scale, 10.0f, 20.0f, 200.0f);				//크기

	Miro02_Matrix = Miro02_Scale * Miro02_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro02_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro03. 가벽
	D3DXMatrixTranslation(&Miro03_Move, 75.0f, -0.5f, 265.5f);		//위치
	D3DXMatrixScaling(&Miro03_Scale, 10.0f, 20.0f, 175.0f);				//크기

	Miro03_Matrix = Miro03_Scale * Miro03_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro03_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	/* 가벽(가로) */
	//Miro04. 가벽
	D3DXMatrixTranslation(&Miro04_Move, -105.0f, -5.5f, 105.5f);		//위치
	D3DXMatrixScaling(&Miro04_Scale, 70.0f, 30.0f, 10.0f);				//크기

	Miro04_Matrix = Miro04_Scale * Miro04_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro04_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro05. 가벽
	D3DXMatrixTranslation(&Miro05_Move, -40.0f, -5.5f, 180.5f);		//위치
	D3DXMatrixScaling(&Miro05_Scale, 72.5f, 30.0f, 10.0f);				//크기

	Miro05_Matrix = Miro05_Scale * Miro05_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro05_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Miro06. 가벽
	D3DXMatrixTranslation(&Miro06_Move, 105.0f, -5.5f, 105.5f);		//위치
	D3DXMatrixScaling(&Miro06_Scale, 70.0f, 30.0f, 10.0f);				//크기

	Miro06_Matrix = Miro06_Scale * Miro06_Move;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro06_Matrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	////Miro06. 가벽. 기준점을 잡기 위한 더미
	//D3DXMatrixTranslation(&Miro06_Move, 75.0f, -5.5f, 255.5f);		//위치
	//D3DXMatrixScaling(&Miro06_Scale, 300.0f, 5.0f, 10.0f);				//크기

	//Miro06_Matrix = Miro06_Scale * Miro06_Move;
	//m_CubeModel->Render(m_D3D->GetDeviceContext());

	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), Miro06_Matrix, viewMatrix, projectionMatrix,
	//	m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	//if (!result)
	//{
	//	return false;
	//}

	//Plane. WorldMatrix=바닥 오브젝트
	D3DXMatrixScaling(&PlaneScale, 6.0f, 6.0f, 6.0f);		//보여지는 오브젝트의 크기를 조절하기 위함인 것.	
	D3DXMatrixRotationX(&PlaneRot, D3DXToRadian(90.0));		//(바꿀 오브젝트의 매트릭스, 회전값을 준 변수)
	D3DXMatrixTranslation(&PlaneMove, 0.0f, -15.0f, 200.0f);

	//worldMatrix = PlaneScale * PlaneRot * PlaneMove; //원본상태

	worldMatrix = PlaneScale * PlaneMove;

	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader and the light arrays.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	/*Grass*/
	////Grass.원본 정가운데 위치
	//D3DXMatrixScaling(&GrassField01_Scale, 2.0f, 0.5f, 2.0f);		//보여지는 오브젝트의 크기를 조절하기 위함인 것.	
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
	D3DXMatrixScaling(&GrassField01_Scale, 2.0f, 0.5f, 0.5f);		//보여지는 오브젝트의 크기를 조절하기 위함인 것.	
	D3DXMatrixTranslation(&GrassField01_Move, -100.0f, -10.0f, 75.0f);
	GrassField01_Matrix = GrassField01_Scale * GrassField01_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField01_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	//Grass02.
	D3DXMatrixScaling(&GrassField02_Scale, 1.0f, 0.5f, 1.0f);		//보여지는 오브젝트의 크기를 조절하기 위함인 것.	
	D3DXMatrixTranslation(&GrassField02_Move, 100.0f, -10.0f, 100.0f);
	GrassField02_Matrix = GrassField02_Scale * GrassField02_Move;

	m_GrassModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GrassModel->GetIndexCount(), GrassField02_Matrix, viewMatrix, projectionMatrix,
		m_GrassModel->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}