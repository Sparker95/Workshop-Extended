/*
Component to be attached to addon lines.
*/

//----------------------------------------------------------------------------------------------
class SCR_WorkshopAddonLineComponent : ScriptedWidgetComponent
{
	protected ref SCR_WorkshopAddonLineWidgets widgets = new SCR_WorkshopAddonLineWidgets();
	protected Widget m_wRoot;	
	protected ref SCR_WorkshopItem m_Item;
	protected bool m_bMouseOver;
	protected ref SCR_WorkshopUiCommon_DownloadSequence m_DownloadRequest;
	
	ref ScriptInvoker m_OnEnableButton = new ScriptInvoker();
	ref ScriptInvoker m_OnDisableButton = new ScriptInvoker();
	ref ScriptInvoker m_OnMouseEnter = new ScriptInvoker();
	ref ScriptInvoker m_OnMouseLeave = new ScriptInvoker();
	
	//----------------------------------------------------------------------------------------------
	SCR_WorkshopItem GetWorkshopItem() {return m_Item;}
	
	//----------------------------------------------------------------------------------------------
	Widget GetRootWidget() { return m_wRoot; }
	
	//----------------------------------------------------------------------------------------------
	void Init(SCR_WorkshopItem item)
	{
		m_Item = item;
		UpdateAllWidgets();
		
		// OnChanged is called whenever something happens with the workshop item.
		// Simplest approach is to refresh whole line associated with the workshop item on this event.
		item.m_OnChanged.Insert(UpdateAllWidgets);
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void UpdateAllWidgets()
	{	
		if (!m_Item)
			return;
	
		bool mouseOver = m_bMouseOver;
			
		// Update name
		widgets.m_NameText.SetText(m_Item.GetName());
		
		
		bool enabled = m_Item.GetEnabled();
		widgets.m_MoveRightButton.SetVisible(!enabled);
		widgets.m_MoveLeftButton.SetVisible(enabled);
		
		// Update state text
		string stateText;
		bool downloading = m_Item.GetDownloadAction() || m_Item.GetDependencyCompositeAction();
		bool problemCritical;
		string problemDescription;
		bool problem = SCR_WorkshopUiCommon.GetHighestPriorityProblemDescription(m_Item, problemDescription, problemCritical);
		if (downloading)
		{
			float progress = SCR_DownloadManager.GetItemDownloadActionsProgress(m_Item);
			stateText = string.Format("Downloading %1%%", Math.Round(100.0*progress));
			widgets.m_StateText.SetVisible(true);
		}
		/*else if (problem)
		{
			stateText = problemDescription;
			widgets.m_StateText.SetVisible(mouseOver); // Those texts are too big, had to disable
		}*/
		else
			widgets.m_StateText.SetVisible(false);
		
		widgets.m_StateText.SetText(stateText);

		
		// Update the action button text
		string actionText = SCR_WorkshopUiCommon.GetPrimaryActionName(m_Item);
		widgets.m_ActionButton.SetVisible(!actionText.IsEmpty());
		if (!actionText.IsEmpty())
		{
			SCR_ButtonEffectText e = SCR_ButtonEffectText.Cast(widgets.m_ActionButtonComponent.FindEffect("text"));
			e.m_sDefault = actionText;
			e.m_sHovered = actionText;
			e.OnStateDefault(true); // todo remake the fn to reapply effects
		}
		
		
		
	}	
	
	//----------------------------------------------------------------------------------------------
	// Custom mouse enter/leave events, because when mouse enters one of buttons inside
	// the native OnMouseLeave event is fired
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_bMouseOver = true;
		UpdateAllWidgets();
		m_OnMouseEnter.Invoke(this);
		return true;
	}
	

	//----------------------------------------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_bMouseOver = false;
		UpdateAllWidgets();
		m_OnMouseLeave.Invoke(this);
		return true;
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected bool GetMouseOver()
	{
		// Check if mouse is over the line
		int xInt, yInt;
		WidgetManager.GetMousePos(xInt, yInt);
		float mx = xInt;
		float my = yInt;
		
		float posx, posy, sizex, sizey;
		m_wRoot.GetScreenPos(posx, posy);
		m_wRoot.GetScreenSize(sizex, sizey);
		
		bool mouseOver =	(mx >= posx) && (mx <= posx + sizex) &&
							(my >= posy) && (my <= posy + sizey);
		
		return mouseOver;
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void OnDeleteButton()
	{
		if (!m_Item)
			return;
		
		SCR_DeleteAddonDialog.CreateDeleteAddon(m_Item);
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void OnOpenDetailsButton()
	{
		if (!m_Item)
			return;
		
		ContentBrowserDetailsMenu.OpenForWorkshopItem(m_Item);
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void OnActionButton()
	{
		if (!m_Item)
			return;
		
		SCR_WorkshopUiCommon.ExecutePrimaryAction(m_Item, m_DownloadRequest);
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void OnEnableButton()
	{
		if (!m_Item)
			return;
		
		m_OnEnableButton.Invoke(this);
	}
	
	
	//----------------------------------------------------------------------------------------------
	protected void OnDisableButton()
	{
		if (!m_Item)
			return;
		
		m_OnDisableButton.Invoke(this);
	}
	
	
	//----------------------------------------------------------------------------------------------
	override void HandlerAttached(Widget w)
	{
		m_wRoot = w;
		if (!SCR_Global.IsEditMode())
		{
			widgets.Init(w);
			
			widgets.m_DeleteButtonComponent.m_OnClicked.Insert(OnDeleteButton);
			widgets.m_OpenDetailsButtonComponent.m_OnClicked.Insert(OnOpenDetailsButton);
			widgets.m_ActionButtonComponent.m_OnClicked.Insert(OnActionButton);
			widgets.m_MoveRightButtonComponent.m_OnClicked.Insert(OnEnableButton);
			widgets.m_MoveLeftButtonComponent.m_OnClicked.Insert(OnDisableButton);
			
			//GetGame().GetCallqueue().CallLater(OnEachFrame, 1, true);
		}
	}
	
	
	//----------------------------------------------------------------------------------------------
	override void HandlerDeattached(Widget w)
	{
		//if (!SCR_Global.IsEditMode())
		//	GetGame().GetCallqueue().Remove(OnEachFrame);
		
		if (m_Item)
			m_Item.m_OnChanged.Remove(UpdateAllWidgets); // Remember to unsubscribe from OnChanged event when this UI component is destroyed!
	}
	
	
	//----------------------------------------------------------------------------------------------
	void OnEachFrame()
	{
		float tDelta = ftime / 1000.0;
		//Update(tDelta);
	}
}
