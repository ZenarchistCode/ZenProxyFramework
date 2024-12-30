class ActionZenFillBottleBaseSinkTest : ActionFillBottleBase
{
	void ActionZenFillBottleBaseSinkTest()
	{
		m_CallbackClass = ActionFillBottleBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_Text = "#fill";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNone();
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

		if (item.IsFullQuantity())
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
			if (!sink)
				return false;
		}

		return true;
	}

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if (super.SetupAction(player, target, item, action_data, extra_data))
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL;
			return true;
		}

		return false;
	}

	override int GetLiquidType(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (IsTargetingZenActionProxy(player, target, item))
			return LIQUID_WATER;

		return LIQUID_NONE;
	}

	override void SetupStance(PlayerBase player)
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
}

