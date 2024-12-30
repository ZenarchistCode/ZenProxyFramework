class ActionZenRepairSinkTestCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1);
	}
};

class ActionZenRepairSinkTest : ActionContinuousBase
{
	void ActionZenRepairSinkTest()
	{
		m_CallbackClass = ActionZenRepairSinkTestCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursorParent(UAMaxDistances.SMALL);
	}

	override string GetText()
	{
		return "#repair";
	}

	override bool HasZenProxyTarget()
	{
		return true;
	}

	override string GetZenProxyNameTarget()
	{
		return ZenProxyConstants.SINKS;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!GetZenProxyFrameworkConfig().DebugOn)
			return false;

		if (!IsTargetingZenActionProxy(player, target, item))
			return false;

		if (GetGame().IsClient())
		{
			// Cursor target object is not sync'd to server, so don't check server-side.
			vector proxyTargetLS;
			vector proxyTargetWS;
			string proxyTargetHouseType;
			string proxyTargetProxyType;

			proxyTargetProxyType = GetZenActionProxyTargetEx(player, target, item, proxyTargetLS, proxyTargetWS, proxyTargetHouseType);
			
			// If sink entity exists, then that means sink is already fixed
			Object sink = GetProxyObject(proxyTargetWS, "ZenProxySinkTest");
			if (sink)
				return false;
		}
		
		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		vector proxyTargetWS = GetZenProxyTargetWS(action_data);
		ItemBase item = ItemBase.Cast(GetProxyObject(proxyTargetWS, "ZenProxySinkTest"));
		if (item)
			return;
		
		// Create an invisible entity at this sink location - simplest way to sync the working sink to all players on the server and handle lifetime in XML etc.
		ZenProxySinkTest sinkProxyObj = ZenProxySinkTest.Cast(GetGame().CreateObjectEx("ZenProxySinkTest", proxyTargetWS, ECE_FULL));
		if (sinkProxyObj)
		{
			action_data.m_MainItem.SetHealth(0);
			return;
		}

		Error("[ZenProxyFramework] Failed to create ZenProxySinkTest @ " + proxyTargetWS);
	}
	
	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);

		if (!GetGame().IsDedicatedServer())
			return;

		Param2<bool, string> play = new Param2<bool, string>(true, "wrench_loop_SoundSet");
		GetGame().RPCSingleParam(action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
	}

	override void OnEnd(ActionData action_data)
	{
		super.OnEnd(action_data);

		if (!GetGame().IsDedicatedServer())
			return;

		Param2<bool, string> play = new Param2<bool, string>(false, "wrench_loop_SoundSet");
		GetGame().RPCSingleParam(action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
	}

	override void OnEndAnimationLoop(ActionData action_data)
	{
		super.OnEndAnimationLoop(action_data);

		if (!GetGame().IsDedicatedServer())
			return;

		Param2<bool, string> play = new Param2<bool, string>(false, "wrench_loop_SoundSet");
		GetGame().RPCSingleParam(action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
	}
}