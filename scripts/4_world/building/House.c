modded class House
{
	static int LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP;
	protected static ref ZenHouseProxyPlugin m_ZenHouseProxyPlugin;

	// These particular proxies don't auto-load for some reason - can't figure out why, so in the meantime here are some common static overrides
	static ref array<ref ZenProxyOverride> Static_OverrideHouseProxyData =
	{
		new ZenProxyOverride("Land_Rail_Station_Small", "-4.745543 -0.213070 1.197218", "sodamachine"),
		new ZenProxyOverride("Land_FuelStation_Build_Enoch", "-1.471846 -0.976302 -0.198597", "cashier"),
		new ZenProxyOverride("Land_Workshop_FuelStation", "3.363530 -0.688908 -0.958930", "cashier"),
		new ZenProxyOverride("Land_Workshop_FuelStation", "-4.885592 -1.269217 4.953902", "workbench_tools"),
		new ZenProxyOverride("Land_Village_Store_Enoch", "1.224203 -1.606711 0.924272", "cashier"),
		new ZenProxyOverride("Land_City_FireStation", "-10.626365 6.745750 -3.564241", "sodamachine"),
		new ZenProxyOverride("Land_City_FireStation", "-11.067739 6.483905 3.357783", "sink"),
		new ZenProxyOverride("Land_City_FireStation", "-11.761846 6.483905 3.357783", "sink"),
		new ZenProxyOverride("Land_City_FireStation", "-12.469562 6.483905 3.357783", "sink"),
		new ZenProxyOverride("Land_City_PoliceStation", "-1.082789 -5.348440 3.436189", "sodamachine"),
		new ZenProxyOverride("Land_City_PoliceStation_Enoch", "-1.082789 -5.348440 3.436189", "sodamachine"),
		new ZenProxyOverride("Land_House_1W10", "-2.855497 -1.184504 -3.792669", "radio_b"),
		new ZenProxyOverride("Land_City_Hospital", "8.997766 -6.313595 -0.143953", "sodamachine"),
		new ZenProxyOverride("Land_Hotel_Damaged", "4.207494 -19.598429 12.164265", "sodamachine"),
		new ZenProxyOverride("Land_City_Store", "-5.472626 -0.581923 0.873293", "cashier"),
		new ZenProxyOverride("Land_City_Store", "-5.472919 -0.581923 3.165845", "cashier"),
		new ZenProxyOverride("Land_City_Store_WithStairs", "-4.122752 -0.576916 0.046144", "cashier"),
		new ZenProxyOverride("Land_City_Store_WithStairs", "-4.123045 -0.576916 2.338696", "cashier"),
		new ZenProxyOverride("Land_Office2", "-18.475609 -0.500156 8.031333", "sodamachine")
		//! ^ LIST UPDATED 4th December, 2024
	};

	void House()
	{
		if (GetGame().IsDedicatedServer() && !GetZenProxyFrameworkConfig().DumpProxies)
			return;

		SetupZenHouseProxy();
	}

	protected void SetupZenHouseProxy()
	{
		if (!m_ZenHouseProxyPlugin)
			m_ZenHouseProxyPlugin = ZenHouseProxyPlugin.Cast(GetPlugin(ZenHouseProxyPlugin));

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenDumpProxyData, 1000, false);
	}

	protected void ZenDumpProxyData()
	{
		if (m_ZenHouseProxyPlugin.HasLoadedProxiesFor(GetType()))
			return;

		LOD geometryLod = GetLODByName(LOD.NAME_GEOMETRY);
		if (!geometryLod)
			return;

		// Get object's direct memory LODs/memory points
		array<string> selectionsList = new array<string>;
		GetSelectionList(selectionsList);

		// Add Geometry LOD selections to list
		array<Selection> geoSelections = new array<Selection>();
		geometryLod.GetSelections(geoSelections);
		foreach (Selection selectionObj : geoSelections)
		{
			if (selectionObj.GetName().Contains("."))
				selectionsList.Insert(selectionObj.GetName());
		}

		if (selectionsList.Count() == 0)
			return;

		string type = GetType();
		type.ToLower();

		int scannedProxies = 0;
		string proxyName;
		vector proxyLocalSpace;
		vector proxyWorldSpace;
		array<string> selectionList = new array<string>;

		// Search geometry LOD selections first
		foreach (Selection selection : geoSelections)
		{
			string selectionName = selection.GetName();
			selectionName.ToLower();

			proxyLocalSpace = GetSelectionPositionLS(selectionName);

			// Valid selection looks like this: 'proxy:\dz\structures\furniture\kitchen\fridge\fridge.001'
			if (!selectionName.Contains("."))
				continue;

			// Easiest way to get final selection object name is to split the filepath by \ and test for . in proxyname
			selectionName.Split("\\", selectionList);

			foreach (string subselection : selectionList)
			{
				if (!subselection.Contains("."))
					continue;

				proxyName = subselection.Substring(0, subselection.IndexOf("."));
				proxyWorldSpace = GetSelectionPositionWS(selectionName);

				m_ZenHouseProxyPlugin.AddHouseProxyData(GetType(), proxyName, proxyLocalSpace);
				scannedProxies++;
			}
		}

		foreach (ZenProxyOverride proxyOverride : Static_OverrideHouseProxyData)
		{
			if (proxyOverride.HouseType == GetType())
			{
				m_ZenHouseProxyPlugin.AddHouseProxyData(GetType(), proxyOverride.ProxyName, proxyOverride.ProxyLocation.ToVector());
				scannedProxies++;
			}
		}

		// Add the custom override proxies if client-side (server-side handles this in the plugin after loading json)
		/*if (GetGame().IsClient())
		{
			foreach (ZenProxyOverride proxyOverride : GetZenProxyFrameworkConfig().OverrideHouseProxyData)
			{
				if (proxyOverride.HouseType == GetType())
				{
					m_ZenHouseProxyPlugin.AddHouseProxyData(GetType(), proxyOverride.ProxyName, proxyOverride.ProxyLocation.ToVector());
					scannedProxies++;
				}
			}
		}*/

		if (scannedProxies > 0 && GetGame().IsDedicatedServer() && GetZenProxyFrameworkConfig().DebugOn)
			Print("[ZenHouseProxyPlugin] Scanned " + scannedProxies + " proxies for: " + GetType());

		LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP = GetGame().GetTime();
	}

	static int GetLastZenProxySpawnTime()
	{
		return GetGame().GetTime() - House.LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP;
	}
}