//! Used for debugging - type anything ingame to get player's position in-game relative to house for placing proxy's
modded class ChatInputMenu
{
    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (GetZenProxyFrameworkConfig().DebugOn)
        {
            if (finished)
            {
                m_close_timer.Run(0.1, this, "Close");
                GetUApi().GetInputByID(UAPersonView).Supress();

                PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
                BuildingSuper nearestBuilding = GetBuildingBelowPlayer(player);

                if (nearestBuilding)
                {
                    // Calculate the map position offset by converting world position to model position
                    vector mapPosOffset = nearestBuilding.WorldToModel(player.GetPosition());
                    GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Pos for " + nearestBuilding.GetType() + ": " + mapPosOffset, ""));
                    GetGame().CopyToClipboard(mapPosOffset.ToString(false));
                }
                else
                {
                    GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "NO BUILDING DETECTED!", ""));
                }

                return true;
            }
        }

        return super.OnChange(w, x, y, finished);
    }

    BuildingSuper GetBuildingBelowPlayer(PlayerBase player)
    {
        for (int i = 0; i < 4; i++)
        {
            // Get player's world position and create a downward ray start and end point
            vector start = player.GetPosition() + "0 0.3 0";
            vector end = start + "0 0 100";

            if (i == 0)
                end = start + "0 0 100";
            else if (i == 1)
                end = start + "100 0 0";
            else if (i == 2)
                end = start + "0 100 0";
            else if (i == 3)
                end = start + "0 -100 0";

            // Perform the raycast
            vector contactPos;
            vector contactDir;
            int contactComponent;
            set<Object> hitObjects = new set<Object>();

            DayZPhysics.RaycastRV(start, end, contactPos, contactDir, contactComponent, hitObjects, NULL, player, false, false, ObjIntersectGeom, 0.1);

            // Check the objects hit by the raycast
            foreach(Object obj : hitObjects)
            {
                // Cast the object to BuildingSuper to check if it's a building
                BuildingSuper building = BuildingSuper.Cast(obj);
                if (building)
                {
                    return building;  // Return the building the player is standing in
                }
            }
        }

        // Return null if no building is detected
        return NULL;
    }
}
