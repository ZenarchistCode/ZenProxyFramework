modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenProxyFrameworkConfig", new Param2<bool, ref array<ref ZenProxyOverride>>(GetZenProxyFrameworkConfig().DebugOn, GetZenProxyFrameworkConfig().GetOverrideHouseProxyData()), true, identity);
	}

	override void OnInit()
	{
		super.OnInit();

		GetZenProxyFrameworkConfig();

		if (!GetZenProxyFrameworkConfig().DebugOn)
			return;

		string worldName = "empty";
		GetGame().GetWorldName(worldName);
		worldName.ToLower();

		if (worldName == "enoch")
		{
			// Spawn testing houses on airfield
			SpawnDebugHouseObject("Land_House_1W01", "3524.451172 241.216797 9889.212891", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W02", "3535.295654 240.406097 9900.393555", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W03", "3548.927979 240.701569 9915.047852", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W04", "3562.295166 240.791092 9926.206055", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W05", "3571.406982 241.156036 9937.050781", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W05_Yellow", "3583.044189 241.156036 9947.070313", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W06", "3596.357422 241.213409 9957.073242", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W07", "3612.094727 241.957779 9972.213867", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W08", "3624.059082 241.057602 9988.176758", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W08_Brown", "3634.044922 241.062042 9998.740234", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W09", "3648.345947 240.680527 10012.083984", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W09_Yellow", "3658.151855 240.680481 10023.395508", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W10", "3666.380127 240.505157 10035.711914", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W10_Brown", "3676.513184 240.963776 10045.212891", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W11", "3691.669434 241.365097 10056.486328", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W12", "3701.905518 241.045929 10069.818359", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_1W12_Brown", "3714.150635 241.045944 10085.647461", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2B01", "3733.332520 242.761871 10098.455078", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2B02", "3754.278320 245.527557 10110.687500", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2B03", "3775.658691 245.588745 10128.453125", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2B04", "3795.408203 245.289627 10151.136719", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W01", "3812.078125 242.488876 10174.457031", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W02", "3819.768066 243.166885 10187.283203", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W03", "3839.847168 244.312042 10202.766602", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W03_Brown", "3854.999268 244.180969 10217.230469", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W04", "3874.868164 244.323395 10231.517578", "0.000000 -0.000000 -0.000000", 1);
			SpawnDebugHouseObject("Land_House_2W04_Yellow", "3887.105469 244.111435 10248.433594", "0.000000 -0.000000 -0.000000", 1);
		}
	}
};

static Object SpawnDebugHouseObject(string type, vector position, vector orientation, float scale = 1.0)
{
	Object obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
	if (!obj) {
		Error("Failed to create object " + type);
		return null;
	}

	obj.SetPosition(position);
	obj.SetOrientation(orientation);
	obj.SetOrientation(obj.GetOrientation());
	obj.SetScale(scale);
	obj.Update();
	obj.SetAffectPathgraph(true, false);
	if (obj.CanAffectPathgraph()) {
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
	}

	return obj;
}
