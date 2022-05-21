class WEXT_Dialogs
{
	const string DIALOGS_CONFIG = "{F96E0133AC81125A}UI/Dialogs.conf";
}

// Dialog with an edit box (not used, was an experiment)
class WEXT_DialogEdit : SCR_ConfigurableDialogUi
{
	void WEXT_DialogEdit(string editText)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "edit_ok", this);
		SetEditText(editText);
	}
	
	void SetEditText(string text)
	{
		EditBoxWidget w = EditBoxWidget.Cast(GetRootWidget().FindAnyWidget("MessageEdit"));
		w.SetText(text);
	}
}


class WEXT_DialogCopyToClipboard : SCR_ConfigurableDialogUi
{
	string m_sText;
	
	void WEXT_DialogCopyToClipboard(string text)
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
class WEXT_ErrorDialog : SCR_ConfigurableDialogUi
{
	void WEXT_ErrorDialog(string message)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "error", this);
		SetMessage(message);
	}
}



// Dialog which also stores reference to addon preset
class WEXT_PresetConfirmDialog : SCR_ConfigurableDialogUi
{
	// "preset" term is used both in WEXT and Configurable Dialogs, it's a bit confusing
	
	// We store ptr to preset which we are going to delete/create,
	// when ok btn is presset we will use it
	ref WEXT_AddonPreset m_Preset;
	string m_sPresetName;
	
	void WEXT_PresetConfirmDialog(WEXT_AddonPreset preset, string presetName, string dlgPreset)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, dlgPreset, this);
		m_Preset = preset;
		m_sPresetName = presetName;
	}
	
	static WEXT_PresetConfirmDialog CreateDeletePresetDialog(string presetName)
	{
		return new WEXT_PresetConfirmDialog(null, presetName, "delete_preset");
	}
	
	static WEXT_PresetConfirmDialog CreateOverridePresetDialog(WEXT_AddonPreset preset)
	{
		return new WEXT_PresetConfirmDialog(preset, preset.GetName(), "override_preset");
	}
}

// Dialog which shows error and lists addons which we failed to load in a preset
class WEXT_ErrorPresetLoadDialog : SCR_ConfigurableDialogUi
{
	void WEXT_ErrorPresetLoadDialog(array<ref WEXT_AddonMeta> notFoundAddons)
	{
		SCR_ConfigurableDialogUi.CreateFromPreset(WEXT_Dialogs.DIALOGS_CONFIG, "error_preset_load", this);
		
		string msg = "These mods are not downloaded:\n\n";
		
		foreach (WEXT_AddonMeta meta : notFoundAddons)
		{
			msg = msg + string.Format("%1 %2\n", meta.GetGuid(), meta.GetName());
		}
		
		SetMessage(msg);
	}
}