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

	void Initialize(VECTOR position, VECTOR target);
	void Update();
	void SetToDxLib();

	void SetPosition(VECTOR pos)	{ m_position = pos; }
	void SetTarget(VECTOR target)   { m_target = target; }
	VECTOR GetPosition() const { return m_position; }
	VECTOR GetTarget() const { return m_target; }
};