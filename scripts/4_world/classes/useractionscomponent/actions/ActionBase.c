class ZenProxyActionReceiveData : ActionReciveData
{
    vector m_ProxyTargetLS;
    vector m_ProxyTargetWS;
    string m_ProxyTargetHouseType;
    string m_ProxyTargetProxyType;
}

class ZenProxyActionData : ActionData
{
    vector m_ProxyTargetLS;
    vector m_ProxyTargetWS;
    string m_ProxyTargetHouseType;
    string m_ProxyTargetProxyType;
}

modded class ActionBase
{
    protected static ref ZenHouseProxyPlugin m_ZenHouseProxyPlugin;

    ZenHouseProxyPlugin GetZenHouseProxyPlugin()
    {
        if (!m_ZenHouseProxyPlugin)
            m_ZenHouseProxyPlugin = ZenHouseProxyPlugin.Cast(GetPlugin(ZenHouseProxyPlugin));

        return m_ZenHouseProxyPlugin;
    }

    override ActionData CreateActionData()
    {
        if (HasZenProxyTarget())
            return new ZenProxyActionData;

        return super.CreateActionData();
    }

    bool HasZenProxyTarget()
    {
        return false;
    }

    string GetZenProxyNameTarget()
    {
        return "";
    }

    // For overriding house target type (if blank, any proxy will do - house type doesn't matter)
    string GetZenProxyHouseTarget()
    {
        return "";
    }

    bool IsTargetingZenActionProxy(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return GetZenActionProxyTarget(player, target, item) != "";
    }

    string GetZenActionProxyTarget(ActionData action_data)
    {
        vector targetLS;
        vector targetWS;
        string houseType;

        return GetZenActionProxyTarget(action_data.m_Player, action_data.m_Target, action_data.m_MainItem);
    }

    string GetZenActionProxyTarget(PlayerBase player, ActionTarget target, ItemBase item)
    {
        vector targetLS;
        vector targetWS;
        string houseType;

        return GetZenActionProxyTargetEx(player, target, item, targetLS, targetWS, houseType);
    }

    string GetZenActionProxyTargetEx(PlayerBase player, ActionTarget target, ItemBase item, out vector targetLS, out vector targetWS, out string houseType)
    {
        #ifdef SERVER
        return "GetGame().IsDedicatedServer() = true";
        #endif

        if (!target || !target.GetParent() || !target.GetObject())
            return "";

        string parentType = "";
        string targetType = "";

        if (target.GetParent())
            parentType = target.GetParent().GetType();

        if (target.GetObject())
            targetType = target.GetObject().GetType();

        vector targetPos = target.GetObject().GetPosition(); // localspace of proxy obj inside house
        vector worldPos = target.GetParent().ModelToWorld(targetPos);

        houseType = GetZenProxyHouseTarget();
        if (houseType == "")
            houseType = target.GetParent().GetType();

        string proxyTarget = GetZenHouseProxyPlugin().GetTargetedProxy(houseType, targetPos);

        array<string> actionTargets = new array<string>;
        string actionTarget = GetZenProxyNameTarget();

        if (actionTarget.IndexOf(",") != -1)
            actionTarget.Split(",", actionTargets);
        else
            actionTargets.Insert(actionTarget);

        foreach (string proxyCheck : actionTargets)
        {
            if (proxyTarget == proxyCheck || proxyCheck == "proxy_debug")
            {
                targetLS = targetPos;
                targetWS = worldPos;
                return proxyTarget;
            }
        }

        return "";
    }

    //! CLIENT -> SAVE PROXY TARGET INFO TO SYNC'D ACTIONDATA
    override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
    {
        bool setupAction = super.SetupAction(player, target, item, action_data, extra_data);

        if (HasZenProxyTarget() && setupAction && GetGame().IsClient())
        {
            ZenProxyActionData proxy_action_data;
            if (Class.CastTo(proxy_action_data, action_data))
            {
                vector proxyTargetLS;
                vector proxyTargetWS;
                string proxyTargetHouseType;
                string proxyTargetProxyType;

                proxyTargetProxyType = GetZenActionProxyTargetEx(player, target, item, proxyTargetLS, proxyTargetWS, proxyTargetHouseType);
                
                proxy_action_data.m_ProxyTargetLS = proxyTargetLS;
                proxy_action_data.m_ProxyTargetWS = proxyTargetWS;
                proxy_action_data.m_ProxyTargetHouseType = proxyTargetHouseType;
                proxy_action_data.m_ProxyTargetProxyType = proxyTargetProxyType;
            }
        }

        return setupAction;
    }

    //! CLIENT -> WRITE TO SERVER
    override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
    {
        super.WriteToContext(ctx, action_data);

        if (HasZenProxyTarget())
        {
            ZenProxyActionData proxy_action_data;
            if (Class.CastTo(proxy_action_data, action_data))
            {
                ctx.Write(proxy_action_data.m_ProxyTargetLS);
                ctx.Write(proxy_action_data.m_ProxyTargetWS);
                ctx.Write(proxy_action_data.m_ProxyTargetHouseType);
                ctx.Write(proxy_action_data.m_ProxyTargetProxyType);
            }
        }
    }

    //! SERVER -> READ FROM CLIENT
    override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
    {
        if (!HasZenProxyTarget())
            return super.ReadFromContext(ctx, action_recive_data);

        if (!action_recive_data)
            action_recive_data = new ZenProxyActionReceiveData;

        super.ReadFromContext(ctx, action_recive_data);
        ZenProxyActionReceiveData receive_data_proxy = ZenProxyActionReceiveData.Cast(action_recive_data);

        vector proxyTargetLS;
        vector proxyTargetWS;
        string proxyTargetHouseType;
        string proxyTargetProxyType;

        if (!ctx.Read(proxyTargetLS))
            return false;
        if (!ctx.Read(proxyTargetWS))
            return false;
        if (!ctx.Read(proxyTargetHouseType))
            return false;
        if (!ctx.Read(proxyTargetProxyType))
            return false;

        receive_data_proxy.m_ProxyTargetLS = proxyTargetLS;
        receive_data_proxy.m_ProxyTargetWS = proxyTargetWS;
        receive_data_proxy.m_ProxyTargetHouseType = proxyTargetHouseType;
        receive_data_proxy.m_ProxyTargetProxyType = proxyTargetProxyType;

        return true;
    }

    //! SERVER -> POST READ FROM CLIENT
    override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
    {
        super.HandleReciveData(action_recive_data, action_data);

        if (!HasZenProxyTarget())
            return;

        ZenProxyActionReceiveData receive_data_proxy = ZenProxyActionReceiveData.Cast(action_recive_data);
        ZenProxyActionData action_data_proxy = ZenProxyActionData.Cast(action_data);

        action_data_proxy.m_ProxyTargetLS = receive_data_proxy.m_ProxyTargetLS;
        action_data_proxy.m_ProxyTargetWS = receive_data_proxy.m_ProxyTargetWS;
        action_data_proxy.m_ProxyTargetHouseType = receive_data_proxy.m_ProxyTargetHouseType;
        action_data_proxy.m_ProxyTargetProxyType = receive_data_proxy.m_ProxyTargetProxyType;
    }

    vector GetZenProxyTargetLS(ActionData action_data)
    {
        if (!HasZenProxyTarget())
            return vector.Zero;

        ZenProxyActionData zenProxyActionData = ZenProxyActionData.Cast(action_data);
        if (!zenProxyActionData)
            return vector.Zero;

        return zenProxyActionData.m_ProxyTargetLS;
    }

    vector GetZenProxyTargetWS(ActionData action_data)
    {
        if (!HasZenProxyTarget())
            return vector.Zero;

        ZenProxyActionData zenProxyActionData = ZenProxyActionData.Cast(action_data);
        if (!zenProxyActionData)
            return vector.Zero;

        return zenProxyActionData.m_ProxyTargetWS;
    }

    string GetZenProxyTargetParentType(ActionData action_data)
    {
        if (!HasZenProxyTarget())
            return "";

        ZenProxyActionData zenProxyActionData = ZenProxyActionData.Cast(action_data);
        if (!zenProxyActionData)
            return "";

        return zenProxyActionData.m_ProxyTargetHouseType;
    }

    string GetZenProxyTargetType(ActionData action_data)
    {
        if (!HasZenProxyTarget())
            return "";

        ZenProxyActionData zenProxyActionData = ZenProxyActionData.Cast(action_data);
        if (!zenProxyActionData)
            return "";

        return zenProxyActionData.m_ProxyTargetProxyType;
    }

    Object GetProxyObject(vector proxyPos, string searchType, float distance = 1)
    {
        array<Object> nearest_objects = new array<Object>;
        GetGame().GetObjectsAtPosition3D(proxyPos, distance, nearest_objects, NULL);

        for (int i = 0; i < nearest_objects.Count(); i++)
        {
            Object obj = nearest_objects.Get(i);

            if (obj.IsMan())
                continue;

            if (obj.IsKindOf(searchType))
                return obj;
        }

        return NULL;
    }
}