class WEXT_ListAddonsSubmenu : SCR_SubMenuBase
{
	protected ref WEXT_ListAddonsSubMenuWidgets widgets = new WEXT_ListAddonsSubMenuWidgets();
	
	
	//------------------------------------------------------------------------------------------------
	//! Refreshes all lists
	protected void RefreshAll()
	{
		// Get offline items from API
		array<WorkshopItem> rawWorkshopItems = new array<WorkshopItem>;
		GetGame().GetBackendApi().GetWorkshop().GetOfflineItems(rawWorkshopItems);
		
		// Register items in Addon Manager
		array<ref SCR_WorkshopItem> itemsRegistered = new array<ref SCR_WorkshopItem>;
		foreach (auto i : rawWorkshopItems)
		{
			SCR_WorkshopItem itemRegistered = SCR_AddonManager.GetInstance().Register(i);
			itemsRegistered.Insert(itemRegistered);
		}
		
		// Convert to basic array for sorting...
		array<SCR_WorkshopItem> itemsWeakPtrs = {};
		foreach (auto i : itemsRegistered)
			itemsWeakPtrs .Insert(i);
		
		// Sort by name...
		SCR_Sorting<SCR_WorkshopItem, WEXT_CompareWorkshopItemName>.HeapSort(itemsWeakPtrs );
		
		// Convert back to array<ref ...>
		array<ref SCR_WorkshopItem> itemsSorted = {};
		foreach (auto i : itemsWeakPtrs)
			itemsSorted.Insert(i);
		
		auto enabledItems = SCR_AddonManager.SelectItemsBasic(itemsSorted, EWorkshopItemQuery.ENABLED);
		auto disabledItems = SCR_AddonManager.SelectItemsBasic(itemsSorted, EWorkshopItemQuery.NOT_ENABLED);
		
		CreateListLines(widgets.m_EnabledAddonsList, widgets.m_EnabledAddonsScroll, enabledItems);
		CreateListLines(widgets.m_DisabledAddonsList, widgets.m_DisabledAddonsScroll, disabledItems);
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	protected void CreateListLines(VerticalLayoutWidget vLayout, ScrollLayoutWidget scroll, array<ref SCR_WorkshopItem> items)
	{
		// Record scroll pos
		float scrollx, scrolly;
		scroll.GetSliderPos(scrollx, scrolly);
		
		// Delete previous lines
		while (vLayout.GetChildren())
		{
			vLayout.RemoveChild(vLayout.GetChildren());
		}
		
		//for (int j = 0; j < 3; j++)
		//{
		foreach (SCR_WorkshopItem item : items)
		{
			// Don't add ourselves to the mod list, it makes no sense
			if (item.GetId() == SCR_AddonManager.WEXT_GUID)
				continue;
			
			Widget w = GetGame().GetWorkspace().CreateWidgets("{1E9609F84FF1BF73}UI/WEXT_AddonLine.layout", vLayout);
			WEXT_AddonLineComponent comp = WEXT_AddonLineComponent.Cast(w.FindHandler(WEXT_AddonLineComponent));
			comp.Init(item);
			comp.m_OnEnableButton.Insert(OnEnableButton);
			comp.m_OnDisableButton.Insert(OnDisableButton);
			comp.m_OnMouseEnter.Insert(OnLineMouseEnter);
			comp.m_OnMouseLeave.Insert(OnLineMouseLeave);
		}
		//}
		
		// Restore scroll pos
		scroll.SetSliderPos(scrollx, scrolly);
	}
	
	
	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuOpen(parentMenu);
	
		widgets.Init(GetRootWidget());
		
		widgets.m_ToolsButtonComponent.m_OnClicked.Insert(OnToolsButton);
		
		// Subscribe to addon manager events
		SCR_AddonManager.GetInstance().m_OnAddonOfflineStateChanged.Insert(Callback_OnAddonOfflineStateChanged);
		
		RefreshAll();
		
		SCR_AddonManager.GetInstance().m_OnAddonsEnabledChanged.Insert(Callback_OnAddonEnabledStateChanged);
	}
	
	
	//------------------------------------------------------------------------------------------------
	override void OnMenuClose(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuClose(parentMenu);
		
		// Unsubscribe from addon manager events
		SCR_AddonManager.GetInstance().m_OnAddonOfflineStateChanged.Remove(Callback_OnAddonOfflineStateChanged);
		SCR_AddonManager.GetInstance().m_OnAddonsEnabledChanged.Remove(Callback_OnAddonEnabledStateChanged);
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	protected void OnLineMouseEnter(WEXT_AddonLineComponent comp)
	{
		auto item = comp.GetWorkshopItem();
		widgets.m_AddonInfoPanelComponent.SetWorkshopItem(item);
	}
	
	
	//------------------------------------------------------------------------------------------------
	protected void OnLineMouseLeave(WEXT_AddonLineComponent comp)
	{
		// Since this event is handled not natively, but it scripted, it is buggy, so 
		// OnMouseLeave can be called before OnMouseEnter. So we don't erase data from info
		// panel.
		//widgets.m_AddonInfoPanelComponent.SetWorkshopItem(null);
	}	
	
	
	
	
	
	
	//------------------------------------------------------------------------------------------------
	// B U T T O N S
	
	
	//------------------------------------------------------------------------------------------------
	protected void OnEnableButton(WEXT_AddonLineComponent comp)
	{
		SCR_WorkshopItem item = comp.GetWorkshopItem();
		item.SetEnabled(true);
		//RefreshAll();
	}
	
	
	//------------------------------------------------------------------------------------------------
	protected void OnDisableButton(WEXT_AddonLineComponent comp)
	{
		SCR_WorkshopItem item = comp.GetWorkshopItem();
		item.SetEnabled(false);
		//RefreshAll();
	}	
	
	protected void OnToolsButton()
	{
		new WEXT_ToolsDialog();
	}
	
	
	//------------------------------------------------------------------------------------------------
	// O T H E R
	
	
	
	//------------------------------------------------------------------------------------------------
	//! Called by SCR_AddonManager when some addon is downloaded or uninstalled
	protected void Callback_OnAddonOfflineStateChanged(SCR_WorkshopItem item, bool newState)
	{
		// Some addon was installed or uninstalled, we must refresh the page
		RefreshAll();
	}
	
	protected void Callback_OnAddonEnabledStateChanged()
	{
		// Something got enabled or disabled, refresh the page
		RefreshAll();
	}
}


//------------------------------------------------------------------------------------------------
class WEXT_CompareWorkshopItemName : SCR_SortCompare<SCR_WorkshopItem>
{
	override static int Compare(SCR_WorkshopItem left, SCR_WorkshopItem right)
	{
		string name1 = left.GetName();
		string name2 = right.GetName();
		
		if (name1.Compare(name2) == -1)
			return 1;
		else
			return 0;
	}
}