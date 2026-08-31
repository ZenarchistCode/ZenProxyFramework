modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenProxyFrameworkConfig", this, SingeplayerExecutionType.Client);
	}

	void RPC_ReceiveZenProxyFrameworkConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param2<bool, ref array<ref ZenProxyOverride>> data;
		if (!ctx.Read(data))
		{
			Error("[ZenProxyFramework] RPC_ReceiveZenProxyFrameworkConfig: sync data read error");
			return;
		}

		GetZenProxyFrameworkConfig().DebugOn = data.param1;

		if (!data.param2)
			return;

		ZenHouseProxyPlugin plugin = ZenHouseProxyPlugin.Cast(GetPlugin(ZenHouseProxyPlugin));
		
		if (plugin)
		{
			foreach (ZenProxyOverride zpo : data.param2)
			{
				plugin.AddHouseProxyData(zpo.HouseConfigType, zpo.ProxyName, zpo.ProxyLocation.ToVector());
				Print("[ZenProxyFramework] RPC_ReceiveZenProxyFrameworkConfig :: house=" + zpo.HouseConfigType + " loc=" + zpo.ProxyLocation + " proxy=" + zpo.ProxyName);
			}
		}
	}
}