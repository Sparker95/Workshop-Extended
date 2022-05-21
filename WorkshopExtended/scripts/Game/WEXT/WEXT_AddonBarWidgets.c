// Autogenerated by the Generate Class from Layout plugin, version 0.3.0
// Layout file: UI/AddonBar/AddonBar.layout

class WEXT_AddonBarWidgets
{
	static const ResourceName s_sLayout = "{E51B373A4E72E920}UI/AddonBar/AddonBar.layout";
	ResourceName GetLayout() { return s_sLayout; }

	HorizontalLayoutWidget m_AddonIconsLayout;

	ButtonWidget m_PresetsButton;
	SCR_ModularButtonComponent m_PresetsButtonComponent;

	TextWidget m_ModsCountText;

	bool Init(Widget root)
	{
		m_AddonIconsLayout = HorizontalLayoutWidget.Cast(root.FindWidget("HorizontalLayout.m_AddonIconsLayout"));

		m_PresetsButton = ButtonWidget.Cast(root.FindWidget("HorizontalLayout.m_PresetsButton"));
		m_PresetsButtonComponent = SCR_ModularButtonComponent.Cast(m_PresetsButton.FindHandler(SCR_ModularButtonComponent));

		m_ModsCountText = TextWidget.Cast(root.FindWidget("Frame0.m_ModsCountText"));

		return true;
	}
}
