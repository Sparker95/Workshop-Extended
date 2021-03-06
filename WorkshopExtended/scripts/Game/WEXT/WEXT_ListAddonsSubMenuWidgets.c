// Autogenerated by the Generate Class from Layout plugin, version 0.3.0
// Layout file: UI/AddonListTab/ListAddonsSubMenu.layout

class WEXT_ListAddonsSubMenuWidgets
{
	static const ResourceName s_sLayout = "{0FFE02F22D314775}UI/AddonListTab/ListAddonsSubMenu.layout";
	ResourceName GetLayout() { return s_sLayout; }

	SizeLayoutWidget m_DisabledAddonsPanel;

	ScrollLayoutWidget m_DisabledAddonsScroll;

	VerticalLayoutWidget m_DisabledAddonsList;

	SizeLayoutWidget m_EnabledAddonsPanel;

	ButtonWidget m_ToolsButton;
	SCR_ModularButtonComponent m_ToolsButtonComponent;

	ScrollLayoutWidget m_EnabledAddonsScroll;

	VerticalLayoutWidget m_EnabledAddonsList;

	OverlayWidget m_AddonInfoPanel;
	SCR_AddonDetailsPanelComponent m_AddonInfoPanelComponent;

	bool Init(Widget root)
	{
		m_DisabledAddonsPanel = SizeLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_DisabledAddonsPanel"));

		m_DisabledAddonsScroll = ScrollLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_DisabledAddonsPanel.VerticalLayout0.Content.Scroll"));

		m_DisabledAddonsList = VerticalLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_DisabledAddonsPanel.VerticalLayout0.Content.Scroll.m_AddonsList"));

		m_EnabledAddonsPanel = SizeLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_EnabledAddonsPanel"));

		m_ToolsButton = ButtonWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_EnabledAddonsPanel.VerticalLayout0.TopBarHeight.TopBar.Content.HorizontalLayout0.m_ToolsButton"));
		m_ToolsButtonComponent = SCR_ModularButtonComponent.Cast(m_ToolsButton.FindHandler(SCR_ModularButtonComponent));

		m_EnabledAddonsScroll = ScrollLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_EnabledAddonsPanel.VerticalLayout0.Content.Scroll"));

		m_EnabledAddonsList = VerticalLayoutWidget.Cast(root.FindWidget("Size.PanelsHorizontal.m_EnabledAddonsPanel.VerticalLayout0.Content.Scroll.m_AddonsList"));

		m_AddonInfoPanel = OverlayWidget.Cast(root.FindWidget("Size.PanelsHorizontal.RightPanel.VerticalLayout0.Content.m_AddonInfoPanel"));
		m_AddonInfoPanelComponent = SCR_AddonDetailsPanelComponent.Cast(m_AddonInfoPanel.FindHandler(SCR_AddonDetailsPanelComponent));

		return true;
	}
}
