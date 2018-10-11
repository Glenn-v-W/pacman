#pragma once
#include "BaseComponent.h"

enum class TileType : int
{
	GridTiles_01 = 1,
	GridTiles_02 = 2,
	GridTiles_03 = 3,
	GridTiles_04 = 4,
	GridTiles_05 = 5,
	GridTiles_06 = 6,
	GridTiles_07 = 7,
	GridTiles_08 = 8,
	GridTiles_09 = 9,
	GridTiles_10 = 10,
	GridTiles_11 = 11,
	GridTiles_12 = 12,
	GridTiles_13 = 13,
	GridTiles_14 = 14,
	GridTiles_15 = 15,
	GridTiles_16 = 16,
	GridTiles_17 = 17,
	GridTiles_18 = 18,
	GridTiles_19 = 19,
	GridTiles_20 = 20,
	GridTiles_21 = 21,
	GridTiles_22 = 22,
	GridTiles_23 = 23,
	GridTiles_24 = 24,
	GridTiles_25 = 25,
	GridTiles_26 = 26,
	GridTiles_27 = 27,
	GridTiles_28 = 28,
	GridTiles_29 = 29,
	GridTiles_30 = 30,
	GridTiles_31 = 31,
	GridTiles_32 = 32,
	GridTiles_33 = 33,
	GridTiles_34 = 34,
	GridTiles_35 = 35,
	GridTiles_36 = 36,
	GridTiles_37 = 37,
	GridTiles_38 = 38,
	GridTiles_39 = 39,
	GridTiles_40 = 40,
	GridTiles_41 = 41,
	GridTiles_42 = 42,
	GridTiles_43 = 43,
	GridTiles_44 = 44,
	GridTiles_45 = 45,
	GridTiles_46 = 46,
	GridTiles_47 = 47,
	GridTiles_48 = 48
};

class GridComponent : public BaseComponent
{
public:
	GridComponent();
	virtual ~GridComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetTileType(TileType tileType);
	bool GetIsWall();
	int TriggerTile();

private:
	void SetTexture();
	void SetIsWall();

	bool firstUpdate = true;

	TileType m_TileType;
	std::string m_TileFilePath{};

	bool m_IsWall = false;
};