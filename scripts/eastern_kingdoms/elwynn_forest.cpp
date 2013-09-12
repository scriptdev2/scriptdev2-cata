/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Elwynn_Forest
SD%Complete: 100
SDComment: Hogger event
SDCategory: Elwynn Forest
EndScriptData */

/* ContentData
npc_hogger
EndContentData */

#include "precompiled.h"

/*######
## npc_hogger
######*/

enum
{
    SAY_CALL_HELP           = -1004000,
    WHISPER_EATING          = -1004001,         // note: this type may need to be updated in the future
    SAY_HOGGER_BEATEN       = -1004002,

    SPELL_EATING            = 87351,
    SPELL_SUMMON_MINIONS    = 87366,
    SPELL_VICIOUS_SLICE     = 87337,

    NPC_MINION_OF_HOOGER    = 46932,
};

struct MANGOS_DLL_DECL npc_hoggerAI : public ScriptedAI
{
    npc_hoggerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiViciousSliceTimer;
    uint32 m_uiEatingEndTimer;
    bool m_bHasSpawnedMinions;
    bool m_bEncounterFinished;

    void Reset() override
    {
        m_uiViciousSliceTimer = urand(15000, 20000);
        m_uiEatingEndTimer    = 0;
        m_bHasSpawnedMinions  = false;
        m_bEncounterFinished  = false;
    }

    void EnterEvadeMode() override
    {
        m_creature->RemoveAllAurasOnEvade();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        // start epilogue event
        if (m_bEncounterFinished)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            m_creature->GetMotionMaster()->MoveWaypoint();
        }
        else
        {
            if (m_creature->isAlive())
                m_creature->GetMotionMaster()->MoveTargetedHome();

            Reset();
        }

        m_creature->SetLootRecipient(NULL);
    }

    void AttackStart(Unit* pWho) override
    {
        // special case for epilogue
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // special case for epilogue
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId)
            return;

        DoCastSpellIfCan(m_creature, SPELL_EATING);
        m_uiEatingEndTimer = 12000;
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_MINION_OF_HOOGER)
            pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage) override
    {
        // evade and start epilogue when near death
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEncounterFinished)
            {
                m_bEncounterFinished = true;
                m_creature->AI()->EnterEvadeMode();
                DoScriptText(SAY_HOGGER_BEATEN, m_creature);

                if (pDealer->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)pDealer)->KilledMonsterCredit(m_creature->GetEntry());
            }
        }

        // start eating at 50% hp
        if (!m_bHasSpawnedMinions && !m_bEncounterFinished && m_creature->GetHealthPercent() < 50.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_MINIONS) == CAST_OK)
            {
                DoScriptText(SAY_CALL_HELP, m_creature);
                DoScriptText(WHISPER_EATING, m_creature, pDealer);

                SetCombatMovement(false);
                m_creature->GetMotionMaster()->MovePoint(1, -10144.47f, 668.8424f, 36.90841f);
                m_bHasSpawnedMinions = true;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiViciousSliceTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_VICIOUS_SLICE) == CAST_OK)
                m_uiViciousSliceTimer = urand(15000, 20000);
        }
        else
            m_uiViciousSliceTimer -= uiDiff;

        // resume combat after eat
        if (m_uiEatingEndTimer)
        {
            if (m_uiEatingEndTimer <= uiDiff)
            {
                SetCombatMovement(true);
                DoStartMovement(m_creature->getVictim());
                m_uiEatingEndTimer = 0;
            }
            else
                m_uiEatingEndTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_hogger(Creature* pCreature)
{
    return new npc_hoggerAI(pCreature);
}

void AddSC_elwynn_forest()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_hogger";
    pNewScript->GetAI = &GetAI_npc_hogger;
    pNewScript->RegisterSelf();
}
