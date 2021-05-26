#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);

	void GetPosition(D3DXVECTOR3&);
	void GetRotation(D3DXVECTOR3&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);


public:
	void MoveLeft(bool);
	void MoveRight(bool);

private:
	D3DXVECTOR3 m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float m_frameTime = 0;

	float m_forwardSpeed = 0;
	float m_backwardSpeed = 0;
	float m_upwardSpeed = 0;
	float m_downwardSpeed = 0;
	float m_leftTurnSpeed = 0;
	float m_rightTurnSpeed = 0;
	float m_lookUpSpeed = 0;
	float m_lookDownSpeed = 0;


	float m_moveleftSpeed = 0;
	float m_moverightSpeed = 0;
};