class WEXT_ToolsDialog : SCR_ConfigurableDialogUi
{
	protected ref WEXT_ConfigurableDialog_ToolsWidgets widgets = new WEXT_ConfigurableDialog_ToolsWidgets();
	
	void WEXT_ToolsDialog()
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "tools", this);
	}
	
	override void OnMenuOpen(SCR_ConfigurableDialogUiPreset preset)
	{
		super.OnMenuOpen(preset);
		
		widgets.Init(GetRootWidget());
		
		widgets.m_GenerateDsJsonButtonComponent.m_OnClicked.Insert(OnGenerateDsJsonButton);
		widgets.m_GenerateCliButtonComponent.m_OnClicked.Insert(OnGenerateCliButton);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnGenerateDsJsonButton()
	{
		auto offlineAddons = SCR_AddonManager.GetInstance().GetOfflineAddons();
		array<ref SCR_WorkshopItem> enabledAddons = SCR_AddonManager.SelectItemsBasic(offlineAddons, EWorkshopItemQuery.ENABLED);
		
		for (int i = enabledAddons.Count() - 1; i >= 0; i--)
		{
			if (enabledAddons[i].GetId() == SCR_AddonManager.WEXT_GUID)
				enabledAddons.RemoveOrdered(i);
		}
		
		string s;
		
		if (!enabledAddons.IsEmpty())
		{
			s = s + GenerateAddonJson(enabledAddons[0]);	
			
			for (int i = 1; i < enabledAddons.Count(); i++)
				s = s + ",\n" + GenerateAddonJson(enabledAddons[i]);
		}
		
		new WEXT_DialogCopyToClipboard(s);
	}
	
	
	// puts quotes around a string
	static string qstr(string s) { return string.Format("\"%1\"", s); }
	
	//------------------------------------------------------------------------------------------------
	protected string GenerateAddonJson(SCR_WorkshopItem item)
	{
		/*
		{
        	"modId": "123",
        	"name": "SomeName",
        	"version": "1.2.3"
      	}
		*/
		return string.Format(
			"\t{\n\t\t\%1: %2,\n\t\t%3: %4,\n\t\t%5: %6\n\t}",
			qstr("modId"), qstr(item.GetId()),
			qstr("name"), qstr(item.GetName()),
			qstr("version"), qstr(item.GetCurrentLocalVersion()) );
	}
	
	
	//------------------------------------------------------------------------------------------------
	protected void OnGenerateCliButton()
	{
		string cli = "-addons ";
		
		auto offlineAddons = SCR_AddonManager.GetInstance().GetOfflineAddons();
		array<ref SCR_WorkshopItem> enabledAddons = SCR_AddonManager.SelectItemsBasic(offlineAddons, EWorkshopItemQuery.ENABLED);
		
		for (int i = enabledAddons.Count() - 1; i >= 0; i--)
		{
			if (enabledAddons[i].GetId() == SCR_AddonManager.WEXT_GUID)
				enabledAddons.RemoveOrdered(i);
		}
		
		if (!enabledAddons.IsEmpty())
		{
			cli = cli + enabledAddons[0].GetId();
			for (int i = 1; i < enabledAddons.Count(); i++)
				cli = cli + "," + enabledAddons[i].GetId();
		}
		
		new WEXT_DialogCopyToClipboard(cli);
	}
}