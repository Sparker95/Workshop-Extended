// Autogenerated by the Generate Class from Layout plugin, version 0.3.0
// Layout file: UI/AddonBarIcon.layout

class WEXT_AddonBarIconWidgets
{
	static const ResourceName s_sLayout = "{9B80BD4A534C651C}UI/AddonBarIcon.layout";
	ResourceName GetLayout() { return s_sLayout; }

	OverlayWidget m_BackendImage;
	SCR_WorkshopItemBackendImageComponent m_BackendImageComponent;

	bool Init(Widget root)
	{
		m_BackendImage = OverlayWidget.Cast(root.FindWidget("Size.m_BackendImage"));
		m_BackendImageComponent = SCR_WorkshopItemBackendImageComponent.Cast(m_BackendImage.FindHandler(SCR_WorkshopItemBackendImageComponent));

		return true;
	}
}