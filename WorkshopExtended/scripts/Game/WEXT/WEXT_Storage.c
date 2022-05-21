//! Meta data of one addon
class WEXT_AddonMeta : JsonApiStruct
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
	
	
	
	WEXT_AddonMeta Copy()
	{
		WEXT_AddonMeta a = (new WEXT_AddonMeta).Init(m_sGuid, m_sName);
		return a;
	}
	
	// We must keep constructor without arguments, otherwise JSON serializer has problems
	WEXT_AddonMeta Init(string guid, string name)
	{
		m_sGuid = guid;
		m_sName = name;
		
		return this;
	}
	
	void WEXT_AddonMeta()
	{
		RegV("m_sGuid");
		RegV("m_sName");
	}
}



//! Preset of addons
class WEXT_AddonPreset : JsonApiStruct
{
	protected string m_sName;							// Preset name
	protected ref array<ref WEXT_AddonMeta> m_aAddonsMeta;	// Array with addon GUIDs
	
	
	string GetName()
	{
		return m_sName;
	}
	
	// We must keep constructor without arguments, otherwise JSON serializer has problems
	WEXT_AddonPreset Init(string name, notnull array<ref WEXT_AddonMeta> addons)
	{
		m_sName = name;
		m_aAddonsMeta = {};
		
		foreach (auto meta : addons)
			m_aAddonsMeta.Insert(meta.Copy());
		
		return this;
	}
	
	WEXT_AddonPreset Copy()
	{
		WEXT_AddonPreset a = (new WEXT_AddonPreset).Init(m_sName, m_aAddonsMeta);
		return a;
	}
	
	void WEXT_AddonPreset()
	{
		RegV("m_sName");
		RegV("m_aAddonsMeta");
	}
	
	int GetAddonCount()
	{
		return m_aAddonsMeta.Count();
	}
	
	array<ref WEXT_AddonMeta> GetAddons()
	{
		array<ref WEXT_AddonMeta> outArray = {};
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
	protected ref array<ref WEXT_AddonPreset> m_aPresets;
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
	
	array<ref WEXT_AddonPreset> GetAllPresets()
	{
		array<ref WEXT_AddonPreset> arrayOut = {};

		foreach (WEXT_AddonPreset preset : m_aPresets)
		{
			arrayOut.Insert(preset);
		}

		return arrayOut;
	}
	
	
	void SavePreset(notnull WEXT_AddonPreset preset)
	{
		string name = preset.GetName();
		
		int id = GetPresetId(name);
		
		// Make a copy of provided data,
		// either append it to array or replace prev. preset with same name
		
		WEXT_AddonPreset presetCopy = preset.Copy();
		
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
	
	WEXT_AddonPreset GetPreset(string name)
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
		foreach (int i, WEXT_AddonPreset preset : m_aPresets)
		{
			if (preset.GetName() == name)
				return i;
		}
		
		return -1;
	}
}