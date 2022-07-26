//! Meta data of one addon
class SCR_WorkshopAddonPresetAddonMeta : JsonApiStruct
{
	
	//----------------------------------------------------------
	// Serialized variables
	protected string m_sGuid;
	protected string m_sName;
	//----------------------------------------------------------
	
	
	
	string GetName()
	{
		return m_sName;
	}
	
	string GetGuid()
	{
		return m_sGuid;
	}
	
	
	
	SCR_WorkshopAddonPresetAddonMeta Copy()
	{
		SCR_WorkshopAddonPresetAddonMeta a = (new SCR_WorkshopAddonPresetAddonMeta).Init(m_sGuid, m_sName);
		return a;
	}
	
	// We must keep constructor without arguments, otherwise JSON serializer has problems
	SCR_WorkshopAddonPresetAddonMeta Init(string guid, string name)
	{
		m_sGuid = guid;
		m_sName = name;
		
		return this;
	}
	
	void SCR_WorkshopAddonPresetAddonMeta()
	{
		RegV("m_sGuid");
		RegV("m_sName");
	}
}



//! Preset of addons
class SCR_WorkshopAddonPreset : JsonApiStruct
{
	protected string m_sName;							// Preset name
	protected ref array<ref SCR_WorkshopAddonPresetAddonMeta> m_aAddonsMeta;	// Array with addon GUIDs
	
	
	string GetName()
	{
		return m_sName;
	}
	
	// We must keep constructor without arguments, otherwise JSON serializer has problems
	SCR_WorkshopAddonPreset Init(string name, notnull array<ref SCR_WorkshopAddonPresetAddonMeta> addons)
	{
		m_sName = name;
		m_aAddonsMeta = {};
		
		foreach (auto meta : addons)
			m_aAddonsMeta.Insert(meta.Copy());
		
		return this;
	}
	
	SCR_WorkshopAddonPreset Copy()
	{
		SCR_WorkshopAddonPreset a = (new SCR_WorkshopAddonPreset).Init(m_sName, m_aAddonsMeta);
		return a;
	}
	
	void SCR_WorkshopAddonPreset()
	{
		RegV("m_sName");
		RegV("m_aAddonsMeta");
	}
	
	int GetAddonCount()
	{
		return m_aAddonsMeta.Count();
	}
	
	array<ref SCR_WorkshopAddonPresetAddonMeta> GetAddons()
	{
		array<ref SCR_WorkshopAddonPresetAddonMeta> outArray = {};
		foreach (auto m : m_aAddonsMeta)
			outArray.Insert(m);
		
		return outArray;
	}
}


// Class which manages storage of multiple addon presets
class WEXT_Storage : JsonApiStruct
{	
	//----------------------------------------------------------
	// Serialized variables
	protected int m_iVersion; // Might be useful for future versioning
	protected ref array<ref SCR_WorkshopAddonPreset> m_aPresets;
	//----------------------------------------------------------
	
	
	const int VERSION = 1;
	const string FILE_NAME = "$profile:WorkshopExtended.json";
	
	void WEXT_Storage()
	{
		RegV("m_iVersion");
		RegV("m_aPresets");
		
		// When constructed, read all data from storage into RAM.
		// When presets will be modified, or created, we will override whole WEXT_Storage object in storage.
		ReadPresetsFromStorage();
	}
	
	
	
	
	//-----------------------------------------------------------------------------------------
	// PUBLIC
	
	array<ref SCR_WorkshopAddonPreset> GetAllPresets()
	{
		array<ref SCR_WorkshopAddonPreset> arrayOut = {};

		foreach (SCR_WorkshopAddonPreset preset : m_aPresets)
		{
			arrayOut.Insert(preset);
		}

		return arrayOut;
	}
	
	
	void SavePreset(notnull SCR_WorkshopAddonPreset preset)
	{
		string name = preset.GetName();
		
		int id = GetPresetId(name);
		
		// Make a copy of provided data,
		// either append it to array or replace prev. preset with same name
		
		SCR_WorkshopAddonPreset presetCopy = preset.Copy();
		
		if (id != -1)
			m_aPresets[id] = presetCopy;
		else
			m_aPresets.Insert(presetCopy);
		
		// Save everything to storage
		SavePresetsToStorage();
	}
	
	
	void DeletePreset(string name)
	{
		int id = GetPresetId(name);
		
		if (id == -1)
			return;
		
		m_aPresets.RemoveOrdered(id);
		
		SavePresetsToStorage();
	}
	
	
	bool PresetExists(string name)
	{
		return GetPresetId(name) != -1;
	}
	
	
	// Reads all presets from storage
	bool ReadPresetsFromStorage()
	{
		m_iVersion = -1;
		m_aPresets = {};
		
		bool expandSuccess = false;
		
		FileHandle f = FileIO.OpenFile(FILE_NAME, FileMode.READ);
		if (f != 0)
		{
			string fileJsonData;
			int fNumRead = f.FGets(fileJsonData);
			f.CloseFile();
			if (fNumRead != -1)
			{
				ExpandFromRAW(fileJsonData);
			}
		}
		
		//expandSuccess = LoadFromFile(FILE_NAME);
		
		if (!m_aPresets)
			m_aPresets = {};

		return true;
	}
	
	SCR_WorkshopAddonPreset GetPreset(string name)
	{
		int id = GetPresetId(name);
		
		if (id == -1)
			return null;
		
		return m_aPresets[id];
	}
	
	//-----------------------------------------------------------------------------------------
	// PROTECTED
	

	//! Saves all presets to storage
	protected void SavePresetsToStorage()
	{		
		m_iVersion = VERSION;
		//InstantPack();
		//string packedJson = AsString();
		//Print(packedJson);
		PackToFile(FILE_NAME);
	}
	
	protected int GetPresetId(string name)
	{
		foreach (int i, SCR_WorkshopAddonPreset preset : m_aPresets)
		{
			if (preset.GetName() == name)
				return i;
		}
		
		return -1;
	}
}