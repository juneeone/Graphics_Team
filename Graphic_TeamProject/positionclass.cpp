#include "stdafx.h"
#include "positionclass.h"


PositionClass::PositionClass()
{
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(D3DXVECTOR3 pos)
{
	m_position = pos;
}


void PositionClass::SetRotation(D3DXVECTOR3 rot)
{
	m_rotation = rot;
}


void PositionClass::GetPosition(D3DXVECTOR3& p)
{
	p = m_position;
}


void PositionClass::GetRotation(D3DXVECTOR3& p)
{
	p = m_rotation;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
}


void PositionClass::MoveForward(bool keydown)
{
	// 키를 누르면 카메라가 왼쪽으로 돌아는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if (m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// 값을 라디안으로 변환하십시오.
	float radians = m_rotation.y * 0.0174532925f;

	// 위치를 업데이트 합니다.
	m_position.x += sinf(radians) * m_forwardSpeed;
	m_position.z += cosf(radians) * m_forwardSpeed;
}

void PositionClass::MoveLeft(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 하향 이동 속도를 갱신합니다.
	if (keydown)
	{
		m_moveleftSpeed += m_frameTime * 0.003f;

		if (m_moveleftSpeed > (m_frameTime * 0.03f))
		{
			m_moveleftSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moveleftSpeed -= m_frameTime * 0.002f;

		if (m_moveleftSpeed < 0.0f)
		{
			m_moveleftSpeed = 0.0f;
		}
	}

	float radians = m_rotation.y * 0.0174532925f;
	// 높이 위치를 업데이트 합니다.
	m_position.x -= cosf(radians) * m_moveleftSpeed;
	m_position.z += sinf(radians) * m_moveleftSpeed;
}

void PositionClass::MoveRight(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 하향 이동 속도를 갱신합니다.
	if (keydown)
	{
		m_moverightSpeed += m_frameTime * 0.003f;

		if (m_moverightSpeed > (m_frameTime * 0.03f))
		{
			m_moverightSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moverightSpeed -= m_frameTime * 0.002f;

		if (m_moverightSpeed < 0.0f)
		{
			m_moverightSpeed = 0.0f;
		}
	}

	float radians = m_rotation.y * 0.0174532925f;

	m_position.x += cosf(radians) * m_moverightSpeed;
	m_position.z -= sinf(radians) * m_moverightSpeed;
}

void PositionClass::MoveBackward(bool keydown)
{
	// 키를 누르면 카메라가 오른쪽으로 회전하는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if (m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// 값을 라디안으로 변환하십시오.
	float radians = m_rotation.y * 0.0174532925f;

	// 위치를 업데이트 합니다.
	m_position.x -= sinf(radians) * m_backwardSpeed;
	m_position.z -= cosf(radians) * m_backwardSpeed;
}


void PositionClass::MoveUpward(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 상향 이동 속도를 갱신합니다.
	if (keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if (m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// 높이 위치를 업데이트 합니다.
	m_position.y += m_upwardSpeed;
}


void PositionClass::MoveDownward(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 하향 이동 속도를 갱신합니다.
	if (keydown)
	{
		m_downwardSpeed += m_frameTime * 0.01f;

		if (m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// 높이 위치를 업데이트 합니다.
	m_position.y -= m_downwardSpeed;
}


void PositionClass::TurnLeft(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 왼쪽 회전 속도를 갱신합니다.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.10f))
		{
			m_leftTurnSpeed = m_frameTime * 0.10f;
		}
	}
	else
	{
		//m_leftTurnSpeed -= m_frameTime * 0.005f;

		//if (m_leftTurnSpeed < 0.0f)
		//{
		//	m_leftTurnSpeed = 0.0f;
		//}
		m_leftTurnSpeed = 0.0f;
	}

	// 회전을 업데이트 합니다.
	m_rotation.y -= m_leftTurnSpeed;

	// 회전값의 범위를 0 ~360 에서 유지합니다.
	if (m_rotation.y < 0.0f)
	{
		m_rotation.y += 360.0f;
	}
}


void PositionClass::TurnRight(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 오른쪽 회전 속도를 갱신합니다.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.10f))
		{
			m_rightTurnSpeed = m_frameTime * 0.10f;
		}
	}
	else
	{
		//m_rightTurnSpeed -= m_frameTime * 0.005f;

		//if (m_rightTurnSpeed < 0.0f)
		//{
		//	m_rightTurnSpeed = 0.0f;
		//}
		m_rightTurnSpeed = 0.0f;
	}

	// 회전을 업데이트 합니다.
	m_rotation.y += m_rightTurnSpeed;

	// 회전값의 범위를 0 ~360 에서 유지합니다.
	if (m_rotation.y > 360.0f)
	{
		m_rotation.y -= 360.0f;
	}
}


void PositionClass::LookUpward(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 위쪽 회전 속도를 갱신합니다.
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if (m_lookUpSpeed > (m_frameTime * 0.10f))
		{
			m_lookUpSpeed = m_frameTime * 0.10f;
		}
	}
	else
	{
		//m_lookUpSpeed -= m_frameTime * 0.005f;

		//if (m_lookUpSpeed < 0.0f)
		//{
		//	m_lookUpSpeed = 0.0f;
		//}

		m_lookUpSpeed = 0.0f;
	}

	// 회전을 업데이트 합니다.
	m_rotation.x -= m_lookUpSpeed;

	// 회전은 최대 90 도로 유지 합니다.
	if (m_rotation.x > 90.0f)
	{
		m_rotation.x = 90.0f;
	}
}


void PositionClass::LookDownward(bool keydown)
{
	// 사용자가 키를 누르고 있는 프레임 시간에 따라 아래쪽 회전 속도를 갱신합니다.
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if (m_lookDownSpeed > (m_frameTime * 0.10f))
		{
			m_lookDownSpeed = m_frameTime * 0.10f;
		}
	}
	else
	{
		//m_lookDownSpeed -= m_frameTime * 0.005f;

		//if (m_lookDownSpeed < 0.0f)
		//{
		//	m_lookDownSpeed = 0.0f;
		//}

		m_lookDownSpeed = 0.0f;
	}

	// 회전을 업데이트 합니다.
	m_rotation.x += m_lookDownSpeed;

	//회전은 최대 90 도로 유지 합니다.
	if (m_rotation.x < -90.0f)
	{
		m_rotation.x = -90.0f;
	}
}