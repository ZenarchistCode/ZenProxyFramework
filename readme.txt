[h1]What Is This?[/h1]

This mod is a framework mod. It adds the ability to implement actions on proxy objects in buildings (eg. on house furniture).

It is heavily inspired by the work of Hunterz, Zedmag and Ujniher's "Search For Loot" mod https://steamcommunity.com/sharedfiles/filedetails/?id=3200745813. Shout-out to their awesome work.

Unfortunately because their mod does not have a #define declared, my mod is not compatible with theirs as some code conflicts with the scripted vanilla houses.

I learned the method for identifying proxy objects through studying their code, but my framework mod is written from scratch with the intention of creating a generic codebase for modders to add their own custom actions to in-game proxy objects.

Some examples of what's possible include:
Adding ability to sleep emote on beds or sit on chairs
Adding ability to repair and drink from sinks
Adding ability to search bookcases/fridges/etc for items

Those are the examples included in my basic proxy action template mod, but you can add any action you want to pretty much any proxy object in houses (with a few rare exceptions).

The mod has a debug mode in which you can press "F" on various objects to get their proxy name (the name is copied to your clipboard). Then to add an action to it, just follow the template code provided.

It will require some intermediate level modding experience and skills to figure out how to add your own actions to things.

I made the framework as intuitive and simple as possible, but if you're new to DayZ modding then you might need to reach out to someone for help (unfortunately I don't have the time to help everyone who messages me - but there will be plenty of modders out there who will know how to add in-game actions using this framework so just ask for help on the various community discords).

A template mod's source code is included in the "extras" folder for reference, and is a mod I created for use on my own server. The framework itself contains some example templates for adding actions to things in the code as well.

There's a video guide on my YouTube channel explaining how the mod works and how to add custom actions.

[h1]Installation Instructions:[/h1]

Install this mod like any other mod - copy it into your server folder and add it to your mods list. Make sure to copy the .bikey into your server keys if you're not using a server management tool like OmegaManager which does that automatically.

This mod must be run on both client & server so make sure to repack it into your server mod or add it to your modlist to use it.

The framework comes with an example .pbo file included for adding some basic proxy actions - repack that pbo file into your server modpack if you want to add some actions to your server but don't know how to.

Alternatively, if you don't know how to repack mods but still want some basic actions added to furniture, I published the template mod to the workshop here: https://steamcommunity.com/sharedfiles/filedetails/?id=3395567006

Don't forget to copy the types.xml entries if you want to enable persistence on the repaired sinks/searched objects.

[h1]Repack & Source Code:[/h1]

You can repack this mod if you like, and do anything else you want with it for that matter. The source code is on my GitHub at www.zenarchist.io

[h1]Buy Me A Coffee:[/h1]

All my mods are free and open source, but it takes an enormous amount of time to put some of these mods together. If I've helped you out, please consider helping me buy my next coffee! I don't expect it, but I very much appreciate it.

https://buymeacoffee.com/zenarchist

Enjoy!
