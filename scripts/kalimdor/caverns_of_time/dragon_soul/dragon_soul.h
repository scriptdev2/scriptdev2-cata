/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DRAGON_SOUL_H
#define DEF_DRAGON_SOUL_H

enum
{
    MAX_ENCOUNTER                   = 5,

    TYPE_MORCHOK                    = 0,
    TYPE_WARLORD_ZONOZZ             = 1,
    TYPE_YORSAJH_UNSLEEPING         = 2,
    TYPE_HAGARA                     = 3,
	TYPE_ULTRAXION                  = 4,
	
    NPC_MORCHOK                     = 55265,            
    NPC_WARLORD_ZONOZZ              = 55308,            
    NPC_YORSAHJ_UNSLEEPING          = 55312,
    NPC_HAGARA                      = 55689,            
    NPC_ULTRAXION                   = 55294,
	
	//Aspects(Helper Ultraxion battle)
	NPC_THRALL                      = 56667,
	NPC_ALEXSTRASZA                 = 56630,
	NPC_YSERA                       = 56665,
	NPC_KALECGOS                    = 56664,
	NPC_NOZDORMU                    = 56666,

    // Spells used to Ultraxion death and evade, need instance or boss Ultraxion script for cast spell, because spell has TARGET_SELF.
    SPELL_ULTRAXION_AURA_CANCELLED  = 109281,
};

// spawn location for Ultraxion in reload case
static const float afUltraxionHomePos[4] = {-1699.75f, -2384.89f, 345.579f, 3.27f};

class instance_dragon_soul : public ScriptedInstance
{
    public:
        instance_dragon_soul(Map* pMap);

        void Initialize() override;
        bool IsEncounterInProgress() const override;

        void OnPlayerEnter(Player* pPlayer) override;
        void OnCreatureCreate(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;
		void OnCreatureDeath(Creature* pCreature) override;
		void OnCreatureEvade(Creature* pCreature) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        void Update(uint32 uiDiff) override;

        const char* Save() const override { return strInstData.c_str(); }
        void Load(const char* chrIn) override;

        // Difficulty wrappers
        bool IsHeroicDifficulty() { return instance->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC; }
        bool Is25ManDifficulty() { return instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL || instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC; }

    protected:

        std::string strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];
		
		uint32 m_uiUltraxionResetTimer;


};

#endif

        
