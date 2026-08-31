class CfgPatches
{
	class ZenProxyFramework
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class ZenProxyFramework
	{
		dir = "ZenProxyFramework";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenProxyFramework";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenProxyFramework/scripts/3_game",
					"ZenProxyFramework/scripts/Common"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenProxyFramework/scripts/4_world",
					"ZenProxyFramework/scripts/Common"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenProxyFramework/scripts/5_mission",
					"ZenProxyFramework/scripts/Common"
				};
			};
		};
	};
};

class CfgVehicles
{
	// Base
	class Inventory_Base;
	class ZenProxyBase : Inventory_Base
	{
		scope = 2;
		model = "ZenProxyFramework\data\models\zen_invisibleproxy.p3d";
		physLayer = "item_small";
	};
	class Zen_StaticObjectProxyBase : ZenProxyBase
	{
		scope = 2;
	};

	// Proxies
	class ZenProxySinkTest : ZenProxyBase
	{
		scope = 2;
	};
};