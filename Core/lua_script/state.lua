template_state = {}

template_state[NPCAlive] = 
{
	Enter = function(npc)
		npc:PrintLog("NPCAlive Enter");	
	end,

	Decide = function(npc)
		npc:PrintLog("NPCAlive Decide");
	end,

	Exit = function(npc)
		npc:PrintLog("NPCAlive Exit");
	end
}

AI_1 = DeepCopy(template_state);
