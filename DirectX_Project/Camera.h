#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
private:
	D3DXVECTOR3 position;
	int startX = 0;
	int startY = 0;
	bool movingXZ = 0;
	bool movingXY = 0;

public:
	Camera(D3DXVECTOR3 pos);
	~Camera();

	D3DXVECTOR3 GetPosition() {
		return position;
	}
	void moveUp(float delta) {
		position.z += delta;
	}
	void moveDown(float delta) {
		position.z -= delta;
	}
	void moveLeft(float delta) {
		position.x += delta;
	}
	void moveRight(float delta) {
		position.x -= delta;
	}
	void moveForward(float delta) {
		position.y += delta;
	}
	void moveBackward(float delta) {
		position.y -= delta;
	}
	void StartMoveXZ(int x, int y) {
		startX = x;
		startY = y;
		movingXZ = 1;
	}
	void StopMoveXZ() {
		startX = 0;
		startY = 0;
		movingXZ = 0;
	}
	void UpdatePositionXZ(int deltaX, int deltaZ) {
		if (movingXZ) {
			position.x += deltaX;
			position.z += deltaZ;
		}
	}
	void StartMoveXY(int x, int y) {
		startX = x;
		startY = y;
		movingXY = 1;
	}
	void StopMoveXY() {
		startX = 0;
		startY = 0;
		movingXY = 0;
	}
	void UpdatePositionXY(int deltaX, int deltaY) {
		if (movingXY) {
			position.x += deltaX;
			position.y += deltaY;
		}
	}

	D3DXMATRIX GetTransformMatrix() {
		D3DXMATRIX rotX, rotY, rotZ, pos;
		D3DXMatrixRotationX(&rotX, 1.0f);
		D3DXMatrixIdentity(&rotY);
		D3DXMatrixIdentity(&rotZ);
		D3DXMatrixTranslation(&pos, position.x, -position.y, position.z);

		return pos * rotZ * rotY * rotX;
	}
};

