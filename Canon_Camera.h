#pragma once

#include "xInput.h"

class Canon_Camera
{
public:

	D3DXMATRIX			 m_matView; 
	
public:

	D3DXMATRIX			 m_matProj;

	//카메라 회전

	float m_fCameraYawAngle;
	float m_fCameraPitchAngle;
	float m_fCameraRollAngle;

	// 뷰 행렬
	D3DXVECTOR3   camera_position;   D3DXVECTOR3 camera_look;
	D3DXVECTOR3   fake_up;			 D3DXVECTOR3 camera_up;
	D3DXVECTOR3   Target_at;         D3DXVECTOR3 camera__Right;

	// 카메라 이동
	float m_fSpeed;
	bool Speed_auto_Lessen();

	// 투영 행렬

	float Vertical_Fov_Angle;
	float Aspect_Ratio;//가로세로비
	float Near_Plane_Distance;
	float Far_Plane_Distance;

	void Set_View_Matrix(
		D3DXVECTOR3 camera_position_in = D3DXVECTOR3(-10.0f, -10.0f, -10.0f),
		D3DXVECTOR3 fake_up_in = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3 Target_at_in = D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	bool ViewMatrix_Update_From_Keyboard_Mouse(D3DXVECTOR4 vValue);

	void Set_Projection_Matrix(float Vertical_Fov_Angle_in, float Aspect_Ratio_in, float Near_Plane_Distance_in, float Far_Plane_Distance_in);
	void Move_on_Look_line(float fValue);
	void Move_on_Right_line(float fValue);
	void Move_on_Up_line(float fValue);
	void SetSpeed(float fValue);

	bool camera_Coordinates_System_from_ViewMatrix_and_YawPitch_Roll_from_LookVector();

	void camera_operation();
	Canon_Camera();
};