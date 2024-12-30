class ActionZenProxyInteractCB : ActionInteractBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUse;
    }
}

class ActionZenProxyTargetDebug : ActionInteractBase
{
    string m_lastTarget = "";

    void ActionZenProxyTargetDebug()
    {
        m_CallbackClass = ActionZenProxyInteractCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINotPresent;
        m_ConditionTarget = new CCTCursorParent(UAMaxDistances.DEFAULT);
    }

    override string GetText()
    {
        return "Proxy -> clipboard";
    }

    override bool HasZenProxyTarget()
    {
        return true;
    }

    override string GetZenProxyNameTarget()
    {
        return "proxy_debug";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!GetZenProxyFrameworkConfig().DebugOn)
            return false;

        return IsTargetingZenActionProxy(player, target, item);
    }

    override void OnStartClient(ActionData action_data)
    {
        super.OnStartClient(action_data);

        House party = House.Cast(action_data.m_Target.GetParent());
        if (!party)
            return;

        string proxyName = GetZenActionProxyTarget(action_data);
        GetGame().CopyToClipboard(proxyName);
        GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "[CLIENT] ProxyName: " + proxyName, ""));
    }

    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        ZenProxyActionData zenProxyActionData = ZenProxyActionData.Cast(action_data);
        if (!zenProxyActionData)
            return;

        vector targetLS = zenProxyActionData.m_ProxyTargetLS;
        vector targetWS = zenProxyActionData.m_ProxyTargetWS;
        string proxyTargetHouseType = zenProxyActionData.m_ProxyTargetHouseType;
        string proxyTargetProxyType = zenProxyActionData.m_ProxyTargetProxyType;
        
        House party = House.Cast(action_data.m_Target.GetParent());
        if (!party)
            return;

        vector targetPos = party.GetPosition();
        vector localPos = party.WorldToModel(targetPos);

        string proxyTarget = GetZenHouseProxyPlugin().GetTargetedProxy(proxyTargetHouseType, targetLS);
        SendMessageToClient(action_data.m_Player, "[SERVER] ProxyName - Client: " + proxyTarget + " / Server: " + proxyTargetProxyType);
    }
}
