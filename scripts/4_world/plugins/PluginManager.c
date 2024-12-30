modded class PluginManager
{
	override void Init()
	{
		super.Init();

		// Register on both client & server
		RegisterPlugin("ZenHouseProxyPlugin", true, true);
	}
}