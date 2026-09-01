modded class House
{
	static int LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP;
	protected static ref ZenHouseProxyPlugin m_ZenHouseProxyPlugin;

	// These particular proxies don't auto-load for some reason.
	// Keep common static overrides here.
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
	};

	void House()
	{
		// Server only needs to inspect building models while generating
		// the proxy cache. During normal operation it loads the JSON cache.
		if (g_Game.IsDedicatedServer() && !GetZenProxyFrameworkConfig().DumpProxies)
			return;

		SetupZenHouseProxy();
	}

	protected void SetupZenHouseProxy()
	{
		if (!m_ZenHouseProxyPlugin)
			m_ZenHouseProxyPlugin = ZenHouseProxyPlugin.Cast(GetPlugin(ZenHouseProxyPlugin));

		if (!m_ZenHouseProxyPlugin)
			return;

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenDumpProxyData, 1000, false);
	}

	protected void ZenDumpProxyData()
	{
		if (!m_ZenHouseProxyPlugin)
			return;

		string houseType = GetType();

		// We only need to scan one instance of each building classname.
		if (m_ZenHouseProxyPlugin.HasLoadedProxiesFor(houseType))
			return;

		LOD geometryLod = GetLODByName(LOD.NAME_GEOMETRY);
		if (!geometryLod)
			return;

		array<string> selectionsList = new array<string>;
		GetSelectionList(selectionsList);

		array<Selection> geoSelections = new array<Selection>;
		geometryLod.GetSelections(geoSelections);

		foreach (Selection geometrySelection : geoSelections)
		{
			string geometrySelectionName = geometrySelection.GetName();

			if (geometrySelectionName.Contains("."))
				selectionsList.Insert(geometrySelectionName);
		}

		if (selectionsList.Count() == 0)
			return;

		int scannedProxies = 0;

		foreach (Selection selection : geoSelections)
		{
			string selectionName = selection.GetName();
			selectionName.ToLower();

			if (!selectionName.Contains("."))
				continue;

			vector proxyLocalSpace = GetSelectionPositionLS(selectionName);

			array<string> selectionPathParts = new array<string>;
			selectionName.Split("\\", selectionPathParts);

			foreach (string selectionPathPart : selectionPathParts)
			{
				if (!selectionPathPart.Contains("."))
					continue;

				int dotIndex = selectionPathPart.IndexOf(".");
				if (dotIndex <= 0)
					continue;

				string proxyName = selectionPathPart.Substring(0, dotIndex);

				m_ZenHouseProxyPlugin.AddHouseProxyData(houseType, proxyName, proxyLocalSpace);
				scannedProxies++;

				break;
			}
		}

		foreach (ZenProxyOverride proxyOverride : Static_OverrideHouseProxyData)
		{
			if (proxyOverride.HouseConfigType != houseType)
				continue;

			m_ZenHouseProxyPlugin.AddHouseProxyData(houseType, proxyOverride.ProxyName, proxyOverride.ProxyLocation.ToVector());
			scannedProxies++;
		}

		if (scannedProxies > 0 && g_Game.IsDedicatedServer() && GetZenProxyFrameworkConfig().DebugOn)
			ZenFunctions.DebugMessage("[ZenHouseProxyPlugin] Scanned " + scannedProxies + " proxies for: " + houseType);

		LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP = g_Game.GetTime();
	}

	static int GetLastZenProxySpawnTime()
	{
		return g_Game.GetTime() - LAST_ZENPROXY_HOUSE_SPAWN_TIMESTAMP;
	}
}