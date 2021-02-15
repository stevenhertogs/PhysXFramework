#pragma once
#include "GameObject.h"

class CubePosColorNorm: public GameObject
{
public:
	CubePosColorNorm(float width, float height, float depth);
	CubePosColorNorm(float width, float height, float depth, DirectX::XMFLOAT4 color);
	~CubePosColorNorm();

	CubePosColorNorm(const CubePosColorNorm& t) = delete;
	CubePosColorNorm& operator=(const CubePosColorNorm& t) = delete;

	virtual void Initialize() override;
	virtual void Update(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	int m_NumIndices;
	float m_Width, m_Height, m_Depth;

	DirectX::XMFLOAT4 m_CustomColor;
	bool m_UseCustomColor;

	ID3D11InputLayout			*m_pVertexLayout;
	ID3D11Buffer				*m_pVertexBuffer;
	ID3D11Buffer				*m_pIndexBuffer;
	ID3DX11Effect				*m_pEffect;
	ID3DX11EffectTechnique		*m_pTechnique;
	ID3DX11EffectMatrixVariable *m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable *m_pMatWorldVariable;


};

