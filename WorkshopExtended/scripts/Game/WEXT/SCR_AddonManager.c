// We add more functionality to SCR_AddonManager.
// AddonManager is an entity placed into main world, so it's perfect for our purpose,
// Because it exists only in main menu.
modded class SCR_AddonManager
{
	const string WEXT_GUID = "5708ADF3AD12AC93";
	protected ref WEXT_Storage m_Storage;
	
	ref ScriptInvoker m_OnAddonsEnabledChanged = new ref ScriptInvoker; // () - Called wherever set of enabled addons has changed
	
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	WEXT_Storage GetPresetStorage()
	{
		return s_Instance.m_Storage;
	}
	
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// Looks up addon from internal map
	SCR_WorkshopItem GetItem(string id)
	{
		return m_mItems.Get(id);
	}
	
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// Enables addons from preset, disables all addons from other presets
	void SelectPreset(WEXT_AddonPreset preset, notnull array<ref WEXT_AddonMeta> addonsNotFound)
	{	
		// Disable all addons except for ourselves :)
		foreach (SCR_WorkshopItem i : GetOfflineAddons())
		{
			if (i.GetId() == WEXT_GUID)
				continue;
			
			i.SetEnabled(false);
		}
		
		
		array<ref WEXT_AddonMeta> enabledAddons = preset.GetAddons();
		foreach (WEXT_AddonMeta meta : enabledAddons)
		{
			string guid = meta.GetGuid();
			SCR_WorkshopItem item = GetItem(guid);
			
			// Didn't find this addon
			if (item)
				item.SetEnabled(true);
			else
				addonsNotFound.Insert(meta);
		}
	}
	
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	WEXT_AddonPreset CreatePresetFromEnabledAddons(string presetName)
	{		
		// Get GUIDs of addons which are enabled
		
		array<ref SCR_WorkshopItem> enabledAddons = SCR_AddonManager.SelectItemsBasic(GetOfflineAddons(), EWorkshopItemQuery.ENABLED);
		
		array<ref WEXT_AddonMeta> addonsMeta = {};
		foreach (SCR_WorkshopItem item : enabledAddons)
		{
			string guid = item.GetId();
			if (guid == WEXT_GUID) // Ignore ourselves
				continue;
			
			addonsMeta.Insert((new WEXT_AddonMeta()).Init(guid, item.GetName()) );
		}
		
		if (addonsMeta.IsEmpty())
			return null;
		
		WEXT_AddonPreset preset = (new WEXT_AddonPreset()).Init(presetName, addonsMeta);
		
		return preset;
	}
	
	
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if(GetGame().InPlayMode())
		{
			m_Storage = new WEXT_Storage();
		}
	}
	
	protected float m_fAddonsEnabledTimer = 0;
	protected string m_sAddonsEnabledPrev;
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		
		
		// Lazy detection of case when addons get enabled/disabled
		// Iterate all addons and invoke event whenever current list of enabled addons is different from prev. list
		// It's a really bad implementation but it's fine in main menu
		m_fAddonsEnabledTimer += timeSlice;
		if (m_fAddonsEnabledTimer > 1/30)
		{
			string addonsEnabled;
			foreach (string id, SCR_WorkshopItem item : m_mItems)
			{
				if (item.GetEnabled())
					addonsEnabled = addonsEnabled + id + " ";
			}
			
			if (addonsEnabled != m_sAddonsEnabledPrev)
			{
				m_OnAddonsEnabledChanged.Invoke();
				m_sAddonsEnabledPrev = addonsEnabled;
			}
			
			m_fAddonsEnabledTimer = 0;
		}
	}
}