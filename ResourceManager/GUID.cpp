#include "GUID.h"

std::unordered_map<int, std::string> GUIDtoAsset 
{ 
	{0, "blabla"}, 
	{42, "assets/cat.png"} 
};

// Apparently it's ok to hardcode the GUID table :)

//For this assignment the mapping table from GUIs to concrete assets within
//packages can be created by hand, in a real scenario this would be a task for a
//pipeline conditioning tool which can process the assets tree and create a bundle
//and all the GUIs.