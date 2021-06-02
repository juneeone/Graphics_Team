////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "stdafx.h"
#include "inputclass.h"
#include "positionclass.h"
#include "TimerClass.h"

class D3DClass;
class CameraClass;
class TextureShaderClass; //아직 안만듬. Lightshader 써보고 안되면 제작->이제 만듬
class BitmapClass;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND,HINSTANCE);
	void Shutdown();
	bool Frame();

public:
	RECT rect;
	POINT centerPT;
	D3D11_VIEWPORT vp;

	void SetDevice(ID3D11Device* device);
	void SetBackground(std::string path);				//배경이미지를 배치할 함수

	void Draw();
	void onLostDevice();
	void onResetDevicec();

	void Clean();
	

private:
	bool Render();


	bool HandleInput(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_CubeModel;
	ModelClass* m_GrassModel;
	ModelClass* m_GateModel;
	ModelClass* m_SkyBox;
	ModelClass* m_SkyDome;
	ModelClass* m_Lightpole;
	ModelClass* m_Bench;

	/*배경을 위해 추가한 변수들*/
	ID3DX10Sprite* m_Sprite;				//스프라이트 
	
	// ID3D11Texture2D* m_Background;
	ID3D11Texture1D* m_Background;
	ID3D11Device* m_Device;


	LightShaderClass* m_LightShader;
	LightClass *m_Light1, *m_Light2, *m_Light3, *m_Light4;

	/*새로 추가*/	
	//CameraClass* m_Camera = nullptr;
	BitmapClass* m_Bitmap = nullptr;

	TextureShaderClass* m_TextureShader=nullptr;

	InputClass* m_Input;
	PositionClass* m_Position;
	TimerClass* m_Timer;
};

#endif