#include "TileMap.h"
#include <ImGui/Inc/imgui.h>

void TileMap::LoadMap(int rows, int columns)
{
	mRows = rows;
	mColums = columns;
	mTileMap = std::make_unique<int[]>(mRows * mColums);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColums; ++x)
		{
			int index = x + (y * mColums);
			mTileMap[index] = 0;
		}
	}

	mIsLoad = true;
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

	if (IsEditorEnable())
	{
		ShowEditor();

		if (X::IsMouseDown(X::Mouse::LBUTTON))
		{
			int colum = (int)(X::GetMouseScreenX() / tileWidth);
			int row = (int)(X::GetMouseScreenY() / tileHeight);

			int index = colum + (row * mColums);
			mTileMap[index] = mChange;
		}
	}


	if (mSignStartPoint)
	{
		if (X::IsMouseDown(X::Mouse::LBUTTON))
		{
			mStartX = (int)(X::GetMouseScreenX() / tileWidth);
			mStartY = (int)(X::GetMouseScreenY() / tileHeight);
			mSignStartPoint = false;
		}
	}

	if (mSignEndPoint)
	{
		if (X::IsMouseDown(X::Mouse::LBUTTON))
		{
			mEndX = (int)(X::GetMouseScreenX() / tileWidth);
			mEndY = (int)(X::GetMouseScreenY() / tileHeight);
			mSignEndPoint = false;
		}
	}

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

void TileMap::ShowEditor()
{
	ImGui::Begin("Tile Map Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	std::string mapName = mMapName[mChange];
	ImGui::Text("Current Tile Name: %s", mapName.c_str());

	if (ImGui::Button("Change Tile"))
	{
		if (mChange < mTextureCount - 1)
		{
			mChange++;
		}
		else
		{
			mChange = 0;
		}

	}
	
	ImGui::End();
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