class ZenProxyFrameworkConfig
{
	// Config location
	private const static string zenModRootFolder = "$profile:\\Zenarchist\\";
	private const static string zenModFolder = zenModRootFolder + "Utilities\\";
	private const static string zenConfigName = "ZenProxyFrameworkConfig.json";
	private const static string CURRENT_VERSION = "1";
	string CONFIG_VERSION = "";

	// Config
	bool DebugOn = false;
	bool DumpProxies = true;

	// Configurable list of proxy overrides which syncs to the client
	ref array<ref ZenProxyOverride> OverrideHouseProxyData = new array<ref ZenProxyOverride>;

	// HOUSE CLASSNAME, PROXY DATA
	ref map<string, ref ZenProxyData> HouseProxyData = new map<string, ref ZenProxyData>;
	string MapName = "";

	void Load()
	{
		string mapNameLocal = GetGame().GetWorldName();

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenProxyFrameworkConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				// Mismatch, back up old version
				JsonFileLoader<ZenProxyFrameworkConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				bool mapMismatch = false;
				if (MapName != mapNameLocal)
					mapMismatch = true;

				if (mapMismatch)
				{
					Print("[ZenProxyFramework] Worlds mismatch: '" + mapNameLocal + "' != '" + MapName + "': renewing proxy dump.");
					HouseProxyData.Clear();
					DumpProxies = true;
				}
				else
				{
					// Config exists and version matches
					return;
				}

			}
		}

		CONFIG_VERSION = CURRENT_VERSION;

		if (MapName == "")
		{
			OverrideHouseProxyData.Insert(new ZenProxyOverride("Example_Building_Name", "-4.745543 -0.213070 1.197218", "sodamachine"));
		}

		MapName = mapNameLocal;

		Save();
	}

	void Save()
	{
		if (!FileExist(zenModRootFolder))
		{
			MakeDirectory(zenModRootFolder);
		}

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenProxyFrameworkConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	//! Because I have an example entry in the default config, we don't really want to sync that to the client unnecessarily
	//! so if that's the ONLY member of the array, just return a NULL array to be sent to the client.
	array<ref ZenProxyOverride> GetOverrideHouseProxyData()
	{
		if (OverrideHouseProxyData.Count() == 1 && OverrideHouseProxyData.Get(0).HouseType == "Example_Building_Name")
			return NULL;

		return OverrideHouseProxyData;
	}
}

class ZenProxyOverride
{
	string HouseType;
	string ProxyLocation;
	string ProxyName;

	void ZenProxyOverride(string house, string loc, string name)
	{
		HouseType = house;
		ProxyLocation = loc;
		ProxyName = name;
	}
};

class ZenProxyData
{
	// string = vector (we can't use vectors for map key/elements)
	protected ref map<string, string> ProxyLocations;

	void ZenProxyData(string pos, string name)
	{
		ProxyLocations = new map<string, string>;
		ProxyLocations.Set(pos, name);
	}

	map<string, string> GetProxyLocations()
	{
		return ProxyLocations;
	}
}

ref ZenProxyFrameworkConfig m_ZenProxyFrameworkConfig;

static ZenProxyFrameworkConfig GetZenProxyFrameworkConfig()
{
	if (!m_ZenProxyFrameworkConfig)
	{
		m_ZenProxyFrameworkConfig = new ZenProxyFrameworkConfig;

		if (GetGame().IsDedicatedServer())
			m_ZenProxyFrameworkConfig.Load();
	}

	return m_ZenProxyFrameworkConfig;
}