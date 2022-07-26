class WEXT_Dialogs
{
	const string DIALOGS_CONFIG = "{F96E0133AC81125A}UI/Dialogs.conf";
}

class SCR_WorkshopDialogCopyToClipboard : SCR_ConfigurableDialogUi
{
	string m_sText;
	
	void SCR_WorkshopDialogCopyToClipboard(string text)
	{
		m_sText = text;
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "copy_to_clipboard", this);
		TextWidget tw = TextWidget.Cast(GetRootWidget().FindAnyWidget("MessageInScroll"));
		tw.SetText(text);
	}
	
	override void OnConfirm()
	{
		System.ExportToClipboard(m_sText);
	}
}

// Basic error dialog with a message
class SCR_WorkshopErrorDialog : SCR_ConfigurableDialogUi
{
	void SCR_WorkshopErrorDialog(string message)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "error", this);
		SetMessage(message);
	}
}



// Dialog which also stores reference to addon preset
class SCR_WorkshopPresetConfirmDialog : SCR_ConfigurableDialogUi
{
	// "preset" term is used both in WEXT and Configurable Dialogs, it's a bit confusing
	
	// We store ptr to preset which we are going to delete/create,
	// when ok btn is presset we will use it
	ref SCR_WorkshopAddonPreset m_Preset;
	string m_sPresetName;
	
	void SCR_WorkshopPresetConfirmDialog(SCR_WorkshopAddonPreset preset, string presetName, string dlgPreset)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, dlgPreset, this);
		m_Preset = preset;
		m_sPresetName = presetName;
	}
	
	static SCR_WorkshopPresetConfirmDialog CreateDeletePresetDialog(string presetName)
	{
		return new SCR_WorkshopPresetConfirmDialog(null, presetName, "delete_preset");
	}
	
	static SCR_WorkshopPresetConfirmDialog CreateOverridePresetDialog(SCR_WorkshopAddonPreset preset)
	{
		return new SCR_WorkshopPresetConfirmDialog(preset, preset.GetName(), "override_preset");
	}
}

// Dialog which shows error and lists addons which we failed to load in a preset
class SCR_WorkshopErrorPresetLoadDialog : SCR_ConfigurableDialogUi
{
	void SCR_WorkshopErrorPresetLoadDialog(array<ref SCR_WorkshopAddonPresetAddonMeta> notFoundAddons)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "error_preset_load", this);
		
		string msg = "These mods are not downloaded:\n\n";
		
		foreach (SCR_WorkshopAddonPresetAddonMeta meta : notFoundAddons)
		{
			msg = msg + string.Format("%1 %2\n", meta.GetGuid(), meta.GetName());
		}
		
		SetMessage(msg);
	}
}