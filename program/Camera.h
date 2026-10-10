#pragma once
class Camera
{
private:
	VECTOR m_position;	//カメラの位置
	VECTOR m_target;	//カメラが注視する座標

	float m_fov;		//視野角
	float m_nearZ;		//描画開始距離
	float m_farZ;		//描画終了距離

public:
	Camera();
	~Camera() = default;


	
};