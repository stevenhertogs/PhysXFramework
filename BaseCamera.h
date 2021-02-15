#pragma once
class SceneContext;

class BaseCamera
{
public:
	BaseCamera();
	virtual ~BaseCamera() = default;

	DirectX::XMFLOAT4X4 GetView() const { return m_View; }
	DirectX::XMFLOAT4X4 GetProjection() const { return m_Projection; }
	const DirectX::XMFLOAT4X4 & GetViewProjection() const { return m_ViewProjection; }

	DirectX::XMFLOAT3 GetPosition() const { return m_Position; }
	DirectX::XMFLOAT3 GetUp() const { return m_Up; }
	DirectX::XMFLOAT3 GetRight() const { return m_Right; }
	DirectX::XMFLOAT3 GetForward() const { return m_Forward; }
	DirectX::XMFLOAT3 GetTarget() const { return m_Target; }

	void SetPosition(const DirectX::XMFLOAT3& newPos) { m_Position = newPos; };
	void SetForward(const DirectX::XMFLOAT3& newForward) { m_Forward = newForward; };


	void BuildView();

	virtual void BuildProjection(float fov, float aspectRatio, float nearZ, float farZ);
	virtual void Update(const SceneContext& sceneContext) = 0;

protected:
	void BuildViewProjection();

	DirectX::XMFLOAT4X4 m_World, m_View, m_Projection, m_ViewProjection;
	DirectX::XMFLOAT3 m_Up, m_Right, m_Forward, m_Position, m_Target;
};

