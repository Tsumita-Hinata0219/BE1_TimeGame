#pragma once

#include "Math/MyMath.h"
#include "GameObject/GameObject.h"


/* JsonからよみとるPlayerのデータ */
struct EntityPlayerData {

};

/* JsonからよみとるEnemyのデータ */
struct EntityEnemyData {
};


/* JsonからよみとるEntityData */
struct EntityData {
	std::string type;
	std::string entityName;
	SRTData srt{};
	EntityPlayerData playerData{};
	EntityEnemyData enemyData{};
	std::map<std::string, std::unique_ptr<EntityData>> children;
};

