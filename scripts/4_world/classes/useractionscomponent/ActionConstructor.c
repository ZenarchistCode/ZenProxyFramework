modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(ActionZenProxyTargetDebug);
		actions.Insert(ActionZenSleepBedTest);
		actions.Insert(ActionZenRepairSinkTest);
		actions.Insert(ActionZenDrinkSinkTest);
		actions.Insert(ActionZenFillBottleBaseSinkTest);
	}
}