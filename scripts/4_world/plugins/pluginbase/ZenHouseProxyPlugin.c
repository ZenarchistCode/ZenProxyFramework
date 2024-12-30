class ZenHouseProxyPlugin extends PluginBase
{
	// Is initialization complete?
	protected bool m_HasWorldInitializedServer;
	protected int m_BeginTimestamp;

	override void OnInit()
	{
		Print("[ZenHouseProxyPlugin] OnInit");

		if (GetGame().IsDedicatedServer())
		{
			m_BeginTimestamp = GetGame().GetTime();
			m_HasWorldInitializedServer = false;

			if (GetZenProxyFrameworkConfig().DumpProxies)
			{
				Print("[ZenHouseProxyPlugin] Dumping all unique building proxies to JSON, this process takes a few minutes...");
				GetZenProxyFrameworkConfig().HouseProxyData.Clear();
				GetZenProxyFrameworkConfig().Save();
			}
			else
			{
				Print("[ZenHouseProxyPlugin] Loading cached unique building proxies from JSON.");
				FinalizeInit();
			}
			
			return;
		}

		Print("[ZenHouseProxyPlugin] Client - loading house proxies whenever the houses spawn in.");
	}

	map<string, ref ZenProxyData> GetHouseProxyData()
	{
		return GetZenProxyFrameworkConfig().HouseProxyData;
	}

	string GetTargetedProxy(string houseType, vector localPos)
	{
		if (houseType == "")
		{
			return "NO HOUSE!";
		}

		ZenProxyData existingProxyData;
		if (!GetZenProxyFrameworkConfig().HouseProxyData.Find(houseType, existingProxyData))
			return "UNRECOGNIZED HOUSE: '" + houseType + "'";

		string proxyObject;
		existingProxyData.GetProxyLocations().Find(localPos.ToString(false), proxyObject);

		if (!proxyObject)
		{
			return "UNRECOGNIZED PROXY @ " + localPos;
		}

		return proxyObject;
	}

	void AddHouseProxyData(string houseType, string proxyName, vector proxyLocalPosition)
	{
		ZenProxyData existingProxyData;
		if (!GetZenProxyFrameworkConfig().HouseProxyData.Find(houseType, existingProxyData))
			existingProxyData = new ZenProxyData(proxyLocalPosition.ToString(false), proxyName);

		existingProxyData.GetProxyLocations().Set(proxyLocalPosition.ToString(false), proxyName);
		GetZenProxyFrameworkConfig().HouseProxyData.Set(houseType, existingProxyData);
	}

	bool HasLoadedProxiesFor(string type)
	{
		return GetZenProxyFrameworkConfig().HouseProxyData.Contains(type);
	}

	override void OnUpdate(float delta_time)
	{
#ifdef SERVER
		if (!m_HasWorldInitializedServer)
		{
			//! TODO: Is there a more robust way to detect when the game has fully loaded all objects in than checking if 30 seconds have passed since the last house spawned?
			if (House.LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP > 0 && GetGame().GetTime() - House.LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP >= 30000)
			{
				FinalizeInit();

				// Save proxies
				if (GetZenProxyFrameworkConfig().DumpProxies)
				{
					Print("[ZenHouseProxyPlugin] Saving proxies to JSON file...");
					GetZenProxyFrameworkConfig().DumpProxies = false;
					GetZenProxyFrameworkConfig().Save();
					Print("[ZenHouseProxyPlugin] Done.");
				}
			}
		}
#endif
	}

	void FinalizeInit()
	{
		int proxyCount = 0;
		for (int i = 0; i < GetZenProxyFrameworkConfig().HouseProxyData.Count(); i++)
		{
			proxyCount += GetZenProxyFrameworkConfig().HouseProxyData.GetElement(i).GetProxyLocations().Count();
		}

		proxyCount += AddCustomProxyOverrides();

		Print("[ZenHouseProxyPlugin] Init complete: Total unique proxy'd buildings=" + GetZenProxyFrameworkConfig().HouseProxyData.Count() + " (" + proxyCount + " proxy locations in memory) Time = " + ((GetGame().GetTime() - m_BeginTimestamp) / 1000) + " secs.");
		m_HasWorldInitializedServer = true;
	}

	int AddCustomProxyOverrides()
	{
		int proxyCount = 0;

		// Add proxy overrides to proxy map<>
		foreach(ZenProxyOverride proxyOverride : GetZenProxyFrameworkConfig().OverrideHouseProxyData)
		{
			AddHouseProxyData(proxyOverride.HouseType, proxyOverride.ProxyName, proxyOverride.ProxyLocation.ToVector());
			proxyCount++;
		}

		return proxyCount;
	}
}