#ifndef INCLUDED_TILEMAP_H
#define INCLUDED_TILEMAP_H

#include <XEngine.h>

class TileMap
{
public:
	void LoadMap(int rows, int columns);
	void UnloadMap();

	void LoadTextures(const char* textureListFileName);
	void UnloadTextures();

	void Update(float deltaTime);
	void Render();

	void EnableEditor(bool enable) { mEnableEditor = enable; }
	bool IsEditorEnable() const { return mEnableEditor; }

	void EnableStartPoint(bool enable) { mSignStartPoint = enable; }
	void EnableEndPoint(bool enable) { mSignEndPoint = enable; }

	bool IsEnableStartPoint() { return mSignStartPoint; }
	bool IsEnableEndPoint() { return mSignEndPoint; }

	int GetStartX() const { return mStartX; }
	int GetStartY() const { return mStartY; }
	int GetEndX() const { return mEndX; }
	int GetEndY() const { return mEndY; }

	void CheckWall(size_t index);
	bool IsWall() const { return mIsWall; }

	bool IsLoadMap() const { return mIsLoad; }

private:
	void ShowEditor();

	std::vector<std::string> mMapName;
	std::unique_ptr<int[]> mTileMap; // auto delet after left the pointer array
	std::unique_ptr<X::TextureId[]> mTextureIds;
	int mRows = 0;
	int mColums = 0;
	int mTextureCount = 0;
	int mChange = 0;
	bool mEnableEditor = false;
	bool mIsWall = false;
	
	bool mSignStartPoint = false;
	bool mSignEndPoint = false;

	int mStartX = 0, mStartY = 0;
	int mEndX = 0, mEndY = 0;

	bool mIsLoad = false;
};


#endif // !INCLUDED_TILEMAP_H