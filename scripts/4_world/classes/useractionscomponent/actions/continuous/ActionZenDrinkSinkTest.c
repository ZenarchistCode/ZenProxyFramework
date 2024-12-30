class ActionZenDrinkSinkTest : ActionDrinkWellContinuous
{
	void ActionZenDrinkSinkTest()
	{
		m_CallbackClass = ActionDrinkWellContinuousCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKWELL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#drink";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone();
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
	}

	override bool HasZenProxyTarget()
	{
		return true;
	}

	override string GetZenProxyNameTarget()
	{
		return ZenProxyConstants.SINKS;
	}

	override bool IsDrink()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!GetZenProxyFrameworkConfig().DebugOn)
			return false;


		if (!IsTargetingZenActionProxy(player, target, item))
			return false;

		if (item && item.IsHeavyBehaviour())
			return false;

		if (!player.CanEatAndDrink())
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
			{
				return false;
			}
		}

		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		Param1<float> nacdata = Param1<float>.Cast(action_data.m_ActionComponent.GetACData());
		if (nacdata)
		{
			float amount = UAQuantityConsumed.DRINK;
			action_data.m_Player.Consume(null, amount, EConsumeType.ENVIRO_POND);
		}

		if (action_data.m_Player.HasBloodyHands() && !action_data.m_Player.GetInventory().FindAttachment(InventorySlots.GLOVES))
		{
			action_data.m_Player.SetBloodyHandsPenalty();
		}
	}
}
