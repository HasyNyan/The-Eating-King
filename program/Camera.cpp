#include"Camera.h"

Camera::Camera()
	:m_position(VGet(0.0f, 10.0f, -20.0f))
	, m_target(VGet(0.0f, 0.0f, 0.0f))
	, m_fov(60.0f)
	, m_nearZ(0.1f)
	, m_farZ(1000.0f) 
{
}

void Camera::Initialize(VECTOR position, VECTOR target)
{
	m_position = position;
	m_target = target;
}

void Camera::Update() 
{
}

void Camera::SetToDxLib()
{
	//描画範囲の設定
	SetCameraNearFar(m_nearZ, m_farZ);
	//視野角の設定
	SetupCamera_Perspective(TO_RADIAN(m_fov));
	//カメラの視点位置・注視点・上方向ベクトルを設定
	SetCameraPositionAndTarget_UpVecY(m_position, m_target);
}