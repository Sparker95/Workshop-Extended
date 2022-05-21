modded class ContentBrowserUI
{
	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		// A bit of a hacky way to add a new tab to layout:
		// This should be possible to do through modding layout file, but atm it doesn't work for some reason,
		// Should figure out later what's happening here.
		m_TabViewComponent.AddTab(
			"{0FFE02F22D314775}UI/WEXT_ListAddonsSubMenu.layout",
			"Downloaded List",
			enabled: true);
	}
}