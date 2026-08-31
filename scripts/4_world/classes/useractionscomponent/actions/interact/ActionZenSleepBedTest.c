//! EXAMPLE ACTION FOR PERFORMING LIE-DOWN EMOTE ON BED PROXIES
class ActionZenSleepBedTest : ActionInteractBase
{
    void ActionZenSleepBedTest()
    {
        m_CallbackClass = ActionZenProxyInteractCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone; //CCINotPresent;
        m_ConditionTarget = new CCTCursorParent(UAMaxDistances.DEFAULT);
    }

    override string GetText()
    {
        return "#STR_USRACT_ID_EMOTE_LYINGDOWN";
    }

    override bool HasZenProxyTarget()
    {
        return true;
    }

    override string GetZenProxyNameTarget()
    {
        return ZenProxyConstants.BEDS;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!GetZenProxyFrameworkConfig().DebugOn)
            return false;

        return IsTargetingZenActionProxy(player, target, item);
    }

    override void OnEndClient(ActionData action_data)
    {
        //! LIE DOWN EMOTE
        if (action_data.m_Player.GetEmoteManager())
        {
            action_data.m_Player.GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_LYINGDOWN);
        }
    }

    override void OnEndServer(ActionData action_data)
    {
        //! TODO: Handle any sleep-related stuff server-side.

        string targetProxyType = GetZenProxyTargetType(action_data);
        if (targetProxyType == "postel_manz_kov" || targetProxyType.Contains("matress"))
        {
            SendMessageToClient(action_data.m_Player, "[SERVER] This bed has no pillow or blanket!");
        }
    }
}

