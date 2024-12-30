modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ActionZenProxyTargetDebug, InputActionMap);
		AddAction(ActionZenSleepBedTest, InputActionMap);
		AddAction(ActionZenDrinkSinkTest, InputActionMap);
	}
}