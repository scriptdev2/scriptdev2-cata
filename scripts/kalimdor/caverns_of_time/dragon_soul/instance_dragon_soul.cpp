/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: instance_dragon_soul
SD%Complete: 0
SDComment: Placeholder
SDCategory: Dragon Soul
EndScriptData */

#include "precompiled.h"
#include "dragon_soul.h"

instance_dragon_soul::instance_dragon_soul(Map* pMap) : ScriptedInstance(pMap),
    m_uiUltraxionResetTimer(0)
{
    Initialize();
}

void instance_ruby_sanctum::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_dragon_soul::IsEncounterInProgress() const
{
    for (uint8 i = 1; i < MAX_ENCOUNTER ; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_dragon_soul::OnPlayerEnter(Player* /*pPlayer*/)
{
  // I don't know, what to write.
}

void instance_dragon_soul::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_MORCHOK:
		case NPC_WARLORD_ZONOZZ:
		case NPC_YORSAHJ_UNSLEEPING:
		case NPC_HAGARA:
		case NPC_ULTRAXION:
		case NPC_THRALL:
		case NPC_ALEXSTRASZA:
		case NPC_YSERA:
		case NPC_KALECGOS:
		case NPC_NOZDORMU:
		    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();

          
            
            break;
    }
}

void instance_dragon_soul::OnCreatureEvade(Creature* pCreature)
{
        if (pCreature->GetEntry() == NPC_ULTRAXION)
        SetData(TYPE_ULTRAXION, FAIL);
}

void instance_dragon_soul::OnCreatureDeath(Creature* pCreature)
{
    // Reset encounter if need.
}
void instance_dragon_soul::SetData(uint32 uiType, uint32 uiData)

{
    switch (uiType)
    {
	    case TYPE_ULTRAXION
		    m_auiEncounter[uiType] = uiData;
			if (uiData == FAIL)
			{
                m_uiUltraxionReset = 20000;
			
            if (Creature* pUltraxion = GetSingleCreatureFromStorage(NPC_ULTRAXION))
                pUltraxion->AI()->SendAIEvent(AI_EVENT_CUSTOM_A, pUltraxion, pUltraxion);
            
			if (Creature* pThrall = GetSingleCreatureFromStorage(NPC_THRALL))
                pThrall->ForcedDespawn(8000);
			if (Creature* pAlexstrasza = GetSingleCreatureFromStorage(NPC_ALEXSTRASZA))
                pAlexstrasza->ForcedDespawn(8000);
			if (Creature* pYsera = GetSingleCreatureFromStorage(NPC_YSERA))
                pYsera->ForcedDespawn(8000);
			if (Creature* pKalecgos = GetSingleCreatureFromStorage(NPC_KALECGOS))
                pKalecgos->ForcedDespawn(8000);
			if (Creature* pNozdormu = GetSingleCreatureFromStorage(NPC_NOZDORMU))
                pNozdormu->ForcedDespawn(8000);
			}	
			break;

	}
	
void instance_dragon_soul::Update(uint32 uiDiff)
{
    DialogueUpdate(uiDiff);
	
    if (m_uiUltraxionReset)
    {
        if (m_uiUltraxionReset <= uiDiff)
        {
            if (Creature* pThrall = GetSingleCreatureFromStorage(NPC_THRALL))
                pThrall->Respawn();
            if (Creature* pAlexstrasza = GetSingleCreatureFromStorage(NPC_ALEXSTRASZA))
                pAlexstrasza->Respawn();
			if (Creature* pYsera = GetSingleCreatureFromStorage(NPC_YSERA))
                pYsera->Respawn();
			if (Creature* pKalecgos = GetSingleCreatureFromStorage(NPC_KALECGOS))
                pKalecgos->Respawn();
			if (Creature* pNozdormu = GetSingleCreatureFromStorage(NPC_NOZDORMU))
                pNozdormu->Respawn();
			
            m_uiUltraxionReset = 0;
        }
        else
            m_uiUltraxionReset -= uiDiff;
    }
}	


uint32 instance_dragon_soul::GetData(uint32 uiType) const
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}



void instance_dragon_soul::Load(const char* in)
{
    if (!in)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(in);

    std::istringstream loadStream(in);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >>
               m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;

}
		    