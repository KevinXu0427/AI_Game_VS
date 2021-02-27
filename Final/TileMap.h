#ifndef INCLUDED_TILEMAP_H
#define INCLUDED_TILEMAP_H

#include <XEngine.h>

class TileMap
{
public:
	void LoadMap(const char* mapFileName);
	void UnloadMap();

	void LoadTextures(const char* textureListFileName);
	void UnloadTextures();

	void Update(float deltaTime);
	void Render();

	void CheckWall(size_t index);
	bool IsWall() const { return mIsWall; }

	bool IsLoadMap() const { return mIsLoad; }

	std::vector<int> GetFreeTileIndex() { return mtileIndex; }

private:

	std::vector<std::string> mMapName;
	std::unique_ptr<int[]> mTileMap; // auto delet after left the pointer array
	std::unique_ptr<X::TextureId[]> mTextureIds;
	int mRows = 0;
	int mColums = 0;
	int mTextureCount = 0;

	bool mIsWall = false;

	int mStartX = 0, mStartY = 0;
	int mEndX = 0, mEndY = 0;

	std::vector<int> mtileIndex;

	bool mIsLoad = false;

};


#endif // !INCLUDED_TILEMAP_H