#include "TileMap.h"
#include <ImGui/Inc/imgui.h>

void TileMap::LoadMap(const char* mapFileName)
{
	FILE* file = nullptr;
	fopen_s(&file, mapFileName, "r");
	if (file == nullptr)
	{
		return;
	}

	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Colums: %d\n", &mColums);

	mTileMap = std::make_unique<int[]>(mRows * mColums);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColums; ++x)
		{
			int index = x + (y * mColums);
			mTileMap[index] = fgetc(file) - '0';

			if (mTileMap[index] != 1)
			{
				mtileIndex.push_back(index);
			}

		}
		fgetc(file);
	}

	fclose(file);
}

void TileMap::UnloadMap()
{
	mTileMap.reset();
}

void TileMap::LoadTextures(const char * textureListFileName)
{
	FILE* file = nullptr;
	fopen_s(&file, textureListFileName, "r");
	if (file == nullptr)
	{
		return;
	}

	fscanf_s(file, "TextureCount: %d\n", &mTextureCount);

	mTextureIds = std::make_unique<X::TextureId[]>(mTextureCount);

	for (size_t i = 0; i < mTextureCount; ++i)
	{
		char textureName[1024];
		fscanf_s(file, "%s\n", textureName, std::size(textureName));
		mMapName.push_back(textureName);
		mTextureIds[i] = X::LoadTexture(textureName);
	}

	fclose(file);
}

void TileMap::UnloadTextures()
{
	mTextureIds.reset();
}

void TileMap::Update(float deltaTime)
{
	float tileWidth = (float)(X::GetSpriteWidth(mTextureIds[0]));
	float tileHeight = (float)(X::GetSpriteHeight(mTextureIds[0]));

}

void TileMap::Render()
{
	float tileWidth = (float)(X::GetSpriteWidth(mTextureIds[0]));
	float tileHeight = (float)(X::GetSpriteHeight(mTextureIds[0]));

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColums; ++x)
		{
			int index = x + (y * mColums);
			int tile = std::min(mTileMap[index], mTextureCount - 1);
			float posX = (x + 0.5f) * tileWidth;
			float posY = (y + 0.5f) * tileHeight;
			X::DrawSprite(mTextureIds[tile], { posX, posY });

		}
	}
}


void TileMap::CheckWall(size_t index)
{
	if (mTileMap[index] == 1)
	{
		mIsWall = true;
	}
	else
	{
		mIsWall = false;
	}
}