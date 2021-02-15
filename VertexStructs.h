#pragma once
#include "stdafx.h"

struct VertexBase
{
	VertexBase():
		Position(DirectX::XMFLOAT3(0,0,0)),
		Color(DirectX::XMFLOAT4(.3f,.4f,.4f,1.f)),
		Normal(DirectX::XMFLOAT3(0,0,0)),
		TexCoord(DirectX::XMFLOAT2(0,0))
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};

struct VertexPosCol
{
	VertexPosCol(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col):
		Position(pos),
		Color(col)
	{}

	VertexPosCol(VertexBase& vertex):
		Position(vertex.Position),
		Color(vertex.Color)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

struct VertexPosColNorm
{
	VertexPosColNorm(){}
	VertexPosColNorm(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col, DirectX::XMFLOAT3 norm):
		Position(pos),
		Color(col),
		Normal(norm)
	{}
	VertexPosColNorm(VertexBase& vertex):
		Position(vertex.Position),
		Color(vertex.Color),
		Normal(vertex.Normal)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
};

struct VertexPosColNormTex
{
	VertexPosColNormTex(VertexBase& vertex):
		Position(vertex.Position),
		Color(vertex.Color),
		Normal(vertex.Normal),
		TexCoord(vertex.TexCoord)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};

struct VertexPosNormTex
{
	VertexPosNormTex(){}
	VertexPosNormTex(VertexBase& vertex):
		Position(vertex.Position),
		Normal(vertex.Normal),
		TexCoord(vertex.TexCoord)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};