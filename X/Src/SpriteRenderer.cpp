//====================================================================================================
// Filename:	SpriteRenderer.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "SpriteRenderer.h"

#include "GraphicsSystem.h"
#include "Texture.h"
#include <DirectXTK/Inc/CommonStates.h>
#include <DirectXTK/Inc/SpriteBatch.h>

using namespace X;

namespace
{
	DirectX::XMFLOAT2 GetOrigin(uint32_t width, uint32_t height, Pivot pivot)
	{
		auto index = static_cast<std::underlying_type_t<Pivot>>(pivot);
		constexpr DirectX::XMFLOAT2 offsets[] =
		{
			{ 0.0f, 0.0f }, // TopLeft
			{ 0.5f, 0.0f }, // Top
			{ 1.0f, 0.0f }, // TopRight
			{ 0.0f, 0.5f }, // Left
			{ 0.5f, 0.5f }, // Center
			{ 1.0f, 0.5f }, // Right
			{ 0.0f, 1.0f }, // BottomLeft
			{ 0.5f, 1.0f }, // Bottom
			{ 1.0f, 1.0f }, // BottomRight
		};
		return { width * offsets[index].x, height * offsets[index].y };
	}

	DirectX::XMFLOAT2 ToXMFLOAT2(const X::Math::Vector2& v)
	{
		return { v.x, v.y };
	}

	SpriteRenderer* sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	XASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initialized!");
	sSpriteRenderer = new SpriteRenderer();
	sSpriteRenderer->Initialize();
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		SafeDelete(sSpriteRenderer);
	}
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer* SpriteRenderer::Get()
{
	XASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] No instance registered.");
	return sSpriteRenderer;
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer::SpriteRenderer()
	: mCommonStates(nullptr)
	, mSpriteBatch(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer::~SpriteRenderer()
{
	XASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Renderer not freed.");
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::Initialize()
{
	XASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Already initialized.");
	GraphicsSystem* gs = GraphicsSystem::Get();
	mCommonStates = new DirectX::CommonStates(gs->GetDevice());
	mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::Terminate()
{
	XASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Already terminated.");
	SafeDelete(mSpriteBatch);
	SafeDelete(mCommonStates);
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::BeginRender()
{
	XASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::EndRender()
{
	XASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	mSpriteBatch->End();

	// Restore state objects
	auto blendState = mCommonStates->Opaque();
	auto depthStencilState = mCommonStates->DepthDefault();
	auto rasterizerState = mCommonStates->CullCounterClockwise();

	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState, 0);
	context->RSSetState(rasterizerState);
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::Draw(const Texture& texture, const Math::Vector2& pos, float rotation, Pivot pivot)
{
	XASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	DirectX::XMFLOAT2 origin = GetOrigin(texture.GetWidth(), texture.GetHeight(), pivot);
	mSpriteBatch->Draw(texture.mShaderResourceView, ToXMFLOAT2(pos), nullptr, DirectX::Colors::White, rotation, origin);
}

//----------------------------------------------------------------------------------------------------
void SpriteRenderer::Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation, Pivot pivot)
{
	XASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	RECT rect;
	rect.left = static_cast<LONG>(sourceRect.left);
	rect.top = static_cast<LONG>(sourceRect.top);
	rect.right = static_cast<LONG>(sourceRect.right);
	rect.bottom = static_cast<LONG>(sourceRect.bottom);
	DirectX::XMFLOAT2 origin = GetOrigin(rect.right - rect.left, rect.bottom - rect.top, pivot);
	mSpriteBatch->Draw(texture.mShaderResourceView, ToXMFLOAT2(pos), &rect, DirectX::Colors::White, rotation, origin);
}