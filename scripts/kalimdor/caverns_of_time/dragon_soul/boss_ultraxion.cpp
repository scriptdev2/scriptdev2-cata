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
SDName: boss_ultraxion
SD%Complete: 0
SDComment: Placeholder
SDCategory: Dragon Soul
EndScriptData */

#include "precompiled.h"
#include "dragon_soul.h"

enum
{
    SPELL_HOUR_TWILIGHT                 = 106371,
	TWILIGHT_BURST                      = 106415,
	SPELL_UNSTABLE_MONSTROSITY_6        = 106372,
	SPELL_UNSTABLE_MONSTROSITY_5        = 106376,
	SPELL_UNSTABLE_MONSTROSITY_4        = 106377,
	SPELL_UNSTABLE_MONSTROSITY_3        = 106378,
	SPELL_UNSTABLE_MONSTROSITY_2        = 106379,
	SPELL_UNSTABLE_MONSTROSITY_1        = 106380,
	
  
};

/*######
## boss_ultraxion
######*/

struct boss_ultraxionAI : public ScriptedAI
{
    boss_dragon_soulAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_dragon_soul*)pCreature->GetInstanceData();
        Reset();
    }
	
	ScriptedInstance* m_pInstance;
	
	uint32 m_uiTwilightEruptionTimer;
	uint32 m_uiHourofTwilightTimer;
	uint32 m_uiTwilightBlastTimer;
	uint32 m_uiUnstableMonstrosity6;
	uint32 m_uiUnstableMonstrosity5;
	uint32 m_uiUnstableMonstrosity4;
	uint32 m_uiUnstableMonstrosity3;
	uint32 m_uiUnstableMonstrosity2;
	uint32 m_uiUnstableMonstrosity1;
	
	
    void Reset() override
    {
        // Flying animation
        m_creature->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
		m_creature->SetLevitate(true);
        SetCombatMovement(false);

		m_uiTwilightEruptionTimer        = 6 * MINUTE * IN_MILLISECONDS;
        m_uiHourofTwilightTimer          = 45000;
		m_uiTwilightBlastTimer           = 2000;
		m_uiUnstableMonstrosity6         = 50000;
		m_uiUnstableMonstrosity5         = 100000;
		m_uiUnstableMonstrosity4         = 150000;
		m_uiUnstableMonstrosity3         = 200000;
		m_uiUnstableMonstrosity2         = 250000;
		m_uiUnstableMonstrosity1         = 300000;
		

		
    }

