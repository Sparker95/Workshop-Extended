class WEXT_AddonBarIconComponent : ScriptedWidgetComponent
{
	protected ref WEXT_AddonBarIconWidgets widgets = new WEXT_AddonBarIconWidgets();
	
	protected SCR_HoverDetectorComponent m_HoverDetector;
	
	protected ref SCR_WorkshopItem m_Item;
	
	ButtonWidget m_wRoot;
	
	override void HandlerAttached(Widget w)
	{
		widgets.Init(w);
		m_wRoot = ButtonWidget.Cast(w);
		
		m_HoverDetector = SCR_HoverDetectorComponent.Cast(w.FindHandler(SCR_HoverDetectorComponent));		
		m_HoverDetector.m_OnHoverDetected.Insert(OnHoverDetected);
	}
	
	void SetWorkshopItem(SCR_WorkshopItem item)
	{
		m_Item = item;
		
		BackendImage img = item.GetThumbnail();
		
		widgets.m_BackendImageComponent.SetWorkshopItemAndImage(item, img);
	}
	
	void OnHoverDetected()
	{
		Widget w = SCR_TooltipManagerEntity.CreateTooltip("{F0BE138815397F49}UI/AddonBarIconTooltip.layout", m_wRoot);
		
		TextWidget wText = TextWidget.Cast(w.FindAnyWidget("Text"));
		wText.SetText(m_Item.GetName());
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		ContentBrowserDetailsMenu.OpenForWorkshopItem(m_Item);
		return false;
	}
}