#ifndef _CAMERA_H
#define _CAMERA_H


class CCamera {

public:
	CCamera();	

	CVector3 Position() {	return m_vPosition;		}
	CVector3 View()		{	return m_vView;			}
	CVector3 UpVector() {	return m_vUpVector;		}
	CVector3 Strafe()	{	return m_vStrafe;		}
	
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	void RotateView(float angle, float X, float Y, float Z);

	void SetViewByMouse(); 

	void SetPos(float x, float y, float z, float angle);

	void RotateAroundPoint(CVector3 vCenter, float X, float Y, float Z);
    
	//w lewo/prawo
	void StrafeCamera(float speed);

    //w przod/tyl
	void MoveCamera(float speed);

	void CheckForMovement();

	void Update();

	void Look();

private:
	CVector3 m_vPosition;					
	CVector3 m_vView;						
	CVector3 m_vUpVector;		
	CVector3 m_vStrafe;						
};

#endif
