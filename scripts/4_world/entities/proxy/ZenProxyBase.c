class ZenProxyBase extends ItemBase 
{
	override void DeferredInit()
	{
		super.DeferredInit();

		SetAllowDamage(false);
		SetTakeable(false);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}
}