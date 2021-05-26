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

private:
	bool Render();


	bool HandleInput(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_CubeModel;
	ModelClass* m_GrassModel;
	LightShaderClass* m_LightShader;
	LightClass *m_Light1, *m_Light2, *m_Light3, *m_Light4;


	InputClass* m_Input;
	PositionClass* m_Position;
	TimerClass* m_Timer;
};

#endif